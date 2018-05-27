#include "SimManager.h"
#include <iostream>
#include <string>
#include <sstream>

#include "globalgl.h"

using namespace std;

GLdouble screenWidth = 800;
GLdouble screenHeight = 600;
GLdouble gLeft = -screenWidth / 2;
GLdouble gRight = screenWidth / 2;
GLdouble gBottom = -screenHeight / 2;
GLdouble gTop = screenHeight / 2;
GLdouble gAspect = screenWidth / screenHeight;

GLdouble gScaleFactor = 2.0;
int gUpdateFreq = 1000;

SimulationMgr sm;
ATime simClock;


typedef map<string, int> NameColorMap;
NameColorMap gNCmap;

// void glGetDoublev( GL_CURRENT_COLOR , GLdouble *params);

//  define some colors  (alpha of 1.0 means most intense
enum COLORS { RED = 0, GREEN, BLUE, PURPLE, YELLOW };
const GLdouble colors[][4] = {
	{ 1.0, 0.0, 0.0, 0.0 },
	{ 0.0, 0.0, 1.0, 0.0 },
	{ 1.0, 0.1, 1.0, 0.0 },
	{ 0.8, 0.8, 0.5, 0.0 },
	{ 0.0, 1.0, 1.0, 0.0 },
	{ 1.0, 0.5, 0.5, 0.0 },
	{ 0.5, 1.0, 0.5, 0.0 },
	{ 0.2, 0.3, 0.7, 0.0 },
	{ 0.5, 0.5, 1.0, 0.0 },
	{ 0.05, 0.05, 0.5, 0.0 },
};


const int NUMBCOLORS = sizeof(colors) / sizeof(GLdouble[4]);

void drawString(void* font, string s) {
	string::iterator i;
	for (i = s.begin(); i != s.end(); i++) {
		glutBitmapCharacter(font, *i);
	}
}

void drawKey() {
	NavyMap* nm = sm.getNavy();
	NavyMap::iterator nmi;
	GLdouble x, y;
	x = gLeft;
	y = gTop - 12;
	GLdouble lastcolor[4];
	glGetDoublev(GL_CURRENT_COLOR, lastcolor);
	ostringstream ss;
	string deploystat = "X ";


	ss << simClock;
	glRasterPos3f(x, y, 0);
	drawString(GLUT_BITMAP_HELVETICA_10, ss.str());

	y -= 12;

	for (nmi = nm->begin(); nmi != nm->end(); nmi++) {
		glColor4dv(colors[gNCmap[nmi->first]]);
		glRasterPos3f(x, y, 0);

		if ((nmi->second)->isDeployed()) {
			deploystat = "O ";
		}
		else if ((nmi->second)->wasDeployed()) {
			deploystat = "- ";
		}
		else {
			deploystat = "X ";
		}

		drawString(GLUT_BITMAP_HELVETICA_10, deploystat + nmi->first);

		y -= 12;
	}
	glColor4dv(lastcolor);
}

// this is a wrapper to nanosleep which sleeps for a certain
// number of milliseconds
void delay(int ms) {
#if defined(WIN32)
	Sleep(ms);
#else
	struct timespec ts;
	ts.tv_sec = ms / 1000;
	ts.tv_nsec = 1000000 * (ms % 1000);
	nanosleep(&ts, NULL);
#endif
}


void myInit(void) {
	glClearColor(1.0, 1.0, 1.0, 0.0);       // the background color is white
	glColor3f(0.0f, 0.0f, 0.0f);
	glPointSize(1.0);                    // a 'dot' is 2 by 2 pixels
	glLineWidth(2.0);
}

void getNavyInfo() {
	NavyMap* nm = sm.getNavy();
	NavyMap::iterator nmi;
	HistoryList* hlp;
	HistoryList::iterator hli;
	GLdouble lastcolor[4];
	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glScaled(gScaleFactor, gScaleFactor, gScaleFactor);
	for (nmi = nm->begin(); nmi != nm->end(); nmi++) {
		glGetDoublev(GL_CURRENT_COLOR, lastcolor);
		hlp = (nmi->second)->getHistory();
		if ((nmi->second)->wasDeployed() && !(nmi->second)->isDeployed()) {
			glColor4d(0.8, 0.8, 0.8, 0.0);   // gray
			glEnable(GL_LINE_STIPPLE);
			glLineStipple(1, 0xAAAA);      // dotted line
		}
		else {
			glColor4dv(colors[gNCmap[nmi->first]]);
			glDisable(GL_LINE_STIPPLE);
		}
		glBegin(GL_LINE_STRIP);
		for (hli = hlp->begin(); hli != hlp->end(); hli++) {
			double x, y, z;
			hli->getXYZ(x, y, z);
			glVertex3d(x, y, z / 300.0);     // convert ft to miles (more or less)
			//cout << x << " " << y << " " << z << endl;
		}
		glColor4dv(lastcolor);
		glEnd();
	}
	glPopMatrix();
}

