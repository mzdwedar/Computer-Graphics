#include <windows.h>
#include <gl/Gl.h>
#include <gl/glut.h>  //OpenGL Utility Toolkit for creating openGL window
#include <iostream> 
#include <vector> 
#include <chrono>
#include <thread>
#include <cmath>


using namespace std;
using std::vector;
using namespace std::this_thread; // sleep_for, sleep_until
using namespace std::chrono; // nanoseconds, system_clock, seconds

class GLintPoint {
public:
	GLint x, y;
};

class Poly {
public:
	vector<GLintPoint> vertices;
};

void myInit(void) {
	glClearColor(1.0, 1.0, 1.0, 0.0);
	glColor3f(0.0f, 0.0f, 0.0f);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0, 800.0, 0.0, 640.0);
}



int screenHeight = 640;
int last = 0;

GLintPoint cVertex;
GLintPoint eVertex;

bool isClicked = false;
Poly cpoly;
Poly polys[60];
bool newPoly;
unsigned char gTheKey;

bool clicked = false;
GLintPoint pVertex;
GLintPoint nVertex;


double distanceBetweenTwoPoints(GLint x, GLint y, GLint a, GLint b) {
	return sqrt(pow(x - a, 2) + pow(y - b, 2));
}

void findNearestPoint() {
	for (int i = 0; i < last; i++) {
		for (std::vector<GLintPoint>::size_type j = 0; j != polys[i].vertices.size(); j++) {
			double dis = distanceBetweenTwoPoints(cVertex.x, cVertex.y, polys[i].vertices[j].x, polys[i].vertices[j].y);
			if (dis < 15.0) {
				cVertex = polys[i].vertices[j];
				return;
			}
		}
	}
}

void refresh() {

	glClearColor(1.0, 1.0, 1.0, 1.0); // set up white clear color
	glClear(GL_COLOR_BUFFER_BIT);   // clear the back ground (white)

	for (GLint i = 0; i < std::size(polys); i++) {

		glBegin(GL_LINE_STRIP);
		for (std::vector<GLintPoint>::size_type j = 0; j != polys[i].vertices.size(); j++) {
			glVertex2i(polys[i].vertices[j].x, polys[i].vertices[j].y);
			sleep_for(microseconds(1000));
		}

		glEnd();
	}

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();                   // setup model matrix
	glScalef(1.5f, 1.5f, 1.0f);     // scale the point distribution
	glPopMatrix();                    // reset model matrix
	glFlush();
}

void realocate(GLintPoint pVertex, GLintPoint newVertex) {
	for (int i = 0; i < last; i++) {
		for (std::vector<GLintPoint>::size_type j = 0; j != polys[i].vertices.size(); j++) {
			if ((polys[i].vertices[j].x == pVertex.x) & (polys[i].vertices[j].y == pVertex.y)) {
				polys[i].vertices[j] = newVertex;
				refresh();
			}
		}
	}
}

void myDisplay(void) {
	
		glClearColor(1.0, 1.0, 1.0, 1.0); // set up white clear color
		glClear(GL_COLOR_BUFFER_BIT);   // clear the back ground (white)

	}

void delVertex(GLintPoint vertex) {

	for (int i = 0; i < last; i++) {
		for (std::vector<GLintPoint>::size_type j = 0; j != polys[i].vertices.size(); j++) {
			if (polys[i].vertices[j].x == vertex.x) {
				polys[i].vertices[j] = polys[i].vertices[j + 1];
				//polys[i].vertices[j + 1].x = 0;  //what do with next vertex
				isClicked = false;
				refresh();
			}
			else {
				std::cout << "vertex is not found";
			}
		}
	}
}

void drawPoly() {
	if (newPoly) {
		polys[last += 1].vertices.push_back(cVertex);
		refresh();
		newPoly = false;
	}
	else {
		polys[last].vertices.push_back(cVertex);
		refresh();
	}
}

void updatePosition(int button, int state, int x, int y) {

	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		cVertex.x = x;
		cVertex.y = screenHeight - y; // flip y coordinate
		isClicked = true;

		switch (gTheKey) {

			case 'b':
				drawPoly();
				break;
			case 'd':
				cout << "the inital vertex" << cVertex.x << "," << cVertex.y;
				findNearestPoint();
				cout << "the estimated vertex" << cVertex.x << "," << cVertex.y;
				delVertex(cVertex);
				break;
			case 'm':
				if (isClicked & (!clicked)) {
					findNearestPoint();
					pVertex = cVertex;
					
					isClicked = false;
					clicked = true;
				}

				if (isClicked & clicked) {
					nVertex = cVertex;
					realocate(pVertex, nVertex);
					cout << "the previos vertex is: " << pVertex.x << "," << pVertex.y;
					cout << "the new vertex is: " << nVertex.x << "," << nVertex.y;
					isClicked = false;
					clicked = false;
				}
			default:
				break;
		}
	}
}


void myKeyboard(unsigned char theKey, int mouseX, int mouseY) {
	cout << theKey;
	switch (theKey) {
		
		cout << theKey;
	case 'b':
		gTheKey = 'b';
		newPoly = true;
		break;

	case 'd':
		gTheKey = 'd';
		break;

	case 'm':
		gTheKey = 'm';
		break;

	case 'r':

		glClearColor(1.0, 1.0, 1.0, 1.0); // set up white clear color
		glClear(GL_COLOR_BUFFER_BIT);   // clear the back ground (white)
		glLoadIdentity(); // Reset The Projection Matrix
		glMatrixMode(GL_MODELVIEW);
		glPushMatrix();                   // setup model matrix
		glScalef(1.5f, 1.5f, 1.0f);     // scale the point distribution
		glPopMatrix();                    // reset model matrix
		glFlush();
		refresh();
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
	glutInitWindowPosition(50, 150);


	glutCreateWindow("Painter");
	glutDisplayFunc(myDisplay);
	glutKeyboardFunc(myKeyboard);     //register keyboard func
	glutMouseFunc(updatePosition);     //register mouse func
	myInit();
	glutMainLoop();
	return 0;
}