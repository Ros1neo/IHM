#include "scene.h"

Camera Scene::_camera;

void Scene::draw(Shader *prog, bool colorPicking)
{
    // draw frame
    glDisable(GL_BLEND);
    if(!colorPicking)
        _frame.draw(prog);

    // draw objects
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    for(uint i=0 ; i<_objs.size() ; ++i)
    {
        // backup render type
        RenderType renderType = _objs[i].getRenderType();

        // color picking : set render type as color by id
        if(colorPicking)
            _objs[i].setRenderType(RENDERTYPE_ID);

        // set the objet id
        glUniform1f(glGetUniformLocation(prog->id(), "objectId"), i/(float)(_objs.size()-1));

        // draw the objet
        _objs[i].draw(prog);

        // restore the render type
        _objs[i].setRenderType(renderType); // restore render type
    }
}

Object *Scene::object(uint i)
{
    assert(i < _objs.size());
    return &_objs[i];
}

Object Scene::openObject(const string path)
{
    Mesh *mesh = new Mesh(path);
    Object obj;
    obj.attachMesh(mesh);
    return obj;
}

void Scene::init()
{
    // init camera
    _camera.setFovY(M_PI/3.);
    _camera.lookAt(Vector3f(0, 0.01, 1).normalized()*2, Vector3f(0, 0, 0), Vector3f::UnitY());

    // init frame
    _frame = openObject(OBJECT_FRAME);
    _frame.setRenderType(RENDERTYPE_COLOR);
}
