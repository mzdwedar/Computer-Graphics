#include <windows.h>
#include <gl/Gl.h>
#include <gl/glut.h>  //OpenGL Utility Toolkit for creating openGL window
#include <iostream> 
#include <vector> 
#include <chrono>
#include <thread>
#include <cmath>
 
#include <algorithm> 


using namespace std;
using std::vector;
using namespace std::this_thread; // sleep_for, sleep_until
using namespace std::chrono; // nanoseconds, system_clock, seconds
 
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

void triangle(GLint x1, GLint y1, GLint x2, GLint y2, GLint x3, GLint y3) {
	glBegin(GL_TRIANGLES);
	glVertex3i(x1, y1, 0);
	glVertex3i(x2, y2, 0);
	glVertex3i(x3, y3, 0);
	glEnd();
}

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

void myDisplay(void) {
	glClearColor(1.0, 1.0, 1.0, 1.0); // set up white clear color
	glClear(GL_COLOR_BUFFER_BIT);   // clear the back ground (white)
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();

	//green
	glColor3f(0.0f, 1.0f, 0.0f);
	glRecti(0, 0, 800, 640 / 5);

	//blue
	glColor3f(0.0f, 0.0f, 1.0f);
	glRecti(0, 640/5, 800, 640);

	//sun
	glColor3f(1.0f, 1.0f, 0.0f);
	GLintPoint sunCenter;
	sunCenter.x = 50;
	sunCenter.y = 640 - 50;
	filledCircle(sunCenter, 40);

	//two tree
	glColor3f(0.5f, 0.35f, 0.05f);
	glRecti(60, 640 / 5, 80, 640 / 2);
	glRecti(800 - 80, 640 / 5, 800 - 60, 640 / 2);

	glColor3f(0.0f, 1.0f, 0.0f);

	GLintPoint treeCenter1;
	treeCenter1.x = 70;
	treeCenter1.y = 640 / 2;
	filledCircle(treeCenter1, 50);
	
	GLintPoint treeCenter2;
	treeCenter1.x = 800-70;
	treeCenter1.y = 640 / 2;
	filledCircle(treeCenter1, 50);

	//house
	glColor3f(0.5f, 0.5f, 0.7f); //pink
	glRecti(800 / 4, 640 / 5, (3 * 800) / 4, (3*640) / 5);
	glColor3f(0.0f, 1.0f, 0.0f); //pink
	triangle(800 / 4, (3 * 640) / 5, (3 * 800) / 4, (3 * 640) / 5, 800 / 2, (3 * 640) / 5 + 50);

	//the door
	GLint doorX = ((800 / 4) + (3 * 800) / 4 ) / 2;
	GLint doorY = ((3 * 640) / 5 + (640 / 5)) / 2; 
	
	glColor3f(0.5f, 0.35f, 0.05f);
	glRecti(doorX, 640 / 5, doorX + 80, doorY);

	GLint windX = (800 / 4) + 40;
	GLint windY	= (3 * 640) / 5 - 40;
	glRecti(windX, windY, windX + 60, windY - 60);

	GLint wind2X = (3 * 800) / 4 - 40;
	GLint wind2Y = (3 * 640) / 5 - 40;
	glRecti(wind2X, wind2Y, wind2X - 60, wind2Y - 60);


	//the cloud
	GLintPoint cloudCenter;
	cloudCenter.x = (800 - 130);
	cloudCenter.y = (640 - 90);

	glColor3f(1.0f, 1.0f, 1.0f);
	filledCircle(cloudCenter, 60);

	cloudCenter.x = (800 - 130)+40;
	cloudCenter.y = (640 - 90)+30;
	filledCircle(cloudCenter, 40);

	cloudCenter.x = (800 - 130) - 40;
	cloudCenter.y = (640 - 90) - 30;
	filledCircle(cloudCenter, 40);

	cloudCenter.x = (800 - 130) + 70 ;
	cloudCenter.y = (640 - 90);
	filledCircle(cloudCenter, 50);


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