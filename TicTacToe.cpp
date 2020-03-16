#include <windows.h>
#include <gl/Gl.h>
#include <gl/glut.h>  //OpenGL Utility Toolkit for creating openGL window
#include <iostream> 
#include <vector> 
#include <chrono>
#include <thread>
#include <cmath>
#include <numeric>
#include <algorithm> 
#include <array>

using namespace std;
using std::vector;


class GLintPoint {
public:
	GLint x, y;
};

void voidCircle(GLintPoint center, double radius) {
	float x1, y1, x2, y2;
	float angle;

	glColor3f(0.0f, 0.0f, 0.0f);

	glBegin(GL_LINE_STRIP);
	for (angle = 1.0f; angle < 361.0f; angle += 0.2) {
		x2 = center.x + sin(angle) * radius;
		y2 = center.y + cos(angle) * radius;
		glVertex2f(x2, y2);
	}
	glEnd();
}

short currPlayer = 0;   // -1 : O, 1 : X

class player {
public:
	short wins =0;
	array<GLfloat, 3> backgroundColor = { 0.5f, 0.5f, 0.5f };
	GLintPoint center;
	array<GLintPoint, 2> backgroundBorder;

	
};

void drawShape(GLintPoint center, bool isO) {
	if (isO) {
		voidCircle(center, 40);
	}
	else {
		glBegin(GL_LINES);
		glVertex2i(center.x - 40, center.y - 40);
		glVertex2i(center.x + 40, center.y + 40);
		glVertex2i(center.x + 40, center.y - 40);
		glVertex2i(center.x - 40, center.y + 40);
		glEnd();
	}
}

int screenHeight = 640;
int screenWidth = 640;

GLintPoint cVertex;
unsigned char gTheKey;
bool isStarted = true;
bool showResult = false;
char record[3][3];
vector<GLintPoint> centers;

player xPlayer;
player oPlayer;

void myInit(void) {
	glClearColor(1.0, 1.0, 1.0, 0.0);
	glColor3f(0.0f, 0.0f, 0.0f);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0.0, screenWidth, 0.0, screenHeight);
}

void initCenters() {
	int count = 0;
	for (GLint y = 524; y > 140; y -= 128)
	{
		for (GLint x = 160 ; x < 544; x += 128)
		{
			centers.push_back({ x + 64, y - 64 });
		}
	}
	
}

void initPlayground() {
	glColor3f(1.0f, 1.0f, 1.0f);
	glRecti(140, 160, 524, 544);
	glColor3f(0.0f, 0.0f, 0.0f);
	for (GLint x = 140; x < 524; x += 128) {
		for (GLint y = 160; y < 544; y += 128) {
			glBegin(GL_LINE_LOOP);
			glVertex2i(x, y + 128);
			glVertex2i(x + 128, y + 128);
			glVertex2i(x + 128, y);
			glVertex2i(x, y);
			glEnd();
		}
	}

	//init X, O objects
	oPlayer.center = { 180,100 };
	oPlayer.backgroundBorder[0] = { 140, 60 };
	oPlayer.backgroundBorder[1] = { 220, 140 };
	xPlayer.backgroundBorder[0] = { 524, 60 };
	xPlayer.backgroundBorder[1] = { 444, 140 };

	glColor3f(oPlayer.backgroundColor[0], oPlayer.backgroundColor[1], oPlayer.backgroundColor[2]);
	glRecti(oPlayer.backgroundBorder[0].x, oPlayer.backgroundBorder[0].y, oPlayer.backgroundBorder[1].x, oPlayer.backgroundBorder[1].y);  //0

	glColor3f(xPlayer.backgroundColor[0], xPlayer.backgroundColor[1], xPlayer.backgroundColor[2]);
	glRecti(xPlayer.backgroundBorder[0].x, xPlayer.backgroundBorder[0].y, xPlayer.backgroundBorder[1].x, xPlayer.backgroundBorder[1].y);  //x

	voidCircle({ oPlayer.center }, 35);

	glBegin(GL_LINES);
	glVertex2i(524, 60);
	glVertex2i(444, 140);
	glVertex2i(524, 140);
	glVertex2i(444, 60);
	glEnd();
}

void drawScore() {
	//the result boxes
	glColor3f(1.0f, 1.0f, 1.0f);
	glRecti(140, 20, 220, 40); //o
	glRecti(524, 20, 444, 40); //x

	glColor3f(0.0f, 0.0f, 0.0f);
	int ylen = 15;
	int xlen = -3;

	for (int i = 0; i < oPlayer.wins; i++) {
		glBegin(GL_LINES);
		glVertex2i((i * xlen) + 215, 23 + ylen);
		glVertex2i((i * xlen) + 215, 23);
		glEnd();
	}

	for (int i = 0; i < xPlayer.wins; i++) {
		glBegin(GL_LINES);
		glVertex2i((i * xlen) + 520, 23 + ylen);
		glVertex2i((i * xlen) + 520, 23);
		glEnd();
	}
	
}

double distanceBetweenTwoPoints(GLint x, GLint y, GLint a, GLint b) {
	return sqrt(pow(x - a, 2) + pow(y - b, 2));
}

