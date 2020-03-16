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
using std::vector;
using namespace std::this_thread; // sleep_for, sleep_until
using namespace std::chrono; // nanoseconds, system_clock, seconds
std::random_device rd;
std::default_random_engine generator(rd()); // rd() provides a random seed
std::uniform_real_distribution<double> distribution(0.1, 1);
int screenHeight = 640;

class GLintPoint {
public:
	GLint x, y;
};

void myInit(void) {
	glClearColor(1.0, 1.0, 1.0, 0.0);
	glColor3f(0.0f, 0.0f, 0.0f);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0, 800.0, 0.0, 640.0);
}

GLintPoint randomVertex() {
	GLintPoint p;
	p.x = rand() % 600;
	p.y = rand() % 760;
	return p;
}

void voidCircle(GLintPoint center, double radius, double color[]) {
	float x1, y1, x2, y2;
	float angle;

	glColor3f(color[0], color[1], color[2]);

	glBegin(GL_LINE_STRIP);
	//glVertex2f(center.x, center.y);

	for (angle = 1.0f; angle < 361.0f; angle += 0.2)
	{
		x2 = center.x + sin(angle) * radius;
		y2 = center.y + cos(angle) * radius;
		glVertex2f(x2, y2);
	}

	glEnd();
}

void filledCircle(GLintPoint center, double radius, double color[]){
	float x1, y1, x2, y2;
	float angle;
	
	glColor3f(color[0], color[1], color[2]);

	glBegin(GL_TRIANGLE_FAN);
	glVertex2f(center.x, center.y);

	for (angle = 1.0f; angle < 361.0f; angle += 0.2)
	{
		x2 = center.x + sin(angle) * radius;
		y2 = center.y + cos(angle) * radius;
		glVertex2f(x2, y2);
	}

	glEnd();
}

void myDisplay(void) {

	glClearColor(1.0, 1.0, 1.0, 1.0); // set up white clear color
	glClear(GL_COLOR_BUFFER_BIT);   // clear the back ground (white)
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();                   // setup model matrix
	glScalef(1.5f, 1.5f, 1.0f);     // scale the point distribution
	double color[3];
	for (int i = 0; i < 100; i++)
	{
		color[0] = distribution(generator);;
		color[1] = distribution(generator);;
		color[2] = distribution(generator);;
		double rad = rand() % 40;
		GLintPoint p = randomVertex();
		if(i%2 ==0)
			filledCircle(p, rad, color);
		else
			voidCircle(p, rad, color);
	}


	glPopMatrix();                    // reset model matrix
	glFlush();

}

int main(int argc, char** argv)
{
	glClear(GL_COLOR_BUFFER_BIT);
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(800, 640);
	glutInitWindowPosition(50, 150);
	glutCreateWindow("flurry");
	glutDisplayFunc(myDisplay);

	myInit();
	glutMainLoop();
	return 0;
}