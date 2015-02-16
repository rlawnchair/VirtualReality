class SceneNode {
  ArrayList<SceneNode> children;
  boolean visible;
  ArrayList<NodeModifier> modifiers;
  String name;

  SceneNode()
  {
    children = new ArrayList<SceneNode>();
    modifiers = new ArrayList<NodeModifier>();
    visible = true;
    name = this.getClass() + "_" + millis();
  }

  // returns this SceneNode for chaining together
  SceneNode addChild(SceneNode child)
  {
    if (!children.contains(child))
      children.add(child);
    else
    {
      // remove it, and add it back on the end
      children.remove(child);
      children.add(child);
    }

    return this;
  }


  void update(Object o)
  {
    for (SceneNode child : children)
    {
      child.update(o);
    }
  }


  void render()
  {
    // this could be done in a separate object as well
    for (SceneNode child : children)
    {
      child.render();
    }
  }

  void toXML(){
    x3d = loadXML("data/data.x3d");
    XML temp = x3d.addChild(this.name);
    for (SceneNode child : children)
    {
      child.toXML();
    }
  }
  // returns this SceneNode for chaining together
  SceneNode removeChild(SceneNode child)
  {
    if (children.contains(child))
    {
      children.remove(child);
    }

    return this;
  }

  boolean containsChild(SceneNode child)
  {
    return children.contains(child);
  }


  // returns this SceneNode for chaining together
  SceneNode addModifier(NodeModifier modifier)
  {
    if (!modifiers.contains(modifier))
      modifiers.add(modifier);
    else
    {
      // remove it, and add it back on the end
      modifiers.remove(modifier);
      modifiers.add(modifier);
    }

    return this;
  }

  // returns this SceneNode for chaining together
  SceneNode removeModifier(NodeModifier modifier)
  {
    if (modifiers.contains(modifier))
    {
      modifiers.remove(modifier);
    }

    return this;
  }

  boolean containsModifier(NodeModifier modifier)
  {
    return modifiers.contains(modifier);
  }

  boolean hasModifiers()
  {
    boolean val = false;

    if (modifiers != null && modifiers.size() > 0) val = true;

    return val;
  }
}

