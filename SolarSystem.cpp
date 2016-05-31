/* Dan Gray
cs4410 HW #4
11-20-14 */

#include "Windows.h"
#include <gl/GL.h>
#include <GL/glut.h>
#include "RGBpixmap.h"
#include <cstring>
#include <iostream>
using namespace std;


double rotationalAngle = 0.0;		//Rotational angle
double mercuryOrbit = 0.0;		//Orbit angle around sun
double venusOrbit = 0.0;		//for each planet
double earthOrbit = 0.0;
double marsOrbit = 0.0;
double jupiterOrbit = 0.0;
double saturnOrbit = 0.0;
double uranusOrbit = 0.0;
double neptuneOrbit = 0.0;
double plutoOrbit = 0.0;
void ngon(int n, float cx, float cy, float radius, float stAngle);

double horizAngle = 0.0;	//Viewing angle
double vertAngle = 0.0;	//Viewing angle
double zoomFactor = 6.25;	//Zooming distance glLookat cam z

double camXFactor = 0.0;	//glLookat cam x
double centerXFactor = 0.0; //glLookat center x
bool moveMode = false;	//For orbitting
bool singlePlanetMode = false;
bool zoomInFlag = false;
bool zoomOutFlag = false;
 
//For texturing
RGBpixmap pix[13];
GLUquadric *quad;

//For clicking planets
#define BUFSIZE 512
GLuint selectBuf[BUFSIZE];
#define RENDER					1
#define SELECT					2
int mode = RENDER;
GLint hits;
int cursorX,cursorY; 

void stopPicking();
void processHits (GLint hits, GLuint buffer[]);

//For writing to the screen
void drawString(char *string);
void beginText();
void endText();
char planetName[8] = {0};



 void drawSun(){
	glPushMatrix();
	glBindTexture(GL_TEXTURE_2D, 0);
	glTranslated(0.0, 0.0, 0.0);
	gluSphere(quad, .2, 20, 20);
	glPopMatrix();
}

void drawMercury(){
	glPushMatrix();
	glBindTexture(GL_TEXTURE_2D, 1);
	glTranslated(0.25, 0.0, 0.0);
	gluSphere(quad, .01, 20, 20);
	glPopMatrix();
}

void drawVenus(){
	glPushMatrix();
	glBindTexture(GL_TEXTURE_2D, 2);
	glTranslated(0.5, 0.0, 0.0);
	gluSphere(quad, .018, 20, 20);
	glPopMatrix();
}

void drawEarth(){
	glPushMatrix();
	glBindTexture(GL_TEXTURE_2D, 3);
	glTranslated(0.75, 0.0, 0.0);
	gluSphere(quad, .02, 20, 20);
	glPopMatrix();
}

void drawMoon(){
	glPushMatrix();
	glBindTexture(GL_TEXTURE_2D, 12);
	glTranslated(0.8, 0.0, 0.0);
	gluSphere(quad, .005, 20, 20);
	glPopMatrix();
}

void drawMars(){
	glPushMatrix();
	glBindTexture(GL_TEXTURE_2D, 4);
	glTranslated(1.0, 0.0, 0.0);
	gluSphere(quad, .013, 20, 20);
	glPopMatrix();
}

void drawJupiter(){
	glPushMatrix();
	glBindTexture(GL_TEXTURE_2D, 5);
	glTranslated(1.25, 0.0, 0.0);
	gluSphere(quad, .1, 20, 20);
	glPopMatrix();
}

void drawSaturn(){
	glPushMatrix();
	glBindTexture(GL_TEXTURE_2D, 6);
	glTranslated(1.5, 0.0, 0.0);
	gluSphere(quad, .08, 20, 20);
	glPopMatrix();
}

void drawSaturnRing(){
	glPushMatrix();
	glBindTexture(GL_TEXTURE_2D, 10);
	glTranslated(1.5, 0.0, 0.0);
	//ngon(int n, float cx, float cy, float radius, float stAngle)
	ngon(40, 0.0, 0.0, 0.125, 0.0);
	glPopMatrix();
}

