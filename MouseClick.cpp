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
int screenHeight = 768;
std::random_device rd;
std::default_random_engine generator(rd()); // rd() provides a random seed
std::uniform_real_distribution<double> distribution(0.1, 1);
bool isStarted = true;

class GLintPoint {
public:
	GLint x, y;
};

class ColoredSquare {
public:
	GLint min, max;
	double r, g, b;
};

ColoredSquare arr[9];
ColoredSquare shw;

void myInit(void) {
	glClearColor(1.0, 1.0, 1.0, 0.0);
	glColor3f(0.0f, 0.0f, 0.0f);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0, 1366.0, 0.0, 768.0);
	
}

void draw() {

	if (isStarted) {
		shw.min = 30;
		shw.max = 900;
		shw.r, shw.g, shw.b = 0, 0, 0;
		for (int i = 0; i < 9; i++) {    // fill the array
			arr[i].min = (30 + i * 100);
			arr[i].max = arr[i].min + 50;

			arr[i].r = distribution(generator);
			arr[i].g = distribution(generator);
			arr[i].b = distribution(generator);
		}
		isStarted = false;
	}	
	glColor3f(shw.r, shw.g, shw.b);
	glRecti(shw.min, 230, shw.max, 700);
	for (int i = 0; i < 9; i++) {   //draw the array

		glColor3f(arr[i].r, arr[i].g, arr[i].b);
		glRecti(arr[i].min, 30, arr[i].max, 200);
		cout << arr[i].min << ',' << 30 <<','<< arr[i].max << ',' << 200 << '\n';
	}
}

void updateColor(GLintPoint v) {
	for (int i = 0; i < 9; i++)	{
		if ((v.x >= arr[i].min) && (v.x <= arr[i].max)) {
			if ((v.y > 30) && (v.y < 200)) {

				shw.r = arr[i].r;
				shw.g = arr[i].g;
				shw.b = arr[i].b;

				draw();

				return;
			}
		}
	}
}

void updatePosition(int button, int state, int x, int y) {
	
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		GLintPoint cVertex;
		cVertex.x = x;
		cVertex.y = screenHeight - y; // flip y coordinate
		updateColor(cVertex);
		cout << "this where clicked" << cVertex.x << ',' << cVertex.y << '\n';
	}
}

void myDisplay(void) {

	glClearColor(1.0, 1.0, 1.0, 1.0); // set up white clear color
	glClear(GL_COLOR_BUFFER_BIT);   // clear the back ground (white)
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();                   // setup model matrix
	//glScalef(1.5f, 1.5f, 1.0f);     // scale the point distribution

	draw();

	glPopMatrix();                    // reset model matrix
	glFlush();

}

int main(int argc, char** argv)
{
	glClear(GL_COLOR_BUFFER_BIT);
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(1366, 768);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("Untitled");
	glutDisplayFunc(myDisplay);
	glutMouseFunc(updatePosition);
	myInit();
	glutMainLoop();
	return 0;
}