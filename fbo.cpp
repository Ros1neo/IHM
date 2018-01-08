#include "fbo.h"
#include <iostream>

FBO::FBO() : fbo(0), renderBuffer(0), texture(0){}

FBO::~FBO(){release();}

GLenum buffers[] = {GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1};

inline void checkFrameBufferStatus()

{
    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    switch(status)
    {
    case GL_FRAMEBUFFER_COMPLETE:
        break;
    default:
        std::cerr << "FBO creation error, " << status << std::endl;
    }
}

void FBO::init(int w, int h, int internalformat)
{
    init(w,h,internalformat,GL_RGBA, GL_FLOAT);
}

void FBO::init(int w, int h, int internalformat, int format, int type)
{
    //Size
    width = w;
    height = h;

    _internalformat = internalformat;
    _format = format;
    _type = type;

    //Initialize texture
    glGenTextures(1,&texture);
    glBindTexture(GL_TEXTURE_2D,texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexImage2D(GL_TEXTURE_2D, 0, _internalformat, width, height, 0,_format, _type, 0);
    glBindTexture(GL_TEXTURE_2D,0);

    //Initialize renderBuffer
    glGenRenderbuffers(1, &renderBuffer);
    glBindRenderbuffer(GL_RENDERBUFFER,renderBuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
    glBindRenderbuffer(GL_RENDERBUFFER,0);


    //Initialize Frame Buffer Object
    glGenFramebuffers(1, &fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
    glFramebufferTexture(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, texture,0);
    //    glFramebufferTexture2D( GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D_MULTISAMPLE, texture, 0 );
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, renderBuffer);
    glDrawBuffers(2,buffers);

    // check for errors
    checkFrameBufferStatus();
    glBindFramebuffer(GL_FRAMEBUFFER,0);
    GL_TEST_ERR;
}

void FBO::resize(int w, int h)
{
    width = w;
    height = h;

    glBindTexture(GL_TEXTURE_2D,texture);
    glTexImage2D(GL_TEXTURE_2D, 0, _internalformat, width, height, 0,_format, _type, NULL);
    glBindTexture(GL_TEXTURE_2D,0);

    glBindRenderbuffer(GL_RENDERBUFFER,renderBuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
    glBindRenderbuffer(GL_RENDERBUFFER,0);
}

void FBO::release()
{
    if (renderBuffer != 0)
    {
        glDeleteRenderbuffers(1, &renderBuffer);
        renderBuffer = 0;
    }
    if (texture != 0)
    {
        glDeleteTextures(1, &texture);
        texture = 0;
    }
    if (fbo != 0)
    {
        glDeleteFramebuffers(1, &fbo);
        fbo = 0;
    }
}

void FBO::bindTexture()
{
    glBindTexture(GL_TEXTURE_2D, texture);
}

void FBO::beginCapture()
{
    glEnable(GL_MULTISAMPLE);
    glBindFramebuffer(GL_FRAMEBUFFER, fbo);
}

void FBO::endCapture()
{
    glDisable(GL_MULTISAMPLE);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
