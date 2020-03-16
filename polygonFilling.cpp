#include <windows.h>
#include <gl/Gl.h>
#include <gl/glut.h>  //OpenGL Utility Toolkit for creating openGL window
#include <iostream> 
#include <vector> 
#include <chrono>
#include <thread>
#include <cmath>

#include <algorithm> 


using namespace std;
using std::vector;
using namespace std::this_thread; // sleep_for, sleep_until
using namespace std::chrono; // nanoseconds, system_clock, seconds

class GLintPoint {
public:
	GLint x, y;
};

class GlobalNode {
public:
	GLint ymin, ymax;
	GLfloat	xval, islope;
};

class ActiveNode {
public:
	GLint ymax;
	GLfloat xval, islope;
};

vector<GLintPoint> vertices;

int screenHeight = 640;
GLintPoint cVertex;
unsigned char gTheKey;
vector<ActiveNode> activeTable;
vector<GlobalNode> globalTable;
bool isDraw = false;
GLint scanLine;

void myInit(void) {
	glClearColor(1.0, 1.0, 1.0, 0.0);
	glColor3f(0.0f, 0.0f, 0.0f);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0, 800.0, 0.0, 640.0);
}

void myDisplay(void) {
	glClearColor(1.0, 1.0, 1.0, 1.0); // set up white clear color
	glClear(GL_COLOR_BUFFER_BIT);   // clear the back ground (white)
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
}

void refresh(bool isClosed) {
	glBegin(isClosed ? GL_LINE_LOOP : GL_LINE_STRIP);
	for (vector<GLintPoint>::size_type j = 0; j != vertices.size(); j++) {
		glVertex2i(vertices[j].x, vertices[j].y);
	}
	glEnd();
	glFlush();
}

void addNewLine(bool isClosed) {
	vertices.push_back(cVertex);
	cout << cVertex.x << ", " << cVertex.y << "\n";
	refresh(isClosed);
}

double distanceBetweenTwoPoints(GLint x, GLint y, GLint a, GLint b) {
	return sqrt(pow(x - a, 2) + pow(y - b, 2));
}

bool findNearestPoint() {
	for (std::vector<GLintPoint>::size_type j = 0; j != vertices.size(); j++) {
		double dis = distanceBetweenTwoPoints(cVertex.x, cVertex.y, vertices[j].x, vertices[j].y);
		if (dis < 20.0) {
			cVertex = { vertices[j].x, vertices[j].y };
			return true;
		}
	}
	return false;
}

void updateActiveTable() {
	for (vector<GLintPoint>::size_type i = 0; i != activeTable.size(); i++) {
		activeTable[i].xval += activeTable[i].islope;
	}
}

bool cmpActive(ActiveNode a1, ActiveNode a2) {
	if (a1.xval == a2.xval) {
		return (a1.ymax < a2.ymax);
	}
	else {
		return (a1.xval < a2.xval);
	}
}

void reorderActiveTable() {
	sort(activeTable.begin(), activeTable.end(), cmpActive);
}

bool cmpGlobal(GlobalNode g1, GlobalNode g2) {
	if (g1.ymin == g2.ymin) {
		return (g1.xval < g2.xval);
	}
	else {
		return (g1.ymin < g2.ymin);
	}
}

void initGlobalTable() {
	//create the table
	for (vector<GLintPoint>::size_type i = 0; i != vertices.size() - 1; i++) {
		int ind = (i + 1);			//edge (firstNode ,lastNode)
		GLint dx = (vertices[ind].x - vertices[i].x);
		GLint dy = (vertices[ind].y - vertices[i].y);
		if (dy == 0) {
			continue;									//skip the edge if its slope is zero (horz. edge)
		}

		GLfloat islope = (GLfloat)dx / (GLfloat)dy;
		GLint ymin, ymax, xval;
		if (vertices[ind].y < vertices[i].y) {
			ymin = vertices[ind].y;
			ymax = vertices[i].y;
			xval = vertices[ind].x;
		} else if (vertices[ind].y > vertices[i].y) {
			ymin = vertices[i].y;
			ymax = vertices[ind].y;
			xval = vertices[i].x;
		} else {
			ymin = vertices[i].y;
			ymax = vertices[i].y;
			xval = (vertices[ind].x < vertices[i].x ? vertices[i + 1].x : vertices[i].x);  //if y eqal, pick smallest x
		}
		GlobalNode gnode = { ymin, ymax, xval, islope };
		globalTable.push_back(gnode);
	}

	//sort the table inc, according to ymin -> xval
	sort(globalTable.begin(), globalTable.end(), cmpGlobal);
}

void updateActiveTableEdges() {
	vector< GlobalNode >::iterator it = globalTable.begin();
	for (vector<GLintPoint>::size_type i = 0; i != globalTable.size(); i++) {
		if (globalTable[i].ymin != scanLine) {
			break;
		}
		else {
			ActiveNode gnode = { globalTable[i].ymax, globalTable[i].xval, globalTable[i].islope };
			activeTable.push_back(gnode);
			globalTable.erase(globalTable.begin() + i);
			i--;
		}
	}
}

void updatePosition(int button, int state, int x, int y) {

	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		cVertex.x = x;
		cVertex.y = screenHeight - y;		// flip y coordinate

		switch (gTheKey) {
		case 'b':
			if (findNearestPoint())
				addNewLine(true);
			else
				addNewLine(false);

			break;
		default:
			break;
		}
	}
}

void polygonFilling() {
	// init SL -> fill the slots -> SL++ -> check SL == ymax (active) -> update -> SL == ymin (global) -> order

	//init global table
	initGlobalTable();

	//init scanline
	scanLine = globalTable[0].ymin;

	//init active table
	updateActiveTableEdges();

	while (activeTable.size() != 0) {
		//Filling the slots
		GLint ind = 0;

		for (GLint i = 0; i < 800; i++) {
			if (i == ceil(activeTable[ind].xval)) {
				isDraw = true;
			}
 
			if (isDraw) {
				glBegin(GL_POINTS);
				glVertex2i(i, scanLine);
				glEnd();
				glFlush();
			}
			if (i == ceil(activeTable[ind + 1].xval)) {
				isDraw = false;
				ind += 2;

				if (ind == activeTable.size()) {

					break;
				}
			}
		}

		refresh(true);

		//	//increase scan line
		scanLine++;

		//	//(SL == ymax) active (remove)
		for (vector<GLintPoint>::size_type i = 0; i != activeTable.size(); i++) {
			if (activeTable[i].ymax == scanLine) {
				activeTable.erase(activeTable.begin() + i);
				i--;
			}
		}

		//	//update xval
		updateActiveTable();

		//	//(SL == ymin) global(move to actitve table) 
		updateActiveTableEdges();

		//	//reorder
		reorderActiveTable();
	}
}

void myKeyboard(unsigned char theKey, int mouseX, int mouseY) {

	switch (theKey) {
	case 'b':
		gTheKey = 'b';
		break;
	case 'c':
		vertices.clear();
		refresh(false);
		break;
	case 'f':
		polygonFilling();
		break;
	case 'q':
		exit(-1);
		break;

	default:
		break;
	}
}

int main(int argc, char** argv)
{
	glClear(GL_COLOR_BUFFER_BIT);
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(800, 640);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("Painter");
	glutDisplayFunc(myDisplay);
	glutKeyboardFunc(myKeyboard);     //register keyboard func
	glutMouseFunc(updatePosition);     //register mouse func
	myInit();
	glutMainLoop();
	return 0;
}