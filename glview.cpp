#include <iostream>

#include <QKeyEvent>
#include <QImage>
#include <QMessageBox>
#include <eigen3/Eigen/Geometry>
#include <fstream>
#include <vector>
#include "OpenGL.h"
#include "glview.h"
#include "fbo.h"

#define VERTEX_SHADER   "../IOGS_RV/simple.vert"
#define FRAGMENT_SHADER "../IOGS_RV/simple.frag"

using namespace Eigen;

GLView::GLView(QWidget *parent)
    : QGLWidget(parent),
      _initGL(false)
{
}

GLView::~GLView()
{
}

void GLView::setScene(int scene)
{
    if(TP_ID == 1)
    {
        switch (scene) {
        case 1:
            _scene = Scene_TP1_1();
            break;
        case 2:
            _scene = Scene_TP1_2();
            break;
        case 3:
            _scene = Scene_TP1_3();
            break;
        case 4:
            _scene = Scene_TP1_4();
            break;
        case 5:
            _scene = Scene_TP1_5();
            break;
        }
    }
    else if(TP_ID == 2)
    {
        switch (scene) {
        case 1:
            _scene = Scene_TP2_1();
            break;
        case 2:
            _scene = Scene_TP2_2();
            break;
        case 3:
            _scene = Scene_TP2_3();
            break;
        }
    }
    else if(TP_ID == 3)
    {
        _scene = Scene_TP3_1();
    }
    updateGL();
}

void GLView::colorPicking(Vector2i mouse_coord)
{
    makeCurrent();

    // init fbo
    FBO* image = new FBO();
    image->init(_scene.camera()->vpWidth(),
                _scene.camera()->vpHeight(),
                GL_RGBA32F, GL_RGBA, GL_FLOAT);
    GL_TEST_ERR;

    // use fbo
    image->beginCapture();

    // init render
    glDisable(GL_BLEND);
    glClearColor(0.,0.,0.,1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glViewport(0, 0,
               _scene.camera()->vpWidth(),
               _scene.camera()->vpHeight());
    activeShaderObject();
    GL_TEST_ERR;

    // draw the scene
    _scene.draw(&_program, true);

    // texture reading
    glUseProgram(0); GL_TEST_ERR;
    image->endCapture(); GL_TEST_ERR;
    image->bindTexture();



    // TODO : retrieve color under mouse coords
    QImage img = Texture2D::currentFrame();
    QRgb color = img.pixel(mouse_coord.data()[0],mouse_coord.data()[1]);

    // TODO : retrieve object associated to the color
    if (qRed(color) == 0){
        if (qBlue(color)==255){
            _selected_id = 0;
        }
        else if (qGreen(color)==255){
            _selected_id = 1;
        }
    }
    else if (qGreen(color)==255){
        _selected_id = 2;
    }
    else
       {
        _selected_id = 3;
    }

    //Free memory
    delete image;
}

void GLView::initializeGL()
{
    if(!_initGL)
    {
        makeCurrent();
        std::cout << "Using OpenGL version: \"" << glGetString(GL_VERSION) << "\"" << std::endl;

        glEnable(GL_DEPTH_TEST);
        glClearColor(0.,0.,0.,0);

        // load shader
        loadShaders();

        // load texture
        QImage img;
        img.load(TEXTURE_WOOD);
        _texture.load(img);
        _selected_id = 0;
        _initGL = true;
    }
}

void GLView::resizeGL(int width, int height)
{
    _scene.camera()->setViewport(width, height);
}

void GLView::paintGL()
{
    if(!_program.valid())
        return;

    makeCurrent();

    /********** DRAW THE SCENE ***********/
    glClearColor(0.,0.,0.,0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glViewport(0, 0, _scene.camera()->vpWidth(), _scene.camera()->vpHeight());

    activeShaderObject();

    // draw scene
    _scene.draw(&_program);

    GL_TEST_ERR;
}

void GLView::activeShaderObject()
{
    _program.activate();

    // camera information
    glUniformMatrix4fv(glGetUniformLocation(_program.id(),"mat_view"), 1, GL_FALSE, _scene.camera()->viewMatrix().data());
    glUniformMatrix4fv(glGetUniformLocation(_program.id(),"mat_proj"), 1, GL_FALSE, _scene.camera()->projectionMatrix().data());
    GL_TEST_ERR;

    // texture
    glActiveTexture(GL_TEXTURE0);
    _texture.bind();
    glUniform1i(glGetUniformLocation(_program.id(), "texture"), 0);
    GL_TEST_ERR;
}

void GLView::loadShaders()
{
    _program.load_from_files(VERTEX_SHADER, FRAGMENT_SHADER);
}
