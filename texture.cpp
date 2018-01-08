#include "texture.h"

#include <iostream>
#include <cassert>
#include <QImage>
#include "eigen3/Eigen/Core"

const int NUM_SAMPLES = 16384;

Texture::Texture()
{
    id = 0;
}

Texture::~Texture()
{
    release();
    assert(id == 0);
}


void Texture::release()
{
    if(id > 0)
    {
        glDeleteTextures(1, &id);
        id = 0;
    }
}

void Texture2D::load(const QImage& img)
{
    // Generate id
    assert(id == 0);
    glGenTextures(1, &id);

    // Bind texture and activate bilinear filtering
    glBindTexture(GL_TEXTURE_2D, id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    // Get image informations
    GLsizei w = img.width();
    GLsizei h = img.height();
    GLenum format = GL_BGRA;
    GLenum type = GL_UNSIGNED_BYTE;
    GLenum internalformat = GL_RGB8;

    // Upload texture
    glTexImage2D(GL_TEXTURE_2D, 0, internalformat, w, h, 0, format, type, img.mirrored().bits());
    GL_TEST_ERR;
}

void Texture2D::save(const string &filename)
{
    int width, height;
    glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &width);
    glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &height);

    uchar* pixels = new uchar[width* height*4];

    glGetTexImage(GL_TEXTURE_2D, 0, GL_BGRA, GL_UNSIGNED_BYTE, pixels);

    QImage img(pixels, width, height, QImage::Format_ARGB32);
    img = img.mirrored(false, true);

    img.save(QString(filename.c_str()));
    delete [] pixels;
}

QImage Texture2D::currentFrame()
{
    int width, height;
    glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_WIDTH, &width);
    glGetTexLevelParameteriv(GL_TEXTURE_2D, 0, GL_TEXTURE_HEIGHT, &height);

    uchar* pixels = new uchar[width* height*4];

    glGetTexImage(GL_TEXTURE_2D, 0, GL_BGRA, GL_UNSIGNED_BYTE, pixels);

    QImage img(pixels, width, height, QImage::Format_ARGB32);
    img = img.mirrored(false, true);
    delete [] pixels;
    return img;
}

void Texture2D::bind() const
{
    glBindTexture(GL_TEXTURE_2D, id);
}
