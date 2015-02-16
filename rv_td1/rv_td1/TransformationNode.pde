class TransformationNode extends SceneNode {
  private float rotX, rotY, rotZ, scale, translateX, translateY, translateZ;
  public PMatrix3D m = new PMatrix3D();
  public TransformationNode() {
    super();
  }

  public TransformationNode(PMatrix3D _m) {
    super();
    m = _m;
  }

  public void render() {
    pushMatrix();
    applyMatrix(m);
    for (SceneNode child : children) {
      child.render();
    }
    popMatrix();
  }

  public void update(Object o) {
    m = (PMatrix3D)o;
  }
}

