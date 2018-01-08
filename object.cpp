#include <eigen3/Eigen/Geometry>
#include <fstream>
#include <QImage>
#include "object.h"
#include "utils.h"

using namespace Eigen;

Object::Object()
    : _geometry(0),
    _transformation(Matrix4f::Identity()),
    _renderType(RENDERTYPE_NORMAL)
{
}

void Object::attachMesh(Mesh* mesh)
{
    _geometry = mesh;
}

void Object::setRenderType(const RenderType& type)
{
    _renderType = type;
}

void Object::setScale(float s)
{
    cout << __FILE__ << " line " << __LINE__  << " : SCALING not implemented" << endl;
}

void Object::setRotation(const Affine3f& r)
{
    _transformation = _transformation*r;
}

void Object::setTranslation(const Translation3f &t)
{
    _transformation = t*_transformation;
}

void Object::draw(Shader* prog)
{
    if(!prog->valid())
        return;

    // active shader
    prog->activate();

    // send uniforms
    glUniformMatrix4fv(glGetUniformLocation(prog->id(),"mat_obj"),  1, GL_FALSE, _transformation.matrix().data());
    glUniform1i(glGetUniformLocation(prog->id(),"renderType"), _renderType);

    // draw mesh
    _geometry->draw_geometry(prog->id());
    GL_TEST_ERR;
}
