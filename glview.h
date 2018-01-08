#ifndef GLVIEW_H
#define GLVIEW_H

#include <string>
#include <vector>
#include <list>
#include <QGLWidget>
#include <QApplication>
#include <QTimer>

#include "shader.h"
#include "mesh.h"
#include "camera.h"
#include "object.h"
#include "scene.h"
#include "texture.h"

using namespace std;
using namespace Eigen;

class GLView : public QGLWidget
{
    Q_OBJECT

public:
    EIGEN_MAKE_ALIGNED_OPERATOR_NEW
    GLView(QWidget *parent = 0);
    ~GLView();

    // set the scene id
    void setScene(int scene);

    // return camera
    Camera* camera() { return _scene.camera(); }

    // return scene
    Scene* scene() { return &_scene; }

    void colorPicking(Vector2i mouse_coord);
    int _selected_id;

protected:
    virtual void initializeGL();
    virtual void resizeGL(int width, int height);
    virtual void paintGL();
    void activeShaderObject();
    void loadShaders();


private:
    bool _initGL;
    Scene _scene;
    Texture2D _texture;
    Shader _program;
};


#endif // GLVIEW_H
