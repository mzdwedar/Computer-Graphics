#include <windows.h>
#include <gl/Gl.h>
#include <gl/glut.h>  //OpenGL Utility Toolkit for creating openGL window
 
GLint screenHeight = 768;
 
class GLintPoint {
public:
	GLint x, y;
};

void filledCircle(GLintPoint center, double radius) {
	float x1, y1, x2, y2;
	float angle;


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

void myInit(void) {
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glColor3f(0.0f, 0.0f, 0.0f);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0, 768.0, 0.0, 768.0);  //(768,1366)
	
} 

void myDisplay(void) {
	glClearColor(1.0, 1.0, 1.0, 1.0); // set up white clear color
	glClear(GL_COLOR_BUFFER_BIT);   // clear the back ground (white)
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();

	glColor3f(1.0f, 1.0f, 0.0f);
	glRecti(0, 0, 1366, 768);
	
	//the head
	glColor3f(1.0f, 0.0f, 0.0f);
	GLintPoint headCenter;
	headCenter.x = 768/2;
	headCenter.y = (2*768)/3;
	filledCircle(headCenter, 200);

	//the body
	glColor3f(1.0f, 0.0f, 0.0f);
	glRecti(headCenter.x - 200, headCenter.y - 300, headCenter.x + 200, headCenter.y);

	//the legs
	GLintPoint legCenter;
	legCenter.x = headCenter.x - 150;
	legCenter.y = headCenter.y - 300;
	for (int i = 0; i < 3; i++) {
		filledCircle(legCenter, 50);
		legCenter.x += 150;
	}

	//the eyess
	GLintPoint eyeCenter;
	eyeCenter.x = headCenter.x + 100;
	eyeCenter.y = headCenter.y - 50;
	for (int i = 0; i < 2; i++) {
		glColor3f(1.0f, 1.0f, 1.0f);
		filledCircle(eyeCenter, 50);

		glColor3f(0.0f, 0.0f, 1.0f);
		filledCircle(eyeCenter, 30);

		eyeCenter.x = headCenter.x - 100;
	}
 
	glPopMatrix();                    // reset model matrix
	glFlush();
}

int main(int argc, char** argv){

	glClear(GL_COLOR_BUFFER_BIT);
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(768, 768);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("Untitled");
	glutDisplayFunc(myDisplay);

	myInit();
	glutMainLoop();
	return 0;
}