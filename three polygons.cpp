#include <windows.h>
#include <gl/Gl.h>
#include <gl/glut.h>  //OpenGL Utility Toolkit for creating openGL window

void myInit(void) {
	glClearColor(1.0, 1.0, 1.0, 0.0);
	glColor3f(0.0f, 0.0f, 0.0f);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0, 1000.0, 0.0, 480.0);
}

class GLintPoint {
public:
	GLint x, y;
};

 

void polygon(void) {
	glClearColor(1.0, 1.0, 1.0, 1.0); // set up white clear color
	glClear(GL_COLOR_BUFFER_BIT);   // clear the back ground (white)

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();                   // setup model matrix
	glScalef(1.5f, 1.5f, 1.0f);     // scale the point distribution

	glColor3f(1.0, 0.0f, 0.0f);  //Triangle
	glBegin(GL_TRIANGLES);
	glVertex3i(20, 20, 0);
	glVertex3i(20, 100, 0);
	glVertex3i(100, 20, 0);
	glEnd();

	
	glColor3f(0.0f, 0.0f, 0.0f);
	glBegin(GL_LINES);          //line
	glVertex2i(130, 20);
	glVertex2i(130, 100);
	glEnd();

	glColor3f(0.0f, 1.0f, 0.0f); //square
	glBegin(GL_POLYGON);
	glVertex3i(150, 20, 0);
	glVertex3i(250, 20, 0);
	glVertex3i(250, 100, 0);
	glVertex3i(150, 100, 0);
	glEnd();

	glColor3f(0.0f, 0.0f, 1.0f);     //rectangle
	glRecti(300,20,450,100);
	glPopMatrix();                    // reset model matrix
	glFlush();
}

void main(int argc, char** argv)
{
	glClear(GL_COLOR_BUFFER_BIT);
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(1000, 480);
	glutInitWindowPosition(100, 150);
	glutCreateWindow("polygon");
	glutDisplayFunc(polygon);
	myInit();
	glutMainLoop();
}