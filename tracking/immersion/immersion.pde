import javax.media.opengl.GL;
import processing.opengl.*;
import java.util.Vector;
import vrpn.*;

// Screen resolution in pixels
static final int screenResolutionX = 2560;
static final int screenResolutionY = 1440;

// Screen size in mm
PVector screenSize = new PVector(285, 180);

// Position of the virtual camera
PVector camPos = new PVector(0, 0, 500);

// Half of the distance between each eyes
float halfEyeDist = 8;  //  a separation of 16 to 18 mm is confortable for most people.

// Parameters of the camera
float nearPlane = 10; 
float farPlane = 2000;

// Last tracked position
double lastX;
double lastY;

PGL pgl;


TrackerRemote m_tracker = null;
TrackerRemoteListener m_trackerListener = null;

void setup() 
{
  size(screenResolutionX, screenResolutionY, P3D);
  //On crée les classes pour accéder au tracker et button vrpn 
  try { 
    m_tracker = new TrackerRemote( "Tracker@localhost", null, null, null, null );
  }
  catch( InstantiationException e ) {
    println("Error when creating the vrpn Tracker and Button devices");
  }
  m_trackerListener = new TrackerRemoteListener(m_tracker);
}

void draw() 
{
  background(0);

  //drawMono();
  drawLeft();
  drawRight();
  Vector updates = m_trackerListener.getTrackerUpdate();
  if (updates != null) {
    for (int i=0; i<updates.size(); i++) {
      vrpn.TrackerRemote.TrackerUpdate u = (vrpn.TrackerRemote.TrackerUpdate)updates.elementAt(i);
      if (u.pos[0] > lastX)
        camPos.x-=10;
      if (u.pos[0] < lastX)
        camPos.x+=10;
      if (u.pos[1] < lastY)
        camPos.y+=10;
      if (u.pos[1] > lastY)
        camPos.y-=10;

      lastX = u.pos[0];
      lastY = u.pos[1];
      println(" sensor : "+u.sensor+" pos: "+u.pos[0]+","+u.pos[1] +","+u.pos[2]);
    }
  }
  ry+= 0.01;
  rx+= 0.01;
  if (abs(tz) == 100)
    sign = -sign;
  tz = tz + sign;
}

void drawMono() 
{
  setCamera(camPos);
  drawScene();
}

void drawLeft()
{
  pgl = beginPGL();
  pgl.colorMask(true, false, false, true);
  PVector camUpdate = new PVector(camPos.x-halfEyeDist, camPos.y, camPos.z);
  setCamera(camUpdate);
  drawScene();
  pgl.clear(PGL.DEPTH_BUFFER_BIT);
  endPGL();
}

void drawRight()
{
  pgl = beginPGL();
  pgl.colorMask(false, true, true, true);
  pgl.clear(PGL.DEPTH_BUFFER_BIT);
  endPGL();
  PVector camUpdate = new PVector(camPos.x+halfEyeDist, camPos.y, camPos.z);
  setCamera(camUpdate);
  drawScene();
  pgl = beginPGL();
  pgl.colorMask(true, true, true, true);
  pgl.clear(PGL.DEPTH_BUFFER_BIT);
  endPGL();
}

void setCamera(PVector cameraPos) {

  PGraphicsOpenGL graphics = (PGraphicsOpenGL) g;

  camera(cameraPos.x, cameraPos.y, cameraPos.z, cameraPos.x, cameraPos.y, 0, 0, 1, 0);

  float nearFactor = nearPlane / cameraPos.z;
  float left   = nearFactor * (- screenSize.x / 2f - cameraPos.x);
  float right  = nearFactor * (  screenSize.x / 2f - cameraPos.x);
  float top    = nearFactor * (  screenSize.y / 2f - cameraPos.y);
  float bottom = nearFactor * (- screenSize.y / 2f - cameraPos.y);

  frustum(left, right, bottom, top, nearPlane, farPlane);

  graphics.projection.m11 = -graphics.projection.m11;
}

/*
void mouseMoved() {
  if (mouseX > lastX)
    camPos.x--;
  if (mouseX < lastX)
    camPos.x++;
  if (mouseY > lastY)
    camPos.y++;
  if (mouseY < lastY)
    camPos.y--;

  lastX = mouseX;
  lastY = mouseY;
}
*/
float ry = 0;
float rx = 0;
int tz = 0;
int sign = 1;

void drawScene()
{
  lights();

  // Draw a rotating cube
  pushMatrix();
  translate(0, 0, tz);
  rotateY(ry);
  rotateX(rx);
  fill(255);
  noStroke();
  // cube de 5cm
  scale(50);
  box(1);
  popMatrix();


  // Draw a 3D grid
  stroke(255);
  strokeWeight(2);

  int numLines = 10;
  float lineDist = 40;
  int intensStep = 255 / numLines;

  // Horizontal lines
  float stepSize = screenSize.x / numLines;

  for (int i = 0; i <  numLines; i++) {
    stroke(255 - i * intensStep);

    // Bottom
    line(-screenSize.x/2f, -screenSize.y/2f, -i * lineDist, 
    screenSize.x/2f, -screenSize.y/2f, -i * lineDist);

    // Top
    line(-screenSize.x/2f, screenSize.y/2f, -i * lineDist, 
    screenSize.x/2f, screenSize.y/2f, -i * lineDist);

    // Line which goes to Z
    // Bottom
    for (float j = -screenSize.x/2f; j <  screenSize.x/2f ; j+= stepSize) {
      line(j, -screenSize.y/2f, -i * lineDist, 
      j, -screenSize.y/2f, -i * lineDist - lineDist);
    }

    // Top
    for (float j = -screenSize.x/2f; j <  screenSize.x/2f ; j+= stepSize) {
      line(j, screenSize.y/2f, -i * lineDist, 
      j, screenSize.y/2f, -i * lineDist - lineDist);
    }
  }

  stepSize = screenSize.y / numLines;
  // Vertical lines
  for (int i = 0; i <  numLines; i++) {
    stroke(255 - i * intensStep);

    // left
    line(-screenSize.x/2f, screenSize.y/2f, -i * lineDist, 
    -screenSize.x/2f, -screenSize.y/2f, -i * lineDist);

    // right
    line(screenSize.x/2f, -screenSize.y/2f, -i * lineDist, 
    screenSize.x/2f, screenSize.y/2f, -i * lineDist);

    // Line which goes to Z
    // left
    for (float j = -screenSize.y/2f; j <  screenSize.y/2f ; j+= stepSize) {
      line(-screenSize.x/2f, j, -i * lineDist, 
      -screenSize.x/2f, j, -i * lineDist - lineDist);
    }

    // right
    for (float j = -screenSize.y/2f; j <  screenSize.y/2f ; j+= stepSize) {
      line(screenSize.x/2f, j, -i * lineDist, 
      screenSize.x/2f, j, -i * lineDist - lineDist);
    }
  }
}

