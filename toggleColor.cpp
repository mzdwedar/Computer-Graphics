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
#include <algorithm> 


using namespace std;

std::random_device rd;
std::default_random_engine generator(rd()); // rd() provides a random seed
std::uniform_real_distribution<double> distribution(0.1, 1);

class GLintPoint {
public:
	GLint x, y;
};

class colorPallete {
public:
	GLfloat r,g,b;
};

colorPallete genColor() {
	colorPallete temp;
	temp.r = distribution(generator);
	temp.g = distribution(generator);
	temp.b = distribution(generator);
	return temp;
}

int screenHeight = 640;
GLintPoint cVertex;
colorPallete sqr = {0.0,0.0,0.0};
colorPallete bkground = { 1.0, 1.0, 1.0 };
bool isStarted = true;

void myInit(void) {
	glClearColor(1.0, 1.0, 1.0, 0.0);
	glColor3f(0.0f, 0.0f, 0.0f);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0, 800.0, 0.0, 640.0);
}

void updatePosition(int button, int state, int x, int y) {

	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
		cVertex.x = x;
		cVertex.y = screenHeight - y;		// flip y coordinate

		if ((cVertex.x > 200) && (cVertex.y > 200) && (cVertex.x < 600) && (cVertex.y < 400)) {
			sqr = genColor();
			bkground = genColor();
		}
	}
}

void myDisplay() {
	glClearColor(1.0, 1.0, 1.0, 1.0); // set up white clear color
	glClear(GL_COLOR_BUFFER_BIT);   // clear the back ground (white)
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();

 
	glColor3f(bkground.r, bkground.g, bkground.b);
	glRecti(0, 0, 800, 640);

	glColor3f(sqr.r, sqr.b, sqr.b);
	glRecti(200, 200, 600, 400);
 
	glPopMatrix();                    // reset model matrix
	glFlush();
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
 
	glutMouseFunc(updatePosition);     //register mouse func
	myInit();
	glutMainLoop();
	return 0;
}