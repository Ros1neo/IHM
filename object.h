#ifndef PGHP_OBJECT_H
#define PGHP_OBJECT_H

#include <map>

#include "mesh.h"
#include "shader.h"
#include "camera.h"
#include "utils.h"

using namespace Eigen;

class Object
{
public:
  Object();
  void attachMesh(Mesh *mesh);
  void setScale(float s);
  void setRotation(const Affine3f &r);
  void setTranslation(const Translation3f& t);
  void draw(Shader *prog);

  // type 0 = normal
  // type 1 = texture
  // type 2 = color
  // type 3 = by id
  void setRenderType(const RenderType &type);
  RenderType getRenderType() const { return _renderType; }

protected:
  Mesh* _geometry;
  RenderType _renderType;

public:
  Affine3f _transformation;
};

#endif //OBJECT_H
