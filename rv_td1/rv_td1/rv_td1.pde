Camera cam;

//ArrayList<Cube> cubes;

// Permet de gérer les rebonds
float stomp = 0.;
boolean rebond = true;
boolean up = false;
boolean down = true;

// Permet de gérer les mouvement de caméra
boolean move_front = false;
boolean move_back = false;
boolean straf_left = false;
boolean straf_right = false;

// Colorpicking
boolean colorPick = false;
PGraphics pg;

int id = -1;

// Groupes
SceneNode nodesScene;
SceneNode nodesSand;
SceneNode nodesBrick;
SceneNode nodesBrickMossy;
SceneNode nodesTnt;

// XML
String filenameX3D = "data/test.x3d";
XML x3d;

// Matrice de transformation du rebond du bloc de TnT
PMatrix3D mRebond = new PMatrix3D();

void setup() {
  randomSeed(0);
  int id_cube = 0;
  size(1024, 768, P3D);
  pg = createGraphics(width, height, P3D); // Utilisé pour le color picking
  textureMode(NORMAL);
  textureWrap(REPEAT);

  textMode(SHAPE);

  cam = new Camera();

  // Create cubes and their materials
  //cubes = new ArrayList<Cube>();
  nodesScene = new SceneNode();
  nodesSand = new SceneNode();
  nodesBrick = new SceneNode();
  nodesBrickMossy = new SceneNode();
  nodesTnt = new SceneNode();
  //nodes = new TransformationNode();

  String[] filenames_sand = { 
    "sand.png", "sand.png", "sand.png"
  };
  Material sand = new Material(filenames_sand, 1);
  String[] filenames_stonebrick = { 
    "stonebrick.png", "stonebrick.png", "stonebrick.png"
  };
  Material stonebrick = new Material(filenames_stonebrick, 2);
  String[] filenames_stonebrick_mossy = { 
    "stonebrick_mossy.png", "stonebrick_mossy.png", "stonebrick_mossy.png"
  };
  Material stonebrick_mossy = new Material(filenames_stonebrick_mossy, 2);

  // Creation et positionnement de blocs
  for (int i=0; i < 2; i++) {
    for (int j=-10; j < 10; j++) {
      for (int k=-10; k < 10; k++) {
        PMatrix3D m = new PMatrix3D();
        m.translate(j, i+1, k);
        if (i==1) {
          TransformationNode sandTrans = new TransformationNode(m);
          nodesSand.addChild(sandTrans.addChild(new ShapeNode(id_cube++).addChild(new GeometryNode(sand, pg))));
        }
        else {
          float r = random(10);
          if ((int)r != 0) {
            TransformationNode stonebrickTrans = new TransformationNode(m);
            nodesBrick.addChild(stonebrickTrans.addChild(new ShapeNode(id_cube++).addChild(new GeometryNode(stonebrick, pg))));
          }
          else {
            TransformationNode stonebrick_mossyTrans = new TransformationNode(m);
            nodesBrickMossy.addChild(stonebrick_mossyTrans.addChild(new ShapeNode(id_cube++).addChild(new GeometryNode(stonebrick_mossy, pg))));
          }
        }
      }
    }
  }

  String[] filenames_tnt = { 
    "tnt_side.png", "tnt_bottom.png", "tnt_top.png"
  };
  Material tnt = new Material(filenames_tnt, 1);
  PMatrix3D m = new PMatrix3D();
  m.translate(2, 0, -1);
  TransformationNode tntTrans = new TransformationNode(m);
  nodesTnt.addChild(tntTrans.addChild(new ShapeNode(id_cube++).addChild(new GeometryNode(tnt, pg))));

  nodesScene.addChild(nodesSand);
  nodesScene.addChild(nodesBrick);
  nodesScene.addChild(nodesBrickMossy);
  nodesScene.addChild(nodesTnt);
}

void draw() {
  if (colorPick) {
    pg.beginDraw();
    pg.background(0);
    drawScene(false);
    pg.endDraw();
  }
  // Clean screen and draw geometry
  background(0);
  drawScene(false);
  cam.display();
}

