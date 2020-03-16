#include <windows.h>
#include <gl/Gl.h>
#include <gl/glut.h>  //OpenGL Utility Toolkit for creating openGL window

void myInit(void) {
	glClearColor(1.0, 1.0, 1.0, 0.0);
	glColor3f(0.0f, 0.0f, 0.0f);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0, 640.0, 0.0, 480.0);
}

class GLintPoint {
public:
	GLint x, y;
};

int random(int m) {
	return rand() % m;
}

 

void Sierpinski(void) {
	GLintPoint T[3] = { { 10, 10 }, {300, 30}, {200, 300} };

	int index = random(3);
	GLintPoint point = T[index];

	glClearColor(1.0, 1.0, 1.0, 1.0); // set up white clear color
	glClear(GL_COLOR_BUFFER_BIT);   // clear the back ground (white)

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();                   // setup model matrix
	glScalef(1.5f, 1.5f, 1.0f);     // scale the point distribution

	glColor3f(0.0f, 0.0f, 0.0f);    // set black draw color
	glPointSize(5.0f);              // set the size of the points 
	glBegin(GL_POINTS);
	for (int i = 0; i < 20000; i++)
	{
		index = random(3);
		point.x = (point.x + T[index].x) / 2;
		point.y = (point.y + T[index].y) / 2;
		drawDot(point.x, point.y);
	}
	glEnd();
	glPopMatrix();                    // reset model matrix
	glFlush();
}

void main(int argc, char** argv)
{
	glClear(GL_COLOR_BUFFER_BIT);
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(640, 480);
	glutInitWindowPosition(100, 150);
	glutCreateWindow("Sierpinski Gasket");
	glutDisplayFunc(Sierpinski);
	myInit();
	glutMainLoop();
}