void drawUranus(){
	glPushMatrix();
	glBindTexture(GL_TEXTURE_2D, 7);
	glTranslated(1.75, 0.0, 0.0);
	gluSphere(quad, .045, 20, 20);
	glPopMatrix();
}

void drawNeptune(){
	glPushMatrix();
	glBindTexture(GL_TEXTURE_2D, 8);
	glTranslated(2.0, 0.0, 0.0);
	gluSphere(quad, .045, 20, 20);
	glPopMatrix();
}

void drawPluto(){
	glPushMatrix();
	glBindTexture(GL_TEXTURE_2D, 9);
	glTranslated(2.25, 0.0, 0.0);
	gluSphere(quad, .012, 20, 20);
	glPopMatrix();
}


void myInit(){
	quad = gluNewQuadric();
	gluQuadricTexture(quad, GL_TRUE);
    gluQuadricNormals(quad, GLU_SMOOTH);


	pix[0].readBMPFile("sunmap.bmp");
	pix[0].setTexture(0);
	pix[1].readBMPFile("mercurymap.bmp");
	pix[1].setTexture(1);
	pix[2].readBMPFile("venusmap.bmp");
	pix[2].setTexture(2);
	pix[3].readBMPFile("earthmap.bmp");
	pix[3].setTexture(3);
	pix[4].readBMPFile("marsmap.bmp");
	pix[4].setTexture(4);
	pix[5].readBMPFile("jupitermap.bmp");
	pix[5].setTexture(5);
	pix[6].readBMPFile("saturnmap.bmp");
	pix[6].setTexture(6);
	pix[7].readBMPFile("uranusmap.bmp");
	pix[7].setTexture(7);
	pix[8].readBMPFile("neptunemap.bmp");
	pix[8].setTexture(8);
	pix[9].readBMPFile("plutomap.bmp");
	pix[9].setTexture(9);
	pix[10].readBMPFile("saturnringcolor.bmp");
	pix[10].setTexture(10);
	pix[12].readBMPFile("moonMap.bmp");
	pix[12].setTexture(12);

	//set properties of the surface material
	GLfloat mat_ambient[] = { 0.0f, 0.0f, 1.0f, 1.0f}; // 
	GLfloat mat_diffuse[] = {0.6f, 0.6f, 0.6f, 1.0f};
	GLfloat mat_specular[] = {1.0f, 1.0f, 1.0f, 1.0f};
	GLfloat mat_shininess[] = {50.0f};
	glMaterialfv(GL_FRONT,GL_AMBIENT,mat_ambient);
	glMaterialfv(GL_FRONT,GL_DIFFUSE,mat_diffuse);
	glMaterialfv(GL_FRONT,GL_SPECULAR,mat_specular);
	glMaterialfv(GL_FRONT,GL_SHININESS,mat_shininess);

	GLfloat lightIntensity[] = {1.0f, 1.0f, 1.0f, 1.0f};
	GLfloat light_position[] = {0.0f, 0.0f, 0.0f, 0.0f};
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightIntensity);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(40, 80/64, 0.01, 10);
}

//Creates the Orbit paths
void ngonO(int n, float cx, float cy, float radius, float stAngle){
	float curx, cury;

	if(n<3) return;
	double angle = stAngle * 3.14159265 / 180; // initial angle in radians
	double angleInc = 2 * 3.14159265 /n; //angle increment

	glBegin(GL_LINE_LOOP); //closed polyline
	for(int k = 0; k < n; k++){  //repeat n times
		curx = cx + radius * cos(angle);
		cury = cy + radius * sin(angle);
		glVertex2f(curx, cury);
		angle += angleInc;
	}
	glEnd();
}

//CREATES SATURNS RING
void ngon(int n, float cx, float cy, float radius, float stAngle){
	float curx, cury;

	if(n<3) return;
	double angle = stAngle * 3.14159265 / 180; // initial angle in radians
	double angleInc = 2 * 3.14159265 /n; //angle increment

	glBegin(GL_TRIANGLE_STRIP); //closed polyline
	for(int k = 0; k <= n; k++){  //repeat n times
		curx = cx + radius * cos(angle);
		cury = cy + radius * sin(angle);
		glVertex2f(curx, cury);

		curx = cx + (radius*.75) * cos(angle);
		cury = cy + (radius*.75) * sin(angle);
		glVertex2f(curx, cury);
		angle += angleInc;
	}
	glEnd();
}

