#include <windows.h>
#include <gl/Gl.h>
#include <gl/glut.h>  //OpenGL Utility Toolkit for creating openGL window
#include <iostream> 
#include <vector> 
#include <chrono>
#include <thread>
#include <cmath>
#include <cstdlib>
#include <stdlib.h>
#include <random>

using namespace std;
GLint screenHeight = 768;

bool firstClicked = false;
bool isLine = false;
bool isWindow = false;


class GLintPoint {
public:
	GLint x, y;
};

class polyline {
public:
	GLintPoint p1, p2;
	void reset() {
		p1.x = 0;
		p1.y = 0;
		p2.x = 0;
		p2.y = 0;
	}
 
};



vector <polyline> lines;
polyline poly, frame,line;

GLint leftB, rightB, top, bottom;

void draw() {

	if (isWindow) {
		glClearColor(1.0, 1.0, 1.0, 0.0);
		glClear(GL_COLOR_BUFFER_BIT);
		glBegin(GL_LINE_LOOP);
		glVertex2i(poly.p1.x, poly.p1.y);
		glVertex2i(poly.p2.x, poly.p1.y);
		glVertex2i(poly.p2.x, poly.p2.y);
		glVertex2i(poly.p1.x, poly.p2.y);
		glEnd();
	}
	else if (isLine) {
		glClearColor(1.0, 1.0, 1.0, 0.0);
		glClear(GL_COLOR_BUFFER_BIT);
		glBegin(GL_LINES);
		glVertex2i(poly.p1.x, poly.p1.y);
		glVertex2i(poly.p2.x, poly.p2.y);
		glEnd();


	}
}

void myMovedMouse(int mouseX, int mouseY) {

	if (firstClicked) {

		poly.p2.x = mouseX;
		poly.p2.y = screenHeight - mouseY;
		draw();
		glFlush();
	}
}

void myInit(void) {
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glColor3f(0.0f, 0.0f, 0.0f);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0, 1366.0, 0.0, 768.0);

}

void drawWindow() {
 
	glBegin(GL_LINE_LOOP);
	glVertex2i(frame.p1.x, frame.p1.y);
	glVertex2i(frame.p2.x, frame.p1.y);
	glVertex2i(frame.p2.x, frame.p2.y);
	glVertex2i(frame.p1.x, frame.p2.y);
	glEnd();
}

void drawLine() {
	for (std::vector<polyline>::size_type j = 0; j != lines.size(); j++) {
 
		glBegin(GL_LINES);
		glVertex2i(lines[j].p1.x, lines[j].p1.y);
		glVertex2i(lines[j].p2.x, lines[j].p2.y);
		glEnd();
	}

}

void myDisplay(void) {

	glClearColor(1.0, 1.0, 1.0, 1.0); // set up white clear color
	glClear(GL_COLOR_BUFFER_BIT);   // clear the back ground (white)
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();                   // setup model matrix
	drawWindow();
	drawLine();
	glPopMatrix();                    // reset model matrix
	glFlush();
}

char encode(GLintPoint P) {
	unsigned char code = 0;													//encoding the position
	if (P.x < (frame.p1.x < frame.p2.x ? frame.p1.x : frame.p2.x)) code |= 8; // set bit 3
	if (P.y > (frame.p1.y < frame.p2.y ? frame.p2.y : frame.p1.y)) code |= 4; // set bit 2
	if (P.x > (frame.p1.x < frame.p2.x ? frame.p2.x : frame.p1.x)) code |= 2; // set bit 1
	if (P.y < (frame.p1.y < frame.p2.y ? frame.p1.y : frame.p2.y)) code |= 1; // set bit 0
	return code;
}

void ChopLine(GLintPoint P, unsigned char code, int slope) {


	if (code & 8) { // to the Left
		P.y = (leftB - P.x) * slope + P.y;
		P.x = leftB;
	}
	else if (code & 2) { // to the Right
		P.y = (rightB - P.x) * slope + P.y;
		P.x = rightB;
	}
	else if (code & 1) { // below
		P.x = (bottom - P.y) / slope + P.x;
		P.y = bottom;
	}
	else if (code & 4) { // above
		P.x = (top - P.y) / slope + P.x;
		P.y = top;
	}
}

void Cohen_SutherLand() {
 
	while (true) {

		unsigned code1 = encode(line.p1);
		unsigned code2 = encode(line.p2);

		//or test
		if ((code1 | code2) == 0) {
 
			lines.push_back(line);
			return;					//trival accepted
		}
		//and test (is result have at least one 1 bit
		if ((code1 & code2) != 0) {
 
			return; //trival rejected
		}
		//calc
		//calc the slope
		int m = (line.p2.y - line.p1.y) / (line.p2.x - line.p1.x);
		//check the four cases
		ChopLine(line.p1, code1, m);
		ChopLine(line.p2, code1, m);
	}
}

void myMouse(int button, int state, int x, int y) {

	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		poly.p1.x = x;
		poly.p1.y = screenHeight - y;
		firstClicked = true;
	}
	if (button == GLUT_LEFT_BUTTON && state == GLUT_UP) {
		firstClicked = false;
		if (isWindow) {
			isWindow = false;
			frame = poly;
			leftB = (frame.p1.x < frame.p2.x ? frame.p1.x : frame.p2.x);
			top = (frame.p1.y < frame.p2.y ? frame.p2.y : frame.p1.y);
			rightB = (frame.p1.x < frame.p2.x ? frame.p2.x : frame.p1.x);
			bottom = (frame.p1.y < frame.p2.y ? frame.p1.y : frame.p2.y);

		}
		else if (isLine) {
			isLine = false;
			line = poly;
			Cohen_SutherLand();
		}
		myDisplay();
	}
}

void myKeyboard(unsigned char theKey, int mouseX, int mouseY) {

	GLint x = mouseX;
	GLint y = screenHeight - mouseY;
	switch (theKey) {

	case 'w':
		frame.reset();
		lines.clear();
		isWindow = true;
		break;
	case 'l':
		isLine = true;
		break;

	default:
		break;
	}
}

int main(int argc, char** argv) {

	glClear(GL_COLOR_BUFFER_BIT);
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(1366, 768);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("Untitled");
	glutDisplayFunc(myDisplay);
	glutMouseFunc(myMouse);
	glutMotionFunc(myMovedMouse);
	glutKeyboardFunc(myKeyboard);
	myInit();
	glutMainLoop();
	return 0;
}