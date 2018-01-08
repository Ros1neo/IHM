#ifndef FBO_H
#define FBO_H

#include "OpenGL.h"

class FBO
{
public:
    FBO();
    ~FBO();

    // Create texture and renderBuffer
    void init(int w, int h, int internalformat);
    void init(int w, int h, int internalformat, int format, int type);
    void resize(int w, int h);

    //Free Graphic Memory
    void release();

    //Use texture
    void bindTexture();

    //Rendering in FBO
    void beginCapture();
    void endCapture();

private :
    GLuint fbo;
    GLuint renderBuffer;
    GLuint texture;

    GLint _internalformat;
    GLint _format;
    GLint _type;

    int width, height;
};

#endif // FBO_H