//If nothing else happening, do orbiting motion
void Move (void)
{
	//Planet orbit speed factors made scientifically correct
	//relative to their year vs 1 earth year
	static float mef = 0.207555;	//Mercury
	static float vf = 0.0811688;	//Venus
	static float ef = 0.05;			//Earth
	static float maf = 0.02631;		//Mars
	static float jf = 0.004166;		//Jupiter
	static float sf = 0.001724;		//Saturn
	static float uf = .0005952;		//Uranus
	static float nf = 0.0003030;	//Neptune
	static float pf = 0.0002016;	//Pluto

	if(moveMode == true){
		rotationalAngle += maf;//Each planet's spin

		mercuryOrbit += mef;
		venusOrbit += vf;
		earthOrbit += ef;
		marsOrbit += maf;
		jupiterOrbit += jf;
		saturnOrbit += sf;
		uranusOrbit += uf;
		neptuneOrbit += nf;
		plutoOrbit += pf;
	}

	//Zoom In/Out from mouse button
	if(zoomInFlag == true){
		zoomFactor -= 0.0025;
	}
	if(zoomOutFlag == true){
		zoomFactor += 0.0025;
	}
	
	if(singlePlanetMode == true){
		rotationalAngle += maf;//Each planet's spin
	}

	glutPostRedisplay ();
}


