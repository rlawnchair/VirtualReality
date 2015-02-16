// z_TestSDK.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "SDK_SampleCode.h"
#include <iostream>
using namespace std;
using namespace PQ_SDK_MultiTouchSample;

/*int _tmain(int argc, _TCHAR* argv[])
{
	Sample sample;
	int err_code = sample.Init();
	if(err_code != PQMTE_SUCCESS){
		cout << "press any key to exit..." << endl
		getchar();
		return 0;
	}
	// do other things of your application;
	cout << "hello world" << endl;
	//
	// here just wait here, not let the process exit;
	char ch  = 0;
	while(ch != 'q' && ch != 'Q'){
		cout << "press \'q\' to exit" << endl;
		ch = getchar();
	}
	return 0;
}*/


#include "GL\glut.h"

extern float mouseX;
extern float mouseY;
extern float mouseDraggingX;
extern float mouseDraggingY;
extern float sphereX;
extern float sphereY;
extern bool mouving;

extern float mouseDraggingStartX;
extern float mouseDraggingStartY;

extern float mouseDraggingEndX;
extern float mouseDraggingEndY;

float coneMoveX = mouseDraggingStartX;
float coneMoveY = mouseDraggingStartY;

 GLdouble worldX = 0.0, worldY = 0.0, worldZ = 0.0; //variables to hold world x,y,z coordinates
  GLdouble worldSphereX = 0.0, worldSphereY = 0.0, worldSphereZ = 0.0;
  GLdouble worldConeX = 0.0, worldConeY = 0.0, worldConeZ = 0.0;

Sample sample;

void timer(int v) {

 /* if(coneMoveX < mouseDraggingEndX)
  coneMoveX++;
  else
  coneMoveX--;

  if(coneMoveY < mouseDraggingEndY)
  coneMoveX++;
  else
  coneMoveY--;*/

  
		cout<<"MOVING CONE  "<<endl;

	   GLint viewport[4]; //var to hold the viewport info
        GLdouble modelview[16]; //var to hold the modelview info
        GLdouble projection[16]; //var to hold the projection matrix info
        GLfloat winX, winY, winZ; //variables to hold screen x,y,z coordinates
       // GLdouble worldX, worldY, worldZ; //variables to hold world x,y,z coordinates
 
        glGetDoublev( GL_MODELVIEW_MATRIX, modelview ); //get the modelview info
        glGetDoublev( GL_PROJECTION_MATRIX, projection ); //get the projection matrix info
        glGetIntegerv( GL_VIEWPORT, viewport ); //get the viewport info
 
	//get the world coordinates from the screen coordinates
		  gluUnProject( coneMoveX, coneMoveY, 0.0, modelview, projection, viewport, &worldConeX, &worldConeY, &worldConeZ);
	   glutPostRedisplay();

  glutPostRedisplay();
  glutTimerFunc(1000/60, timer, v);
}

