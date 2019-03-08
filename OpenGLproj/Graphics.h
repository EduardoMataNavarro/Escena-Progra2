#ifndef _GRR
#define _GRR

#include <windows.h>
#include <gdiplus.h>
#include <windowsx.h>
#include <gl\GLU.h>
#include <gl\GL.h>
#include <cmath>
#include "glext.h"

using namespace Gdiplus;

class Camara {
public:
	float poscx, poscy, poscz, dircx, dircy, dircz;

	Camara()
	{
	}

	void CamaraUpdate()
	{
		gluLookAt(poscx, poscy, poscz,			// posición de la cámara
			      dircx, dircy, dircz,		// hacia donde "ve" la cámara
			      0, 1, 0);					// Vector "UP"
	}
};

class Graph : public Camara
{
private: 
public:
	bool ufoRay;
	GLfloat LightAmb[4] = { 1.0f, 1.0f, 1.0f, 0.3f };
	GLfloat lux, luy, luz;
	GLfloat meshRotation = 0, levSheep;
	GLfloat LightPos1[4] = { -lux, 20.0f, 10.0f, 0.0f };
	GLdouble smokeSize = 1, smokeHeight = 20, bucketY, ropeH, ufoX, ufoZ;
	GLdouble duck1movZ, duck2movZ;

