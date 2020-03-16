#include <windows.h>
#include <gl/Gl.h>
#include <gl/glut.h>  //OpenGL Utility Toolkit for creating openGL window
#include <iostream> 
#include <vector> 
#include <chrono>
#include <thread>
#include <cmath>
 
#include <algorithm> 

class GLintPoint {
public:
	GLint x, y;
};

GLintPoint CP;

void moveTo(float x, float y)
{
	CP.x = x;
	CP.y = y;
}

void lineTo(float x, float y)
{
	glBegin(GL_LINES);
	glVertex2f((GLfloat)CP.x, (GLfloat)CP.y);
	glVertex2f((GLfloat)x, (GLfloat)y); // draw the line
	glEnd();
	CP.x = x;
	CP.y = y;
	glFlush();
}

void ngon(int n, float cx, float cy, float radius, float rotAngle)
{ // assumes global Canvas object, cvs
	if (n < 3) return; // bad number of sides
	double angle = rotAngle * 3.14159265 / 180; // initial angle
	double angleInc = 2 * 3.14159265 / n; //angle increment
	moveTo(radius + cx, cy);
	for (int k = 0; k < n; k++) // repeat n times
	{
		angle += angleInc;
		lineTo(radius * cos(angle) + cx, radius * sin(angle) + cy);
	}
}

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

	ngon(7, 100,100,70,0);
	
	glPopMatrix();                    // reset model matrix
	glFlush();

}

int main(int argc, char** argv)
{
	glClear(GL_COLOR_BUFFER_BIT);
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(800, 640);
	glutInitWindowPosition(0,0);

	glutCreateWindow("Painter");
	glutDisplayFunc(myDisplay);
 
	myInit();
	glutMainLoop();
	return 0;
}