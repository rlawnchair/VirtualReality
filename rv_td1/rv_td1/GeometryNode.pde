class GeometryNode extends ShapeNode {
  Material material;
  //PMatrix3D transform;
  color c;
  PGraphics pg;
  //int id;

  GeometryNode(Material _m, /*PMatrix3D _t, int _id,*/ PGraphics _pg) {
    super();
    material = _m;
    //transform = _t; 
    c = color(random(255), random(255), random(255));
    pg = _pg;
    //id = _id;
  }

  // make the color change
  public void changeColor() {
    int r = (int)red(c);
    int g = (int)green(c);
    int b = (int)blue(c);
    c = color(r, 255 - g, b);
  }

  void render() {
    /*pushMatrix();
     applyMatrix(transform);
     */
    beginShape(QUADS);
    if (!colorPick)
      texture(material.tex[0]);
    else {
      pg.fill(c);
      fill(c);
      stroke(255);
    }
    // +Z "front" face
    vertex( 0, 0, 1, 0, 0);
    vertex( 1, 0, 1, material.uv_factor, 0);
    vertex( 1, 1, 1, material.uv_factor, material.uv_factor);
    vertex( 0, 1, 1, 0, material.uv_factor);

    // -Z "back" face
    vertex( 1, 0, 0, 0, 0);
    vertex( 0, 0, 0, material.uv_factor, 0);
    vertex( 0, 1, 0, material.uv_factor, material.uv_factor);
    vertex( 1, 1, 0, 0, material.uv_factor);

    // +X "right" face
    vertex( 1, 0, 1, 0, 0);
    vertex( 1, 0, 0, material.uv_factor, 0);
    vertex( 1, 1, 0, material.uv_factor, material.uv_factor);
    vertex( 1, 1, 1, 0, material.uv_factor);

    // -X "left" face
    vertex( 0, 0, 0, 0, 0);
    vertex( 0, 0, 1, material.uv_factor, 0);
    vertex( 0, 1, 1, material.uv_factor, material.uv_factor);
    vertex( 0, 1, 0, 0, material.uv_factor);
    endShape();


    // +Y "bottom" face
    beginShape(QUADS);
    if (!colorPick)
      texture(material.tex[1]);
    else {
      pg.fill(c);
      fill(c);
      stroke(255);
    }
    vertex( 0, 1, 1, 0, 0);
    vertex( 1, 1, 1, material.uv_factor, 0);
    vertex( 1, 1, 0, material.uv_factor, material.uv_factor);
    vertex( 0, 1, 0, 0, material.uv_factor);
    endShape();


    // -Y "top" face
    beginShape(QUADS);
    if (!colorPick)
      texture(material.tex[2]);
    else {
      pg.fill(c);
      fill(c);
      stroke(255);
    }
    vertex( 0, 0, 0, 0, 0);
    vertex( 1, 0, 0, material.uv_factor, 0);
    vertex( 1, 0, 1, material.uv_factor, material.uv_factor);
    vertex( 0, 0, 1, 0, material.uv_factor);
    endShape();

    //popMatrix();
  }

  // id 0 gives color -2, etc.
  color getColor(int id) {
    return -(id + 2);
  }

  // color -2 gives 0, etc.
  int getId(color c) {
    return -(c + 2);
  }
}