	Graph(HWND hWnd)
	{
		//experimenten cambiandole de valores a la camara
		poscx = 0;
		poscy = 45;
		poscz = 115;
		dircx = 0;
		dircy = 0;
		dircz = 0;

		levSheep = 15;
		duck1movZ = 10;
		duck2movZ = -10;
		bucketY = 11;
		ropeH = 2;
		ufoX = 5;
		ufoZ = 5;
		ufoRay = false;
		glShadeModel(GL_SMOOTH);
		//habilitamos el control de profundidad en el render
		glEnable(GL_DEPTH_TEST);
		//habilitamos la iluminacion
		glEnable(GL_LIGHTING);
		// habilitamos la luz 0 o primer luz
		glEnable(GL_LIGHT0);
		glEnable(GL_LIGHT1);
		//habilitamos la forma de reflejar la luz
		glEnable(GL_COLOR_MATERIAL);
	}
	void dibujarCilindroCompleto(GLUquadricObj *quadratic, double base, double top, double height, double slices, double stacks)
	{
		// El truco esta en crear dos discos, los cuales posicionamos
		// uno de cada lado del cilindro, dando el efecto de que es un
		// cilindro "completo" o "cerrado".
		glPushMatrix();
		gluCylinder(quadratic, base, top, height, slices, stacks);
		glRotatef(180, 1, 0, 0);
		gluDisk(quadratic, 0.0f, top, slices, 1);
		glRotatef(180, 1, 0, 0);
		glTranslatef(0.0f, 0.0f, height);
		gluDisk(quadratic, 0.0f, top, slices, 1);
		glPopMatrix();
	}
	void drawConeTree( double posX, double posZ)
	{
		glPushMatrix();
		glTranslated(posX, 10, posZ);
		glColor3f(.65, .28, .15);
		glRotated(90, 5, 0, 0);
		gluCylinder(gluNewQuadric(), 1, 1, 8, 24, 10);
		glPopMatrix();
		glPushMatrix();
		glColor3f(.1, .95, .2);
		glTranslated(posX, 10, posZ);
		glRotated(270, 5, 0, 0);
		glutSolidCone(2.75, 5, 24, 10);
		glPopMatrix();
		glPushMatrix();
		glTranslated(posX, 13, posZ);
		glRotated(270, 5, 0, 0);
		glutSolidCone(2.4, 2, 24, 10);
		glPopMatrix();

	}
	void drawRiverRock(double startX, double incremental)
	{
		glPushMatrix();
		glColor3d(0.561, 0.737, 0.561);
		glTranslated(startX, 4, 49 - incremental);
		glutSolidSphere(1, 20, 20);
		glPopMatrix();
	}
	void drawNormalTree(double posX, double posZ)
	{
		glPushMatrix();
		glTranslated(posX, 11.33, posZ);
		glRotated(90, 1, 0, 0);
		glColor3d(0.545, 0.271, 0.075);
		gluCylinder(gluNewQuadric(), 1, 1, 7, 20, 20);
		glPopMatrix();
		glPushMatrix();
		glColor3d(0.133, 0.545, 0.133);
		glTranslated(posX, 10, posZ);
		glutSolidSphere(2, 25, 10);
		glPopMatrix();
		glPushMatrix();
		glTranslated(posX -1, 10.5, posZ);
		glutSolidSphere(2, 25, 10);
		glPopMatrix();
		glPushMatrix();
		glTranslated(posX + 2, 10.5, posZ);
		glutSolidSphere(2.5, 25, 10);
		glPopMatrix();
		glPushMatrix();
		glTranslated(posX - 0.25, 11.5, posZ);
		glutSolidSphere(2.5, 25, 10);
		glPopMatrix();

	}
	void drawSheep(double posX, double posY, double posZ)
	{
		glPushMatrix();
			glColor3d(0.871, 0.722, 0.529);
			glTranslated(posX, posY, posZ);
			glScaled(1.5, .80, 1);
			glutSolidCube(4);
		glPopMatrix();						//Cuerpo de la ovejita
		glPushMatrix();
		glColor3d(1.000, 0.871, 0.678);
			glTranslated(posX - 3, posY + 2, posZ);
			glutSolidCube(2);
		glPopMatrix();						//Cabecita de la ovejita
		glPushMatrix();
			glColor3d(0,0,0);
			glTranslated(posX - 3.85, posY + 2.01, posZ);
			glutSolidCube(1.5);
		glPopMatrix();
		glPushMatrix();
			glColor3d(1, 1, 1);
			glTranslated(posX - 4.5, posY + 2.15, posZ - 0.5);
			glutSolidSphere(.25, 24, 10);
		glPopMatrix();
		glPushMatrix();
			glColor3d(1, 1, 1);
			glTranslated(posX - 4.5, posY + 2.15, posZ + 0.5);
			glutSolidSphere(.25, 24, 10);
		glPopMatrix();
		glPushMatrix();
			glTranslated(posX - 2, posY, posZ + 1);
			glColor3d(0, 0, 0);
			glRotated(90, 1, 0, 0);
			gluCylinder(gluNewQuadric(), .25, .25, 3, 24, 10);
		glPopMatrix();						//Patita frontal izquierda
		glPushMatrix();
			glTranslated(posX - 2, posY, posZ -1);
			glColor3d(0, 0, 0);
			glRotated(90, 1, 0, 0);
			gluCylinder(gluNewQuadric(), .25, .25, 3, 24, 10);
		glPopMatrix();						//Patita frontal derecha
		glPushMatrix();
			glTranslated(posX + 2, posY, posZ -1);
			glColor3d(0, 0, 0);
			glRotated(90, 1, 0, 0);
			gluCylinder(gluNewQuadric(), .25, .25, 3, 24, 10);
		glPopMatrix();						//Patita trasera derecha
		glPushMatrix();
			glTranslated(posX + 2, posY, posZ + 1);
			glColor3d(0, 0, 0);
			glRotated(90, 1, 0, 0);
			gluCylinder(gluNewQuadric(), .25, .25, 3, 24, 10);
		glPopMatrix();						//Patita trasera izquierda

	}
	void drawUFO(double posX, double posY, double posZ, bool ray)
	{
		glPushMatrix();
			glColor3d(0.502, 0.502, 0.502);
			glTranslated(posX, posY, posZ);
			glRotated(90, 1, 0, 0);
			dibujarCilindroCompleto(gluNewQuadric(), 5, 5, 1.5, 24, 10);
		glPopMatrix();
		glPushMatrix();
			glColor3d(0.604, 0.804, 0.196);
			glTranslated(posX, posY, posZ);
			glScaled(1, .5, 1);
			glutSolidSphere(3.5, 24, 10);
		glPopMatrix();
		if (ray)
		{
			glPushMatrix();
			glColor3d(0.416, 0.353, 0.804);
			glTranslated(posX, 5, posZ);
			glRotated(270, 10, 0, 0);
			glRotated(meshRotation, 0, 0, 1);
			glutWireCone(12, posY - 5, 10, 100);
			glPopMatrix();
		}
	}
	void drawDuck(double posX, double posZ)
	{
		glPushMatrix();
			glColor3d(1, 1, 1);
			glTranslated(posX, 5, posZ);
			glScaled(1.5, 1, 1.75);
			glutSolidCube(2);
		glPopMatrix();
		glPushMatrix();
			glColor3d(1.000, 0.894, 0.710);
			glTranslated(posX - 0.05, 7, posZ - 1);
			glScaled(1, 3, 1);
			glutSolidCube(1);
		glPopMatrix();
		glPushMatrix();
			glColor3d(1.000, 0.843, 0.000);
			glTranslated(posX - 0.05, 8, posZ - 1.5);
			glScaled(1, .5, 2);
			glutSolidCube(0.5);
		glPopMatrix();
		glPushMatrix();
			glColor3d(0, 0, 0);
			glTranslated(posX + .2, 8.4, posZ - 1.65);
			glutSolidSphere(.08, 10, 10);
		glPopMatrix();
		glPushMatrix();
			glColor3d(0, 0, 0);
			glTranslated(posX - .2, 8.4, posZ - 1.65);
			glutSolidSphere(.08, 10, 10);
		glPopMatrix();

	}
	//el metodo render toma el dispositivo sobre el cual va a dibujar
	//y hace su tarea ya conocida
	void Render(HDC hDC)
	{
		//borramos el biffer de color y el z para el control de profundidad a la 
		//hora del render a nivel pixel.
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glLightfv(GL_LIGHT0, GL_AMBIENT, LightAmb);
		glLightfv(GL_LIGHT0, GL_POSITION, LightPos1);
		//esta instruccion  limpia la matriz que acumula las transformaciones hechas
		//que implican movimiento y/o deformaciones.
		glLoadIdentity();
		CamaraUpdate();
		//---------------------------------------------------------------
		glPushMatrix();
		glColor3d(.5, .23, .12);
		glScalef(25, 1, 20);
		glTranslated(0, 2, 0);
		glutSolidCube(5);
		glPopMatrix();
		glPushMatrix();
			
		//---------------------------------------------------------------Forma del piso de la escena
		glPushMatrix();
		glScalef(2.5, 3, 5);
		glColor3d(.65, .25, .10);
		glTranslated(-12, 3, 5);
		glutSolidCube(6);
		glPopMatrix();
		glPushMatrix();
		glColor3d(0.663, 0.663, 0.663);
		glTranslated(-35, 20, 35);
		glRotated(90, 5, 0, 0);
		gluCylinder(gluNewQuadric(), 1, 1, 3, 24, 10);
		glPopMatrix();
		glPushMatrix();
		glColor3d(0.686, 0.933, 0.933);
		glTranslated(-23, 10, 30);
		glutSolidCube(2);
		glPopMatrix();
		glPushMatrix();
		glColor3d(0.871, 0.722, 0.529);
		glRotated(90, 0, 5, 0);
		glTranslated(-20, 8, -22);
		glScaled(1, 1.85, 0.1);
		glutSolidCube(3);
		glPopMatrix();
		//--------------------------------------------------------------Forma de la casita en la escena
		drawRiverRock(5, 5);
		drawRiverRock(15, 39);
		drawRiverRock(16, 37);
		drawRiverRock(25, 15);
		drawRiverRock(35, 20);
		drawRiverRock(37, 21);
		drawRiverRock(16, 65);
		drawRiverRock(25, 38);
		drawRiverRock(35, 46);
		drawRiverRock(37, 41);

		drawConeTree(5, 4);
		drawConeTree(10, 25);
		drawConeTree(20, 32);
		drawConeTree(8, 20);
		drawConeTree(5, 39);
		drawConeTree(8, -20);
		drawConeTree(5, -39);
		drawConeTree(42, 0);
		drawConeTree(50, 0);
		drawConeTree(42, -20);
		drawConeTree(45, 32);
		drawConeTree(50, 40);
		drawConeTree(42, 45);
		drawConeTree(50, -45);
		drawConeTree(45, 15);
		drawConeTree(50, -40);
		drawConeTree(42, 40);
		drawConeTree(-15, -40);
		for (double i = 0; i < 10; i++)
			drawNormalTree(-56, 45 - (i * 10));
		for (double i = 0; i < 10; i++)
			drawNormalTree(56, 45 - (i * 10));
		for (double i = 0; i < 8; i++)
			drawNormalTree(-38 + (i * 10), 45 );
		for (double i = 0; i < 8; i++)
			drawNormalTree(-38 + (i * 10), -45);
		//--------------------------------------------------------------Árboles cónicos
		glPushMatrix();
			glColor3d(0.275, 0.510, 0.706);
			glTranslated(-44.95, 4.8, 0);
			glScaled(1.95, .01, 20);
			glutSolidCube(5);
		glPopMatrix();
		for (int i = 0; i <= 39; i++)
		{
			drawRiverRock(-50, i * 2.5);
			drawRiverRock(-40, i * 2.5);
		}
		//---------------------------------------------------------------Piedras de río
		drawNormalTree(32, -25);
		drawNormalTree(23, 25);
		drawNormalTree(29, -20);
		drawNormalTree(35, -35);
		drawNormalTree(22, -29);
		drawNormalTree(32, 39);
		drawNormalTree(29, 20);
		drawNormalTree(-10, 10);
		drawNormalTree(0, 39);
		drawNormalTree(1, 20);
		drawNormalTree(10, -10);
		drawNormalTree(5, 0);
		drawNormalTree(20, 5);
		drawNormalTree(25, -10);
		drawNormalTree(-15, -45);

		//---------------------------------------------------------------Árboles normales
		glPushMatrix();
			glColor3d(0.412, 0.412, 0.412);
			glTranslated(-35, smokeHeight, 35);
			glutSolidSphere(smokeSize, 25, 10);
		glPopMatrix();
		//--------------------------------------------------------------Humo
		glPushMatrix();
			glTranslated(25, 9.5, -15);	
			glRotated(90, 1, 0, 0);
			gluCylinder(gluNewQuadric(), 3, 3, 5, 24, 10);
		glPopMatrix();
		glPushMatrix();
			glColor3d(0.275, 0.510, 0.706);
			glTranslated(25, 9, -15);
			glRotated(90, 1, 0, 0);
			gluDisk(gluNewQuadric(), 0, 3, 24, 1);
		glPopMatrix();
		//Cubeta
		glPushMatrix();
			glColor3d(0.737, 0.561, 0.561);
			glTranslated(25, bucketY, -15);
			glRotated(270, 1, 0, 0);
			gluCylinder(gluNewQuadric(), .5, 1, 1.5, 24, 10);
			gluDisk(gluNewQuadric(), 0, .5, 24, 1);
		glPopMatrix();
		//Base 
		glPushMatrix();
			glColor3d(0.627, 0.322, 0.176);
			glTranslated(22, 5, -15);
			glRotated(270, 1, 0, 0);
			dibujarCilindroCompleto(gluNewQuadric(), .25, .25, 9, 24, 50);
		glPopMatrix();
		glPushMatrix();//Medio
			glColor3d(0.627, 0.322, 0.176);
			glTranslated(22, 13.7, -15);
			glRotated(90, 0, 1, 0);
			dibujarCilindroCompleto(gluNewQuadric(), .15, .15, 6, 24, 50);
		glPopMatrix();
		glPushMatrix();//Cuerda inicio
			glColor3d(0.855, 0.647, 0.125);
			glTranslated(24, 13.7, -15);
			glRotated(90, 0, 1, 0);
			dibujarCilindroCompleto(gluNewQuadric(), .18, .18, 2, 24, 50);
		glPopMatrix();
		glPushMatrix();//Cuerda 
			glColor3d(0.855, 0.647, 0.125);
			glTranslated(25, 13.7, -15);
			glRotated(90, 1, 0, 0);
			dibujarCilindroCompleto(gluNewQuadric(), .18, .18, ropeH, 24, 50);
		glPopMatrix();
		glPushMatrix();
			glColor3d(0.627, 0.322, 0.176);
			glTranslated(28, 5, -15);
			glRotated(270, 1, 0, 0);
			dibujarCilindroCompleto(gluNewQuadric(), .25, .25, 9, 24, 50);
		glPopMatrix();
		//------------------------------------------------------------Pozo
		glPushMatrix();
			glColor3d(0.741, 0.718, 0.420);
			glTranslated(-42, 5.5, 22);
			glRotated(90, 0, 5, 0);
			dibujarCilindroCompleto(gluNewQuadric(), 10, 10, 2, 24, 10);
		glPopMatrix();
		glPushMatrix();
			glColor3d(0.545, 0.271, 0.075);
			glTranslated(-40, 13, 25);
			glRotated(90, 0, 5, 0);
			gluCylinder(gluNewQuadric(), 1, 1, 3, 24, 10);
		glPopMatrix();
		//-------------------------------------------------------------Rueda
		drawSheep(35, 8, 0);
		drawSheep(20, 8, 9);
		drawSheep(-26, 8, -4);
		drawSheep(-30, 22, 16);;	
		drawSheep(0, levSheep, -20);
		//-------------------------------------------------------------Ovejitas
		glPushMatrix();
			glColor3d(0.416, 0.353, 0.804);
			glTranslated(0, 5, -20);
			glRotated(270, 10, 0, 0);
			glRotated(meshRotation, 0, 0, 1);
			glutWireCone(12, 30, 10, 100);
		glPopMatrix();
		drawUFO(0, 35, -20, true);
		drawUFO(ufoX, 30, ufoZ, ufoRay);
		drawDuck(-45, duck1movZ);
		drawDuck(-45, duck2movZ);
		SwapBuffers(hDC);
	}
};

#endif 

