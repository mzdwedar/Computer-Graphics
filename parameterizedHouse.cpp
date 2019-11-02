#include <windows.h>
#include <gl/Gl.h>
#include <gl/glut.h>  //OpenGL Utility Toolkit for creating openGL window

int screenHeight = 720;

class GLintPoint {
public:
	GLint x, y;
};

void myInit(void) {
	glClearColor(1.0, 1.0, 1.0, 0.0);
	glColor3f(0.0f, 0.0f, 0.0f);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0, 1280.0, 0.0, 720.0);
}

void parameterizedHouse(GLintPoint peak, GLint width, GLint height)
{

	glBegin(GL_LINE_LOOP);												//the shell
	glVertex2i(peak.x, peak.y);											// draw shell of house

	glVertex2i(peak.x - width / 6, peak.y - 3 * height / 24);   //chimy
	glVertex2i(peak.x - 2 * width / 12, peak.y);
	glVertex2i(peak.x - 4 * width / 12, peak.y);
	glVertex2i(peak.x - 2 * width / 6, peak.y - 6 * height / 24);

	glVertex2i(peak.x - width / 2, peak.y - 3 * height / 8);
	glVertex2i(peak.x - width / 2, peak.y - height);
	glVertex2i(peak.x + width / 2, peak.y - height);
	glVertex2i(peak.x + width / 2, peak.y - 3 * height / 8);
	glEnd();											
	
	glBegin(GL_LINE_LOOP);								//the door
	glVertex2i(peak.x - 5 * width / 12, peak.y - height);         
	glVertex2i(peak.x - 5 * width / 12, peak.y - 4 * height/8);
	glVertex2i(peak.x - 2 * width / 12, peak.y - 4 * height/8 );
	glVertex2i(peak.x - 2 * width / 12, peak.y - height);
	glEnd();

	glBegin(GL_LINE_LOOP);                             //window
	glVertex2i(peak.x + width/12, peak.y - 3 * height/5);
	glVertex2i(peak.x + 3 * width / 12, peak.y - 3 * height/5);
	glVertex2i(peak.x + 3 * width / 12, peak.y - 2 * height/5);
	glVertex2i(peak.x + width / 12, peak.y - 2 * height/5);
	glEnd();

}


void myDisplay(void) {
	glClearColor(1.0, 1.0, 1.0, 1.0); // set up white clear color
	glClear(GL_COLOR_BUFFER_BIT);   // clear the back ground (white)
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();                   // setup model matrix
	glScalef(1.5f, 1.5f, 1.0f);     // scale the point distribution

	GLintPoint peak;
	peak.x = 360;
	peak.y= 400;
	parameterizedHouse(peak, 400, 350);

	glPopMatrix();                    // reset model matrix
	glFlush();
}

void main(int argc, char** argv)
{	
	glClear(GL_COLOR_BUFFER_BIT);
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(720, 480);
	glutInitWindowPosition(50, 100);
	glutCreateWindow("House");
	glutDisplayFunc(myDisplay);
	myInit();
	glutMainLoop();
}