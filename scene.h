#ifndef SCENE_H
#define SCENE_H

#include <string>
#include "object.h"
#include "mesh.h"
#include "utils.h"

using namespace std;

class Scene
{
public:

    // return camera
    Camera* camera() { return &_camera; }

    // draw the scene
    void draw(Shader* prog, bool colorPicking=false);

    // return number of object
    uint n_object() { return _objs.size(); }

    // return the i-th object
    Object* object(uint i);

protected:
    Object openObject(const string path);
    virtual void init();

public:
    vector<Object> _objs;

protected:
    Object _frame;
    static Camera _camera;
};



/******** SCENE TP1 1 ********/
class Scene_TP1_1 : public Scene
{
public:
    Scene_TP1_1()
    {
        init();
    }

private:
    virtual void init()
    {
        // init parent
        Scene::init();

        // object
        Object obj = openObject(OBJECT_BOX);
        obj.setRenderType(RENDERTYPE_TEXTURE);
        _objs.push_back(obj);
    }
};

/******** SCENE TP1 2 ********/
class Scene_TP1_2 : public Scene
{
public:
    Scene_TP1_2()
    {
        init();
    }

private:
    virtual void init()
    {
        // init parent
        Scene::init();

        // camera
        _camera.setFovY(M_PI/3.);
        _camera.lookAt(Vector3f(1, 1, 1).normalized()*2, Vector3f(0, 0, 0), Vector3f::UnitY());

        // object
        Object obj = openObject(OBJECT_BOX);
        obj.setRenderType(RENDERTYPE_TEXTURE);
        _objs.push_back(obj);
    }
};

/******** SCENE TP1 3 ********/
class Scene_TP1_3 : public Scene
{
public:
    Scene_TP1_3()
    {
        init();
    }

private:
    virtual void init()
    {
        // init parent
        Scene::init();

        // object
        Object obj = openObject(OBJECT_BOX);
        obj.setRenderType(RENDERTYPE_TEXTURE);
        obj.setTranslation(Translation3f(1, 0, 0));
        obj.setRotation(Affine3f(AngleAxisf(-45*M_PI/180., Vector3f::UnitY())));
        _objs.push_back(obj);
    }
};

/******** SCENE TP1 4 ********/
class Scene_TP1_4 : public Scene
{
public:
    Scene_TP1_4()
    {
        init();
    }

private:
    virtual void init()
    {
        // init parent
        Scene::init();

        // object
        Object obj = openObject(OBJECT_BOX);
        obj.setRenderType(RENDERTYPE_TEXTURE);
        obj.setTranslation(Translation3f(0.2, 0, 0));
        obj.setRotation(Affine3f(AngleAxisf(-12*M_PI/180., Vector3f::UnitY())));
        obj.setTranslation(Translation3f(0.9, 0, 0));
        obj.setRotation(Affine3f(AngleAxisf(-41*M_PI/180., Vector3f::UnitY())));
        obj.setTranslation(Translation3f(-0.1, 0, 0));
        obj.setRotation(Affine3f(AngleAxisf(8*M_PI/180., Vector3f::UnitY())));
        _objs.push_back(obj);
    }
};

/******** SCENE TP1 5 ********/
class Scene_TP1_5 : public Scene
{
public:
    Scene_TP1_5()
    {
        init();
    }

private:
    virtual void init()
    {
        // init parent
        Scene::init();
        _camera.setFovY(M_PI/3.);
        _camera.lookAt(Vector3f(0, 1, 0.01).normalized()*2, Vector3f(0, 0, 0), Vector3f::UnitY());

        // object
        Object obj = openObject(OBJECT_BOX);
        obj.setRenderType(RENDERTYPE_TEXTURE);
        obj.setTranslation(Translation3f(1, 0, 0));
        obj.setRotation(Affine3f(AngleAxisf(-45*M_PI/180., Vector3f::UnitY())));
        _objs.push_back(obj);
    }
};



/******** SCENE TP2 1 ********/
class Scene_TP2_1 : public Scene
{
public:
    Scene_TP2_1()
        : Scene()
    {
        init();
    }

private:
    virtual void init()
    {
        // init parent
        Scene::init();

        // object
        Object obj = openObject(OBJECT_BOX);
        obj.setRenderType(RENDERTYPE_TEXTURE);
        _objs.push_back(obj);
    }
};

/******** SCENE TP2 2 ********/
class Scene_TP2_2 : public Scene
{
public:
    Scene_TP2_2()
        : Scene()
    {
        init();
    }

private:
    virtual void init()
    {
        // init parent
        Scene::init();

        // object
        Object obj = openObject(OBJECT_BOX);
        obj.setRenderType(RENDERTYPE_ID);
        _objs.push_back(obj);
    }
};

/******** SCENE TP2 3 ********/
class Scene_TP2_3 : public Scene
{
public:
    Scene_TP2_3()
        : Scene()
    {
        init();
    }

private:
    virtual void init()
    {
        // init parent
        Scene::init();
        _camera.lookAt(Vector3f(0, 0.01, 1).normalized()*5, Vector3f(0, 0, 0), Vector3f::UnitY());

        // object 1
        Object obj1 = openObject(OBJECT_BOX);
        obj1.setRenderType(RENDERTYPE_ID);
        obj1.setTranslation(Translation3f(1, -1, 0));
        _objs.push_back(obj1);

        // object 2
        Object obj2 = openObject(OBJECT_BOX);
        obj2.setRenderType(RENDERTYPE_ID);
        obj2.setTranslation(Translation3f(-1, -1, 0));
        _objs.push_back(obj2);

        // object 3
        Object obj3 = openObject(OBJECT_BOX);
        obj3.setRenderType(RENDERTYPE_ID);
        obj3.setTranslation(Translation3f(1, 1, 0));
        _objs.push_back(obj3);

        // object 4
        Object obj4 = openObject(OBJECT_BOX);
        obj4.setRenderType(RENDERTYPE_ID);
        obj4.setTranslation(Translation3f(-1, 1, 0));
        _objs.push_back(obj4);
    }
};


/******** SCENE TP3 1 ********/
class Scene_TP3_1 : public Scene
{
public:
    Scene_TP3_1()
        : Scene()
    {
        init();
    }

private:
    virtual void init()
    {
        // init parent
        Scene::init();

        // objects
        for(int z=-10 ; z<=10 ; z+=3)
            for(int y=-10 ; y<=10 ; y+=3)
                for(int x=-10 ; x<=10 ; x+=3)
                {
                    Object obj = openObject(OBJECT_BOX);
                    obj.setTranslation(Translation3f(x, y, z));
                    obj.setRenderType(RENDERTYPE_TEXTURE);
                    _objs.push_back(obj);
                }
    }
};

#endif // SCENE_H