void myDisplay(void){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glTexEnvf(GL_TEXTURE_ENV,GL_TEXTURE_ENV_MODE,GL_DECAL);	
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	//eye, center, up
	gluLookAt(camXFactor, 0.0, zoomFactor, centerXFactor, 0.0, 0.0, 0.0, 1.0, 0.0);  //zoom in/out start at 7.0


	if(mode == SELECT){
		GLint viewport[4]; // The Size Of The Viewport. [0] Is <x>, [1] Is <y>, [2] Is <length>, [3] Is <width>

		glSelectBuffer(BUFSIZE,selectBuf);
		glRenderMode(GL_SELECT);

		glMatrixMode(GL_PROJECTION); //to redefine viewing volume so it renders small area around place where mouse was clicked
		glPushMatrix();   //save normal rendering mode settings
		glLoadIdentity();

		glGetIntegerv(GL_VIEWPORT,viewport); //get current viewport
		//(cursor loc x, cursor loc y, width picking region, height picking region, viewport)
		gluPickMatrix(cursorX,viewport[3]-cursorY,5,5,viewport); //define viewing volume so rendering done in small area cursor
		gluPerspective(40, 80/64, 0.01, 20);
		glMatrixMode(GL_MODELVIEW);
		glInitNames(); //Initialize name stack for rendering
	}
	
	//Display Planets
	glPushMatrix();
	glPushName(99);
	glRotated(vertAngle, 1.0, 0.0, 0.0f);
	glRotated(horizAngle, 0.0, 1.0, 0.0f);
	ngonO(50, 0.0, 0.0, 0.25, 0);
	ngonO(50, 0.0, 0.0, 0.5, 0);
	ngonO(50, 0.0, 0.0, 0.75, 0);
	ngonO(50, 0.0, 0.0, 1.0, 0);
	ngonO(50, 0.0, 0.0, 1.25, 0);
	ngonO(50, 0.0, 0.0, 1.5, 0);
	ngonO(50, 0.0, 0.0, 1.75, 0);
	ngonO(50, 0.0, 0.0, 2.0, 0);
	ngonO(50, 0.0, 0.0, 2.25, 0);
	glPopName();
	glPopMatrix();

	//Sun
	glPushMatrix();
	glPushName(0);
	glRotated(vertAngle, 1.0, 0.0, 0.0f);
	glRotated(horizAngle, 0.0, 1.0, 0.0f);
	glRotated(rotationalAngle, 0.0, 0.0, 1.0);
	drawSun();
	glPopName();
	glPopMatrix();
	
	//Mercury Position
	glPushMatrix();
	glPushName(1);
	glRotated(vertAngle, 1.0, 0.0, 0.0f);
	glRotated(horizAngle, 0.0, 1.0, 0.0f);
	glRotated(mercuryOrbit , 0.0, 0.0, 1.0);
	glTranslated(0.25, 0.0, 0.0);
	glRotated(rotationalAngle, 0.0, 0.0, 1.0);
	glTranslated(-0.25, 0.0, 0.0);
	drawMercury();
	glPopName();
	glPopMatrix();

	//Venus Position
	glPushMatrix();
	glPushName(2);
	glRotated(vertAngle, 1.0, 0.0, 0.0f);
	glRotated(horizAngle, 0.0, 1.0, 0.0f);
	glRotated(venusOrbit , 0.0, 0.0, 1.0);
	glTranslated(0.5, 0.0, 0.0);
	glRotated(rotationalAngle, 0.0, 0.0, 1.0);
	glTranslated(-0.5, 0.0, 0.0);
	drawVenus();
	glPopName();
	glPopMatrix();

	//Earth Position
	glPushMatrix();
	glPushName(3);
	glRotated(vertAngle, 1.0, 0.0, 0.0f);
	glRotated(horizAngle, 0.0, 1.0, 0.0f);
	glRotated(earthOrbit , 0.0, 0.0, 1.0);
	glTranslated(0.75, 0.0, 0.0);
	glRotated(rotationalAngle, 0.0, 0.0, 1.0);
	glTranslated(-0.75, 0.0, 0.0);
	drawEarth();
	glPopName();
	glPopMatrix();

	//Moon
	glPushMatrix();
	glPushName(3);
	glRotated(vertAngle, 1.0, 0.0, 0.0f);
	glRotated(horizAngle, 0.0, 1.0, 0.0f);
	glRotated(earthOrbit , 0.0, 0.0, 1.0);
	glTranslated(0.75, 0.0, 0.0);
	glRotated(rotationalAngle, 0.0, 0.0, 1.0);
	glTranslated(-0.75, 0.0, 0.0);
	drawMoon();
	glPopName();
	glPopMatrix();

	//Mars Position
	glPushMatrix();
	glPushName(4);
	glRotated(vertAngle, 1.0, 0.0, 0.0f);
	glRotated(horizAngle, 0.0, 1.0, 0.0f);
	glRotated(marsOrbit , 0.0, 0.0, 1.0);
	glTranslated(1.0, 0.0, 0.0);
	glRotated(rotationalAngle, 0.0, 0.0, 1.0);
	glTranslated(-1.0, 0.0, 0.0);
	drawMars();
	glPopName();
	glPopMatrix();

	//Jupiter Position
	glPushMatrix();
	glPushName(5);
	glRotated(vertAngle, 1.0, 0.0, 0.0f);
	glRotated(horizAngle, 0.0, 1.0, 0.0f);
	glRotated(jupiterOrbit , 0.0, 0.0, 1.0);
	glTranslated(1.25, 0.0, 0.0);
	glRotated(rotationalAngle, 0.0, 0.0, 1.0);
	glTranslated(-1.25, 0.0, 0.0);
	drawJupiter();
	glPopName();
	glPopMatrix();

	//Saturn Position
	glPushMatrix();
	glPushName(6);
	glRotated(vertAngle, 1.0, 0.0, 0.0f);
	glRotated(horizAngle, 0.0, 1.0, 0.0f);
	glRotated(saturnOrbit , 0.0, 0.0, 1.0);
	drawSaturnRing();
	glPopName();
	glPopMatrix();

	glPushMatrix();
	glPushName(6);
	glRotated(vertAngle, 1.0, 0.0, 0.0f);
	glRotated(horizAngle, 0.0, 1.0, 0.0f);
	glRotated(saturnOrbit , 0.0, 0.0, 1.0);
	glTranslated(1.5, 0.0, 0.0);
	glRotated(rotationalAngle, 0.0, 0.0, 1.0);
	glTranslated(-1.5, 0.0, 0.0);
	drawSaturn();
	glPopName();
	glPopMatrix();

	//Uranus Position
	glPushMatrix();
	glPushName(7);
	glRotated(vertAngle, 1.0, 0.0, 0.0f);
	glRotated(horizAngle, 0.0, 1.0, 0.0f);
	glRotated(uranusOrbit , 0.0, 0.0, 1.0);
	glTranslated(1.75, 0.0, 0.0);
	glRotated(rotationalAngle, 0.0, 0.0, 1.0);
	glTranslated(-1.75, 0.0, 0.0);
	drawUranus();
	glPopName();
	glPopMatrix();

	//Neptune Position
	glPushMatrix();
	glPushName(8);
	glRotated(vertAngle, 1.0, 0.0, 0.0f);
	glRotated(horizAngle, 0.0, 1.0, 0.0f);
	glRotated(neptuneOrbit , 0.0, 0.0, 1.0);
	glTranslated(2.0, 0.0, 0.0);
	glRotated(rotationalAngle, 0.0, 0.0, 1.0);
	glTranslated(-2.0, 0.0, 0.0);
	drawNeptune();
	glPopName();
	glPopMatrix();

	//Pluto Position
	glPushMatrix();
	glPushName(9);
	glRotated(vertAngle, 1.0, 0.0, 0.0f);
	glRotated(horizAngle, 0.0, 1.0, 0.0f);
	glRotated(plutoOrbit , 0.0, 0.0, 1.0);
	glTranslated(2.25, 0.0, 0.0);
	glRotated(rotationalAngle, 0.0, 0.0, 1.0);
	glTranslated(-2.25, 0.0, 0.0);
	drawPluto();
	glPopName();
	glPopMatrix();

	if(singlePlanetMode == true){

	beginText();
	glColor3f(1, 1, 1);
	drawString(planetName);
	endText();

	//Display Planets
	glPushMatrix();
	glPushName(0);
	glRotated(vertAngle, 1.0, 0.0, 0.0f);
	glRotated(horizAngle, 0.0, 1.0, 0.0f);
	glRotated(rotationalAngle, 0.0, 0.0, 1.0);
	drawSun();
	glPopName();
	glPopMatrix();
	
	//Mercury Position
	glPushMatrix();
	glPushName(1);
	glRotated(vertAngle, 1.0, 0.0, 0.0f);
	glRotated(horizAngle, 0.0, 1.0, 0.0f);
	glRotated(mercuryOrbit , 0.0, 0.0, 1.0);
	glTranslated(0.25, 0.0, 0.0);
	glRotated(rotationalAngle, 0.0, 0.0, 1.0);
	glTranslated(-0.25, 0.0, 0.0);
	drawMercury();
	glPopName();
	glPopMatrix();

	//Venus Position
	glPushMatrix();
	glPushName(2);
	glRotated(vertAngle, 1.0, 0.0, 0.0f);
	glRotated(horizAngle, 0.0, 1.0, 0.0f);
	glRotated(venusOrbit , 0.0, 0.0, 1.0);
	glTranslated(0.5, 0.0, 0.0);
	glRotated(rotationalAngle, 0.0, 0.0, 1.0);
	glTranslated(-0.5, 0.0, 0.0);
	drawVenus();
	glPopName();
	glPopMatrix();

	//Earth Position
	glPushMatrix();
	glPushName(3);
	glRotated(vertAngle, 1.0, 0.0, 0.0f);
	glRotated(horizAngle, 0.0, 1.0, 0.0f);
	glRotated(earthOrbit , 0.0, 0.0, 1.0);
	glTranslated(0.75, 0.0, 0.0);
	glRotated(rotationalAngle, 0.0, 0.0, 1.0);
	glTranslated(-0.75, 0.0, 0.0);
	drawEarth();
	glPopName();
	glPopMatrix();

	//Moon
	glPushMatrix();
	glPushName(3);
	glRotated(vertAngle, 1.0, 0.0, 0.0f);
	glRotated(horizAngle, 0.0, 1.0, 0.0f);
	glRotated(earthOrbit , 0.0, 0.0, 1.0);
	glTranslated(0.75, 0.0, 0.0);
	glRotated(rotationalAngle, 0.0, 0.0, 1.0);
	glTranslated(-0.75, 0.0, 0.0);
	drawMoon();
	glPopName();
	glPopMatrix();

	//Mars Position
	glPushMatrix();
	glPushName(4);
	glRotated(vertAngle, 1.0, 0.0, 0.0f);
	glRotated(horizAngle, 0.0, 1.0, 0.0f);
	glRotated(marsOrbit , 0.0, 0.0, 1.0);
	glTranslated(1.0, 0.0, 0.0);
	glRotated(rotationalAngle, 0.0, 0.0, 1.0);
	glTranslated(-1.0, 0.0, 0.0);
	drawMars();
	glPopName();
	glPopMatrix();

	//Jupiter Position
	glPushMatrix();
	glPushName(5);
	glRotated(vertAngle, 1.0, 0.0, 0.0f);
	glRotated(horizAngle, 0.0, 1.0, 0.0f);
	glRotated(jupiterOrbit , 0.0, 0.0, 1.0);
	glTranslated(1.25, 0.0, 0.0);
	glRotated(rotationalAngle, 0.0, 0.0, 1.0);
	glTranslated(-1.25, 0.0, 0.0);
	drawJupiter();
	glPopName();
	glPopMatrix();

	//Saturn Position
	glPushMatrix();
	glPushName(6);
	glRotated(vertAngle, 1.0, 0.0, 0.0f);
	glRotated(horizAngle, 0.0, 1.0, 0.0f);
	glRotated(saturnOrbit , 0.0, 0.0, 1.0);
	glRotated(90 , 0.0, 0.0, 1.0);
	drawSaturnRing();
	glPopName();
	glPopMatrix();

	glPushMatrix();
	glPushName(6);
	glRotated(vertAngle, 1.0, 0.0, 0.0f);
	glRotated(horizAngle, 0.0, 1.0, 0.0f);
	glRotated(saturnOrbit , 0.0, 0.0, 1.0);
	glTranslated(1.5, 0.0, 0.0);
	glRotated(rotationalAngle, 0.0, 0.0, 1.0);
	glTranslated(-1.5, 0.0, 0.0);
	drawSaturn();
	glPopName();
	glPopMatrix();

	//Uranus Position
	glPushMatrix();
	glPushName(7);
	glRotated(vertAngle, 1.0, 0.0, 0.0f);
	glRotated(horizAngle, 0.0, 1.0, 0.0f);
	glRotated(uranusOrbit , 0.0, 0.0, 1.0);
	glTranslated(1.75, 0.0, 0.0);
	glRotated(rotationalAngle, 0.0, 0.0, 1.0);
	glTranslated(-1.75, 0.0, 0.0);
	drawUranus();
	glPopName();
	glPopMatrix();

	//Neptune Position
	glPushMatrix();
	glPushName(8);
	glRotated(vertAngle, 1.0, 0.0, 0.0f);
	glRotated(horizAngle, 0.0, 1.0, 0.0f);
	glRotated(neptuneOrbit , 0.0, 0.0, 1.0);
	glTranslated(2.0, 0.0, 0.0);
	glRotated(rotationalAngle, 0.0, 0.0, 1.0);
	glTranslated(-2.0, 0.0, 0.0);
	drawNeptune();
	glPopName();
	glPopMatrix();

	//Pluto Position
	glPushMatrix();
	glPushName(9);
	glRotated(vertAngle, 1.0, 0.0, 0.0f);
	glRotated(horizAngle, 0.0, 1.0, 0.0f);
	glRotated(plutoOrbit , 0.0, 0.0, 1.0);
	glTranslated(2.25, 0.0, 0.0);
	glRotated(rotationalAngle, 0.0, 0.0, 1.0);
	glTranslated(-2.25, 0.0, 0.0);
	drawPluto();
	glPopName();
	glPopMatrix();
	}
	
	if(mode == SELECT){
		stopPicking();
	}

	/*Each new picture is drawn in the invisible buffer, and when the drawing is
	complete the display switches to it.*/
	glutSwapBuffers();
}

