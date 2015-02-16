class MaterialNode{
  PImage[] tex;
  int uv_factor;

  MaterialNode(String[] filenames, int _uv_factor) {
    tex = new PImage[3];
    tex[0] = loadImage(filenames[0]);
    tex[1] = loadImage(filenames[1]);
    tex[2] = loadImage(filenames[2]);
    uv_factor = _uv_factor;
  }
}

