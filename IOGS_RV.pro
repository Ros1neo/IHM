#-------------------------------------------------
#
# Project created by QtCreator 2014-09-11T11:23:31
#
#-------------------------------------------------

QT      += core
QT      += gui
QT      += opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = IOGS_RV
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    vbo.cpp \
    shader.cpp \
    mesh.cpp \
    objformat.cpp \
    objutil.cpp \
    object.cpp \
    camera.cpp \
    glview.cpp \
    iomesh.cpp \
    fbo.cpp \
    scene.cpp \
    texture.cpp

HEADERS  += scene.h \
    mainwindow.h \
    vbo.h \
    OpenGL.h \
    shader.h \
    mesh.h \
    objformat.h \
    objutil.h \
    object.h \
    camera.h \
    glview.h \
    iomesh.h \
    fbo.h \
    texture.h \
    utils.h

INCLUDEPATH += /usr/include/c++/6

LIBS += -L/usr/lib
LIBS += -L/usr/local/lib
LIBS += -lGLU -lGL

FORMS    += mainwindow.ui

OTHER_FILES += \
    simple.frag \
    simple.vert