void myMouse(int button, int state, int mouseX, int mouseY){

	if(state == GLUT_DOWN){
		if(button == GLUT_LEFT_BUTTON){
			zoomInFlag = true;
			mode = SELECT;
		}
		if(button ==GLUT_RIGHT_BUTTON){
			zoomOutFlag = true;
		}
	}
	else if(state == GLUT_UP){
		if(button == GLUT_LEFT_BUTTON){
			zoomInFlag = false;
		}
		if(button == GLUT_RIGHT_BUTTON){
			zoomOutFlag = false;
		}
	}

	cursorX = mouseX;
	cursorY = mouseY;

}

void stopPicking() {
	int hits;
	
	// restoring the original projection matrix
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
	glMatrixMode(GL_MODELVIEW);
	glFlush();	

	hits = glRenderMode(GL_RENDER); //returning to normal rendering mode
	
	// if there are hits process them
	if (hits != 0){
		processHits(hits,selectBuf);
	}
	mode = RENDER;
}


void processHits (GLint hits, GLuint buffer[])
{
   unsigned int i;
   GLuint names, *ptr, minZ,*ptrNames, numberOfNames;   

   ptr = (GLuint *) buffer;

   minZ = 0xffffffff;
   for (i = 0; i < hits; i++) {	
      names = *ptr;
	  ptr++;
	  if (*ptr < minZ) {
		  numberOfNames = names;
		  minZ = *ptr;
		  ptrNames = ptr+2;
	  }	  
	  ptr += names+2;
	}

	if(*ptrNames >= 0){
		mercuryOrbit = 0.0;
		venusOrbit = 0.0;
		earthOrbit = 0.0;
		marsOrbit = 0.0;
		jupiterOrbit = 0.0;
		saturnOrbit = 0.0;
		uranusOrbit = 0.0;
		neptuneOrbit = 0.0;
		plutoOrbit = 0.0;
		moveMode = false;
	}

	if(*ptrNames == 0){
		singlePlanetMode = true;
		zoomFactor = 1.0;
		vertAngle = 90;
		strcpy(planetName, "Sun");
	}
	else if(*ptrNames == 1){
		singlePlanetMode = true;
		zoomFactor = 0.2;
		camXFactor = 0.25;
		centerXFactor = 0.25;
		vertAngle = 90;
		horizAngle = 0.0;
		strcpy(planetName, "Mercury");
	}
	else if(*ptrNames == 2){
		singlePlanetMode = true;
		zoomFactor = 0.25;
		camXFactor = 0.5;
		centerXFactor = 0.5;
		vertAngle = 90;
		horizAngle = 0.0;
		strcpy(planetName, "Venus");
	}
	else if(*ptrNames == 3){
		singlePlanetMode = true;
		zoomFactor = 0.25;
		camXFactor = 0.75;
		centerXFactor = 0.75;
		vertAngle = 90;
		horizAngle = 0.0;
		strcpy(planetName, "Earth");
	}
	else if(*ptrNames == 4){
		singlePlanetMode = true;
		zoomFactor = 0.2;
		camXFactor = 1.0;
		centerXFactor = 1.0;
		vertAngle = 90;
		horizAngle = 0.0;
		strcpy(planetName, "Mars");
	}
	else if(*ptrNames == 5){
		singlePlanetMode = true;
		zoomFactor = 0.55;
		camXFactor = 1.25;
		centerXFactor = 1.25;
		vertAngle = 90;
		horizAngle = 0.0;
		strcpy(planetName, "Jupiter");
	}
	else if(*ptrNames == 6){
		singlePlanetMode = true;
		zoomFactor = 0.55;
		camXFactor = 1.5;
		centerXFactor = 1.5;
		vertAngle = 90;
		horizAngle = 0.0;
		strcpy(planetName, "Saturn");
	}
	else if(*ptrNames == 7){
		singlePlanetMode = true;
		zoomFactor = 0.4;
		camXFactor = 1.75;
		centerXFactor = 1.75;
		vertAngle = 90;
		horizAngle = 0.0;
		strcpy(planetName, "Uranus");
	}
	else if(*ptrNames == 8){
		singlePlanetMode = true;
		zoomFactor = 0.4;
		camXFactor = 2.0;
		centerXFactor = 2.0;
		vertAngle = 90;
		horizAngle = 0.0;
		strcpy(planetName, "Neptune");
	}
	else if(*ptrNames == 9){
		singlePlanetMode = true;
		zoomFactor = 0.2;
		camXFactor = 2.25;
		centerXFactor = 2.25;
		vertAngle = 90;
		horizAngle = 0.0;
		strcpy(planetName, "Pluto");
	}
	else{
		cout << "Didn't click a planet!\n";
	}  

}