void drawSeas() {
	glColor4f(0.96f, 0.96f, 1.0f, 0.5f);
	glBegin(GL_POLYGON);
	glVertex3d(gLeft, gBottom, 0);
	glVertex3d(gLeft, gTop, 0);
	glVertex3d(gRight, gTop, 0);
	glVertex3d(gRight, gBottom, 0);
	glColor3f(0.0f, 0.0f, 0.0f);
	glEnd();

}

void myDisplay() {
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(gLeft, gRight, gBottom, gTop, -800, 800);
	glClear(GL_COLOR_BUFFER_BIT);


	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	double topWinOffset = 0.15 * screenHeight;    // top window is 15%

	// view from z axis.  This should be scaled to about 10% of window
	glPushMatrix();
	glViewport(0, 0, screenWidth, screenHeight - topWinOffset);
	gluLookAt(0, 0, 100, 0, 0, 0, 0.0, 1.0, 0.0);
	getNavyInfo();
	glPopMatrix();

	// view from y axis to show planes.  Scale to 90% of windows
	glPushMatrix();
	drawKey();
	glViewport(0, screenHeight - topWinOffset, screenWidth, topWinOffset);
	gluLookAt(-0.2, -1.5, 0.4, 0, 0, 0, 0.0, 0.0, 1.0);
	drawSeas();
	getNavyInfo();
	glPopMatrix();

	glutSwapBuffers();
}

void myIdle() {
	if (simClock < sm.getStop()) {
		sm.simDoUpdate(simClock);
		delay(gUpdateFreq);
		glutPostRedisplay();
		simClock += 60;  // add one minute
	}
}

void initColNameMap() {
	NavyMap* nm = sm.getNavy();
	NavyMap::iterator nmi;
	int color = 0;
	for (nmi = nm->begin(); nmi != nm->end(); nmi++) {
		gNCmap[nmi->first] = (color++ % NUMBCOLORS);
	}
}

void doReshape(int w, int h) {
	//cout << "width = " << w << " height = " << h << endl;
	// need to recalulate everything based on width and height
	screenHeight = h;
	screenWidth = w;
	gLeft = -screenWidth / 2;
	gRight = screenWidth / 2;
	gBottom = -screenHeight / 2;
	gTop = screenHeight / 2;
	gAspect = screenWidth / screenHeight;
	//cout << "left = " << gLeft << " right = " << gRight << endl;
	//cout << "bottom = " << gBottom << " top = " << gTop << endl;
	glutPostRedisplay();
}

void doKeyboard(unsigned char key, int x, int y) {
	switch (key) {
	case '\033':
	case 'q':
		exit(0);
		break;
	case 'f':
		glutFullScreen();
		break;
	}
	glutPostRedisplay();
}

void doSpecialKeys(int key, int x, int y) {
	switch (key) {
	case GLUT_KEY_UP:
		gScaleFactor += 0.2;
		break;
	case GLUT_KEY_DOWN:
		gScaleFactor -= 0.2;
		break;
	default:
		break;
	}
	glutPostRedisplay();
}


int
main(int ac, char** av) {
	glutInit(&ac, av);          // initialize the toolkit
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(screenWidth, screenHeight);
	glutInitWindowPosition(0, 0);   // initial window position
	glutCreateWindow("Navy Theatre");
	//glutFullScreen();
	glutReshapeFunc(doReshape);         // called on resize
	glutKeyboardFunc(doKeyboard);       // keyboard handler
	glutSpecialFunc(doSpecialKeys);
	//glutMouseFunc(myMouse);             // called when mouse button pushed
	glutIdleFunc(myIdle);             // called when mouse button pushed
	glutDisplayFunc(myDisplay);     // register the redraw function
	myInit();                       // your initialization

	if (ac < 2) {
		cout << "Usage: " << av[0] << " <orders>" << endl;
		return 1;
	}
	if (sm.simInit(av[1])) {
		sm.printOrders();
		sm.printNavy();
		simClock = sm.getStart();
	}
	else {
		cout << "Init failed" << endl;
		return 1;
	}

	if (ac == 3) {
		gUpdateFreq = 100;
	}

	initColNameMap();
	glutMainLoop();                 // go into a perpetual loop
	return 0;       // you can't get here, but compiler does not know 
}