void display() {
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glMatrixMode(GL_MODELVIEW);
  glPushMatrix();

  // Rotate the scene so we can see the tops of the shapes.
  glRotatef(-20.0, 1.0, 0.0, 0.0);

  // Make a torus floating 0.5 above the x-z plane.  The standard torus in
  // the GLUT library is, perhaps surprisingly, a stack of circles which
  // encircle the z-axis, so we need to rotate it 90 degrees about x to
  // get it the way we want.
  glPushMatrix();
  //glTranslatef(-0.75, 0.5, 0.0);
  //glTranslatef(mouseX/1000, mouseY/1000, 0.0);

  glTranslatef(worldX, worldY, 0.0);

  glRotatef(90.0, 1.0, 0.0, 0.0);
  glutSolidTorus(0.275, 0.85, 16, 40);
  glPopMatrix();

  // Make a cone.  The standard cone "points" along z; we want it pointing
  // along y, hence the 270 degree rotation about x.
  glPushMatrix();
 // glTranslatef(-0.75, -0.5, 0.0);
  glTranslatef(worldConeX, worldConeY, 0.0);
  glRotatef(270.0, 1.0, 0.0, 0.0);
  glutSolidCone(1.0, 2.0, 70, 12);
  glPopMatrix();

  for(int i = mouseDraggingStartX; i< mouseDraggingEndX; ++i){
  
	   for(int j = mouseDraggingStartY; j< mouseDraggingEndY; ++j){
			
		    glPushMatrix();
			 // glTranslatef(-0.75, -0.5, 0.0);
			  glTranslatef(i, j, 0.0);
			  glRotatef(270.0, 1.0, 0.0, 0.0);
			  glutSolidCone(1.0, 2.0, 70, 12);
			  glPopMatrix();
	 }
	   glutPostRedisplay();
  }

  // Add a sphere to the scene.
  glPushMatrix();
  //glTranslatef(0.75, 0.0, -1.0);
  glTranslatef(worldSphereX, worldSphereY, 0.0);
  glutSolidSphere(1.0, 30, 30);
  glPopMatrix();

  glPopMatrix();
  glFlush();
    glutSwapBuffers();

	// equation de droite

	/*
			mouseDraggedStartY = a*mouseDraggedStartX + b
			b = mouseDraggedStartY - a*mouseDraggedStartX

			mouseDraggedEndY = a*mouseDraggedEndX + b => mouseDraggedEndY = a*mouseDraggedEndX +  mouseDraggedStartY - a*mouseDraggedStartX => mouseDraggedEndY = a*(mouseDraggedEndX- mouseDraggedStartX) +  mouseDraggedStartY 
			:: => a = (mouseDraggedEndY -  mouseDraggedStartY)/(mouseDraggedEndX- mouseDraggedStartX)

			y = a*x + b

	*/
	
	
		float a = (mouseDraggingEndY -  mouseDraggingStartY)/(mouseDraggingEndX- mouseDraggingStartX);
float b = mouseDraggingStartY - a*mouseDraggingStartX;

		coneMoveY = a*coneMoveX + b;

		if(coneMoveX < mouseDraggingEndX)
		coneMoveX++;
		else
			coneMoveX--;
		
	
}

// We don't want the scene to get distorted when the window size changes, so
// we need a reshape callback.  We'll always maintain a range of -2.5..2.5 in
// the smaller of the width and height for our viewbox, and a range of -10..10
// for the viewbox depth.
void reshape(GLint w, GLint h) {
  glViewport(0, 0, w, h);
  glMatrixMode(GL_PROJECTION);
  GLfloat aspect = GLfloat(w) / GLfloat(h);
  glLoadIdentity();
  if (w <= h) {
    // width is smaller, so stretch out the height
    glOrtho(-2.5, 2.5, -2.5/aspect, 2.5/aspect, -10.0, 10.0);
  } else {
    // height is smaller, so stretch out the width
    glOrtho(-2.5*aspect, 2.5*aspect, -2.5, 2.5, -10.0, 10.0);
  }
}

// Performs application specific initialization.  It defines lighting
// parameters for light source GL_LIGHT0: black for ambient, yellow for
// diffuse, white for specular, and makes it a directional source
// shining along <-1, -1, -1>.  It also sets a couple material properties
// to make cyan colored objects with a fairly low shininess value.  Lighting
// and depth buffer hidden surface removal are enabled here.
void init() {

//  glOrtho(0, 1920, 0, 1024, -10.0, 10.0);
  GLfloat black[] = { 0.0, 0.0, 0.0, 1.0 };
  GLfloat yellow[] = { 1.0, 1.0, 0.0, 1.0 };
  GLfloat cyan[] = { 0.0, 1.0, 1.0, 1.0 };
  GLfloat white[] = { 1.0, 1.0, 1.0, 1.0 };
  GLfloat direction[] = { 1.0, 1.0, 1.0, 0.0 };

  glMaterialfv(GL_FRONT, GL_AMBIENT_AND_DIFFUSE, cyan);
  glMaterialfv(GL_FRONT, GL_SPECULAR, white);
  glMaterialf(GL_FRONT, GL_SHININESS, 30);

  glLightfv(GL_LIGHT0, GL_AMBIENT, black);
  glLightfv(GL_LIGHT0, GL_DIFFUSE, yellow);
  glLightfv(GL_LIGHT0, GL_SPECULAR, white);
  glLightfv(GL_LIGHT0, GL_POSITION, direction);

  glEnable(GL_LIGHTING);                // so the renderer considers light
  glEnable(GL_LIGHT0);                  // turn LIGHT0 on
  glEnable(GL_DEPTH_TEST);              // so the renderer considers depth

  int err_code = sample.Init();
	if(err_code != PQMTE_SUCCESS){
		cout << "press any key to exit..." << endl;
		getchar();
		
	}
}

