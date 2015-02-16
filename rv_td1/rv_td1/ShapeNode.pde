class ShapeNode extends SceneNode {
  protected int id;

  public ShapeNode() {
    super();
    id = -1;
  }
  public ShapeNode(int _id){
    super();
    id = _id;
  }
  
  public void setId(int _id){
    id = _id;
  }
}

