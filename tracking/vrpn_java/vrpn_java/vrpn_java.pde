import processing.opengl.*;
import java.util.Vector;
import vrpn.*;


TrackerRemote m_tracker = null;
TrackerRemoteListener m_trackerListener = null;


void setup() {
  size(640, 480, P3D); 

  //On crée les classes pour accéder au tracker et button vrpn 
  try { 
    m_tracker = new TrackerRemote( "Tracker@localhost", null, null, null, null );
  }
  catch( InstantiationException e ) {
    println("Error when creating the vrpn Tracker and Button devices");
  }
  m_trackerListener = new TrackerRemoteListener(m_tracker);
}

void draw() {
  background(255);
  noFill();
  stroke(0, 0, 255);

  Vector updates = m_trackerListener.getTrackerUpdate();
  if (updates != null) {
    for (int i=0; i<updates.size(); i++) {
      vrpn.TrackerRemote.TrackerUpdate u = (vrpn.TrackerRemote.TrackerUpdate)updates.elementAt(i);
      //ellipse((float)u.pos[0],(float)u.pos[1],(float)u.pos[2],(float)u.pos[2]);noStroke();
      noStroke();
      lights();
      translate((float)u.pos[0],(float)u.pos[1], 0.0);
      fill(128);
      sphere(100);
      println(" sensor : "+u.sensor+" pos: "+u.pos[0]+","+u.pos[1] +","+u.pos[2]);
    }
  }
}