void drawString(char *string){
	glRasterPos2i(475,620); //800,640
	void *font = GLUT_BITMAP_HELVETICA_18; 
	for (char* c=string; *c != '\0'; c++) {
		glutBitmapCharacter(font, *c); 
	}
}

void beginText(){
	glMatrixMode(GL_PROJECTION);
	glPushMatrix();
	glLoadIdentity();
	gluOrtho2D(0, 1024, 0, 720);

	glMatrixMode(GL_MODELVIEW);
	glPushMatrix();
	glLoadIdentity();
}

void endText(){
	glMatrixMode(GL_MODELVIEW);
	glPopMatrix();
	glMatrixMode(GL_PROJECTION);
	glPopMatrix();
}

void myKeyboard(unsigned char key, int x, int y){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	switch (key)
    {
    case 's':
		if(moveMode == false){
			moveMode = true;
		}
		else if(moveMode == true){
			moveMode = false;
		}
	break;
	case 'd':
		zoomFactor = 6.25;
		camXFactor = 0.0;
		centerXFactor = 0.0;
		rotationalAngle = 0.0;
		vertAngle = 0.0;
		horizAngle = 0.0;
		mercuryOrbit = 0.0;
		venusOrbit = 0.0;
		earthOrbit = 0.0;
		marsOrbit = 0.0;
		jupiterOrbit = 0.0;
		saturnOrbit = 0.0;
		uranusOrbit = 0.0;
		neptuneOrbit = 0.0;
		plutoOrbit = 0.0;
		moveMode = false;
		singlePlanetMode = false;
	break;
    case 'e':
		exit(0);			// exit
    }
	
	/*sets a flag so that when the mainloop is ready it will call the real "display" callback once per mainloop iteration*/
	glutPostRedisplay();		//refresh the display, redraw the image now
}