void drawScene(boolean id_mode) {
  cam.applyMatrix();

  drawAxes();

  scale(90);
  noStroke();
  // Rebond Tnt

  if (rebond) {
    if (down) {
      stomp++;
      //PMatrix3D m = new PMatrix3D();
      mRebond.translate(0, -0.1, 0);
      nodesTnt.update(mRebond);
      //nodes.get(nodes.size()-1).transform.translate(0, -0.1, 0);
    }
    if (up) {
      stomp--;
      //PMatrix3D m = new PMatrix3D();
      mRebond.translate(0, 0.1, 0);
      nodesTnt.update(mRebond);
      //nodes.get(nodes.size()-1).transform.translate(0, 0.1, 0);
    }
    if (stomp == 20) {
      up = true;
      down = false;
    }
    if (stomp == 0) {
      up = false;
      down = true;
    }
  }
  else
  {
    //PMatrix3D m = new PMatrix3D();
    mRebond.translate(0, 0, 0);
    nodesTnt.update(mRebond);
    //nodes.get(nodes.size()-1).transform.translate(0, 0, 0);
  }

  /*******************************************************/
  /*pg.beginDraw();
   pg.background(0);
   */
  // Draw cubes
  /*for (int i=0; i<nodes.size(); i++) {
   nodes.get(i).render();
   }*/
  nodesScene.render();
  //pg.endDraw();
}

void drawAxes() {
  // 3D axes
  strokeWeight(1);
  textSize(20);
  stroke(255, 0, 0);
  line(0, 0, 0, 20, 0, 0);
  fill(255, 0, 0);
  text('x', 25, 0, 0);
  stroke(0, 255, 0);
  line(0, 0, 0, 0, 20, 0);
  fill(0, 255, 0);
  text('y', 5, 25, 0);
  stroke(0, 0, 255);
  line(0, 0, 0, 0, 0, 20);
  fill(0, 0, 255);
  text('z', -5, -5, 22);
}

void mouseMoved() {
  cam.update(mouseX, mouseY, false, false, false, false);
}


void keyPressed() {
  if (key == 'z')
    move_front = true;
  if (key == 's')
    move_back = true;
  if (key == 'q')
    straf_left = true;
  if (key == 'd')
    straf_right = true;
  cam.update(mouseX, mouseY, move_front, move_back, straf_left, straf_right);
  if (key == 'a') {
    cam.move = !cam.move;
  }
  if (key == 'c') {
    colorPick = !colorPick;
  }
  if (key == 'x') {
    println("Saving to x3d");
    saveX3D(filenameX3D);
  }
  if (key == 'l') {
    println("Loading x3d");
    loadX3D(filenameX3D);
  }
}

void keyReleased() {
  if (key == 'z')
    move_front = false;
  if (key == 's')
    move_back = false;
  if (key == 'q')
    straf_left = false;
  if (key == 'd')
    straf_right = false;
  cam.update(mouseX, mouseY, move_front, move_back, straf_left, straf_right);
}

void saveX3D(String filenameX3D) {
  /*x3d = loadXML(filenameX3D);
   XML scene = x3d.addChild("Scene");
   XML transformation = scene.addChild("Transform");*/
  nodesScene.toXML();
  saveXML(x3d, "data/data.x3d");
}

void loadX3D(String filenameX3D) {
  x3d = loadXML(filenameX3D);

  SceneNode[] nodesSceneArray;
  XML[] scene = x3d.getChildren("Scene");
  nodesSceneArray = new SceneNode[scene.length];

  for (int i = 0; i < nodesSceneArray.length; i++) {
    println("Node Scene");

    TransformationNode[] tN;
    XML[] transform = scene[i].getChildren("Transform");
    tN = new TransformationNode[transform.length];

    for (int j = 0; j < tN.length; j++) {
      println("Node Transform");
      String translation = transform[j].getString("translation");
      float[] xyz = float(split(translation, ' '));
      PMatrix3D m = new PMatrix3D();
      m.translate(xyz[0], xyz[1], xyz[2]);
      tN[j] = new TransformationNode(m);
      nodesSceneArray[i].addChild(tN[j]);
    }
    //nodesSceneArray[i].render();
  }
}