bool checkSum(int sum) {
	// if a player wins, inc his score, reset the game, start the new game with his turn
	if (sum == 3) {  // X wins
		xPlayer.wins += 1;
		memset(record, 0, sizeof(record));
		showResult = true;
		return true;
	}
	else if (sum == -3) {  // O wins
		oPlayer.wins += 1;
		memset(record, 0, sizeof(record));
		showResult = true;
		return true;
	}
	return false;
}

bool CheckWinning() {
	int sum = 0;
	bool isWon = false;
	//check rows
	for (int i = 0; i < 3; i++) {
		sum = accumulate(record[i], record[i] + 3, sum);
		if (checkSum(sum)) {
			return true;
		}
		sum = 0;
	}

	//check colums
	for (int j = 0; j < 3; j++) {
		for (int i = 0; i < 3; i++) {
			sum += record[i][j];
		}
		if (checkSum(sum)) {
			return true;
		}
		sum = 0;
	}

	//check diagonals
	sum = record[0][0] + record[1][1] + record[2][2];
	if (checkSum(sum)) {
		return true;
	}

	sum = record[0][2] + record[1][1] + record[2][0];
	if (checkSum(sum)) {
		return true;
	}
	return false;
}

void play() {
	//is it in the playground bounds
	if ((cVertex.x > 140) && (cVertex.y > 160) && (cVertex.x < 524) && (cVertex.y < 544)) {
		//check the nearest
		GLintPoint center = { 0,0 };
		float j = 0.33333;
		int row=0, col=0;
		double dist = 1000.0;

		for (std::vector<GLintPoint>::size_type i = 0; i != centers.size(); i++) {
			double temp = distanceBetweenTwoPoints(centers[i].x, centers[i].y, cVertex.x, cVertex.y);
			if (dist > temp) {
				dist = temp;
				center = { centers[i].x, centers[i].y };
				row = floor(j);
				col = i % 3;
			}
			j += 0.33333;
		}

		if (currPlayer == 1) {
			record[row][col] = 1;
			
			if (CheckWinning()) {
				return;
			}

			oPlayer.backgroundColor = { 1.0f, 1.0f, 1.0f };
			xPlayer.backgroundColor = { 0.5f, 0.5f, 0.5f }; //white background to gray
			currPlayer = -1;

		}
		else if (currPlayer == -1) {
			record[row][col] = -1;

			if (CheckWinning()) {
				return;
			}

			oPlayer.backgroundColor = { 0.5f, 0.5f, 0.5f };
			xPlayer.backgroundColor = { 1.0f, 1.0f, 1.0f };
			currPlayer = 1;
		}

	}
}

void updatePosition(int button, int state, int x, int y) {
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN) {
		cVertex.x = x;
		cVertex.y = screenHeight - y;		// flip y coordinate

		//check if it isStarted && one of the two button clicked
		if (isStarted) {
			if ( (cVertex.x < 524) && (cVertex.y > 60) && (cVertex.x > 444) && (cVertex.y < 140) ) {   // X
				xPlayer.backgroundColor = { 1.0, 1.0 ,1.0 };
				currPlayer = 1;
				isStarted = false;
			} else if ( (cVertex.x > 140) && (cVertex.y > 60) && (cVertex.x < 220) && (cVertex.y < 140) ) {  // O
				oPlayer.backgroundColor = { 1.0, 1.0 ,1.0 };
				currPlayer = -1;
				isStarted = false;
			}
			return;
		}
		play();
	}
}

void plantPlayground() {
	int k = 0;
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			if (record[i][j] == 1) {
				drawShape(centers[k], false);
			}
			else if (record[i][j] == -1) {
				drawShape(centers[k], true);
			}
			k++;
		}
	}
}

void myDisplay(void) {
	glClearColor(0.5, 0.5, 0.5, 1.0); // set up white clear color
	glClear(GL_COLOR_BUFFER_BIT);   // clear the back ground (white)
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();

	initPlayground();

	initCenters();

	plantPlayground();

	if (showResult) {
		drawScore();
	}

	glPopMatrix();
	glFlush();

}

void myKeyboard(unsigned char theKey, int mouseX, int mouseY) {

	switch (theKey) {
	case 'n': //restart , keep the score
		memset(record, 0, sizeof(record));
		myDisplay();
		break;
	case 'r':  //restart, reset the score
		memset(record, 0, sizeof(record));
		xPlayer.wins = 0;
		oPlayer.wins = 0;
		showResult = false;
		isStarted = true;
		oPlayer.backgroundColor = { 0.5f, 0.5f, 0.5f };
		xPlayer.backgroundColor = { 0.5f, 0.5f, 0.5f };
		myDisplay();
		break;

	default:
		break;
	}
}

int main(int argc, char** argv)
{
	glClear(GL_COLOR_BUFFER_BIT);
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(screenWidth, screenHeight);
	glutInitWindowPosition(0, 0);
	glutCreateWindow("Painter");
	glutDisplayFunc(myDisplay);
	glutKeyboardFunc(myKeyboard);     //register keyboard func
	glutMouseFunc(updatePosition);     //register mouse func
	myInit();
	glutMainLoop();
	return 0;
}