void arrowKeyPress(int key, int x, int y){
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	switch(key){
	case GLUT_KEY_RIGHT:
		horizAngle += 2;
	break;
	case GLUT_KEY_LEFT:
		horizAngle -= 2;
	break;
	case GLUT_KEY_UP:
		vertAngle -= 2;
	break;
	case GLUT_KEY_DOWN:
		vertAngle += 2;
	break;
    }

    glutPostRedisplay();		// redraw the image now
}


void main(int argc, char **argv){

	glutInit(&argc, argv);  //OpenGL utility kit
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);  //double buffering for animation
	glutInitWindowSize(800, 640);
	glutInitWindowPosition(100, 100);
	glutCreateWindow("It's the Solar System!!!");
	glutDisplayFunc(myDisplay);
	glutIdleFunc(Move);		//Ongoing Moveing function for when nothing else happening
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glColor3f(1.0, 1.0, 1.0);

	glutKeyboardFunc(myKeyboard);	//myKeyboard will handle normal keys
	glutSpecialFunc(arrowKeyPress);	//arrowKeyPress will handle special keys
	glutMouseFunc(myMouse);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_NORMALIZE);
	glEnable (GL_COLOR_MATERIAL);	//Material color match vertex color specified
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_TEXTURE_2D);

	myInit();
	glutMainLoop();
}