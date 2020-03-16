#include <windows.h>
#include <gl/Gl.h>
#include <gl/glut.h>  //OpenGL Utility Toolkit for creating openGL window
#include <cmath>
 
 

using namespace std;
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

void triangle(GLint x1, GLint y1, GLint x2, GLint y2, GLint x3, GLint y3) {
	glBegin(GL_TRIANGLES);
	glVertex3i(x1, y1, 0);
	glVertex3i(x2, y2, 0);
	glVertex3i(x3, y3, 0);
	glEnd();
}

void pentagon(float cx, float cy, float radius) {
	GLintPoint pent[5];
	GLintPoint temp;
	float rotAngle = 0;
	double angle = rotAngle * 3.14159265 / 180; // initial angle
	double angleInc = 2 * 3.14159265 / 5; //angle increment
	temp.x = radius + cx;
	temp.y = cy;
	//pent[0] = temp;
	for (int k = 0; k < 5; k++) // repeat n times
	{
		angle += angleInc;
		temp.x = radius * cos(angle) + cx;
		temp.y = radius * sin(angle) + cy;
		pent[k] = temp;
	}
	
	glBegin(GL_POLYGON);
	for (int i = 0; i < 5; i++) {
		glVertex2i(pent[i].x, pent[i].y);
	}
	glEnd();
}

void myInit(void) {
	glClearColor(1.0, 1.0, 1.0, 1.0);
	glColor3f(0.0f, 0.0f, 0.0f);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0, 768.0, 0.0, 750.0);  //(768,1366)
	 
}

void myDisplay(void) {
	glClearColor(1.0, 1.0, 1.0, 1.0); // set up white clear color
	glClear(GL_COLOR_BUFFER_BIT);   // clear the back ground (white)
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();

	//The sky
	glColor3f(0.0f, 0.7f, 1.0f);
	glRecti(0, 768 / 2, 768, 768);

	//the street
	glColor3f(0.5f, 0.5f, 0.5f);
	glRecti(0, 768 / 4, 768, 768 / 2);

	//the grass
	glColor3f(0.8f, 1.0f, 0.3f);
	glRecti(0, 0, 768, 768 / 4);

	//the sun
	glColor3f(1.0f, 1.0f, 0.0f);
	GLintPoint sunCenter;
	sunCenter.x = 0;
	sunCenter.y = 768;
	filledCircle(sunCenter, 40);

	//the clouds
	glColor3f(1.0f, 1.0f, 1.0f);
	GLintPoint cloudCenter;
	cloudCenter.y = 768 - 80;
	cloudCenter.x = 60;
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			filledCircle(cloudCenter, 30);
			cloudCenter.x += 30;
		}
		cloudCenter.x += 200;
	}

	//the tress
	GLintPoint tree;
	tree.x = 60;
	tree.y = 768 / 2;
	GLintPoint treeTop;
	treeTop.x = tree.x + 10;
	treeTop.y = tree.y + 190;
	GLintPoint treeLeft;
	treeLeft.x = tree.x - 50;
	treeLeft.y = tree.y + 150;
	GLintPoint treeRight;
	treeRight.x = tree.x + 70;
	treeRight.y = tree.y + 150;

	for (int i = 0; i < 4; i++) {
		glColor3f(0.5f, 0.35f, 0.05f); //brown
		glRecti(tree.x, tree.y, tree.x + 20, tree.y + 150);

		glColor3f(0.0, 0.5, 0.0); //dark green
		for (int j = 0; j < 3; j++) {

			triangle(treeLeft.x, treeLeft.y, treeRight.x, treeRight.y, treeTop.x, treeTop.y);
			treeLeft.y += 20;
			treeRight.y += 20;
			treeTop.y += 20;
		}

		treeTop.y = tree.y + 190;
		treeLeft.y = tree.y + 150;
		treeRight.y = tree.y + 150;

		tree.x += 200;
		treeLeft.x += 200;
		treeRight.x += 200;
		treeTop.x += 200;

	}

	//walkSign
	GLintPoint walkSign;
	walkSign.x = 0;
	walkSign.y = (3 * 768) / 8;
	for (int i = 0; i < 6; i++) {
		if (i % 2 == 0) {
			glColor3f(1.0f, 1.0f, 0.0f);
		}
		else {
			glColor3f(1.0f, 1.0f, 1.0f);
		}
		
		glRecti(walkSign.x, walkSign.y, walkSign.x + 120, walkSign.y + 15);
		walkSign.x += 130;
	}

	//sideStreet
	GLintPoint sideStreet;
	sideStreet.x = 0;
	sideStreet.y = 768 / 2;

	for (int i = 0; i < 8; i++) {
		if (i % 2 != 0) {
			glColor3f(1.0f, 1.0f, 1.0f);
		}
		else {
			glColor3f(1.0f, 0.0f, 0.0f);
		}
		glRecti(sideStreet.x, sideStreet.y, sideStreet.x + 96, sideStreet.y + 20);
		glRecti(sideStreet.x, sideStreet.y / 2, sideStreet.x + 96, sideStreet.y / 2 + 20);
		sideStreet.x += 96;

	}

	//pentagon
	glColor3f(0.0, 0.5, 0.0);
	GLintPoint p;
	p.x = 150;
	p.y = 30;
	for (int i = 0; i < 7; i++) {
		pentagon(p.x, p.y, 30);
		p.x += 70;
	}
	

	glPopMatrix();                    // reset model matrix
	glFlush();
}

int main(int argc, char** argv) {

	glClear(GL_COLOR_BUFFER_BIT);
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(768, 700);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("Untitled");
	glutDisplayFunc(myDisplay);

	myInit();
	glutMainLoop();
	return 0;
}