void myMouse(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON && state == GLUT_DOWN)
	{
	   //isDragging = true;
	   mouseX = x;
	   mouseY = y;
	   cout<<"LEFT MOUSE DOWN MOVE TORUS TO X:"<<mouseX<<" TO Y: "<<mouseY<<endl;
	   cout<<"MOVE SPHERE TO X:"<<sphereX<<" TO Y: "<<sphereY<<endl;

	   GLint viewport[4]; //var to hold the viewport info
        GLdouble modelview[16]; //var to hold the modelview info
        GLdouble projection[16]; //var to hold the projection matrix info
        GLfloat winX, winY, winZ; //variables to hold screen x,y,z coordinates
       // GLdouble worldX, worldY, worldZ; //variables to hold world x,y,z coordinates
 
        glGetDoublev( GL_MODELVIEW_MATRIX, modelview ); //get the modelview info
        glGetDoublev( GL_PROJECTION_MATRIX, projection ); //get the projection matrix info
        glGetIntegerv( GL_VIEWPORT, viewport ); //get the viewport info
 
	winX = (float)x;
        winY = (float)viewport[3] - (float)y;
	winZ = 0;
 
	//get the world coordinates from the screen coordinates
        gluUnProject( winX, winY, winZ, modelview, projection, viewport, &worldX, &worldY, &worldZ);
		 gluUnProject( sphereX, sphereY, 0.0, modelview, projection, viewport, &worldSphereX, &worldSphereY, &worldSphereZ);
		 // gluUnProject( mouseDraggingX, mouseDraggingY, 0.0, modelview, projection, viewport, &worldConeX, &worldConeY, &worldConeZ);
	   glutPostRedisplay();

	 //  if(state == GLUT_UP)
		   //isDragging = false;
	}
}

void mouseMove(int x, int y) {
 
	//if(isDragging){
		mouseDraggingX = x;
		mouseDraggingY = y;

		/*cout<<"MOVING CONE TO X:"<<sphereX<<" TO Y: "<<sphereY<<endl;

	   GLint viewport[4]; //var to hold the viewport info
        GLdouble modelview[16]; //var to hold the modelview info
        GLdouble projection[16]; //var to hold the projection matrix info
        GLfloat winX, winY, winZ; //variables to hold screen x,y,z coordinates
       // GLdouble worldX, worldY, worldZ; //variables to hold world x,y,z coordinates
 
        glGetDoublev( GL_MODELVIEW_MATRIX, modelview ); //get the modelview info
        glGetDoublev( GL_PROJECTION_MATRIX, projection ); //get the projection matrix info
        glGetIntegerv( GL_VIEWPORT, viewport ); //get the viewport info
 
	winX = (float)x;
        winY = (float)viewport[3] - (float)y;
	winZ = 0;
 
	//get the world coordinates from the screen coordinates
		  gluUnProject( mouseDraggingX, mouseDraggingY, 0.0, modelview, projection, viewport, &worldConeX, &worldConeY, &worldConeZ);
	   glutPostRedisplay();*/

	//}
}

// The usual application statup code.
int main(int argc, char** argv) {
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB | GLUT_DEPTH);
  glutInitWindowPosition(0, 0);
  glutInitWindowSize(1920, 1024);
  glutCreateWindow("Cyan Shapes in Yellow Light");
  glutReshapeFunc(reshape);
  glutDisplayFunc(display);
  glutMouseFunc( myMouse );
  init();
  glutTimerFunc(100, timer, 0);
  glutMainLoop();
}