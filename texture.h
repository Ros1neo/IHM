#ifndef TEXTURE_H
#define TEXTURE_H

#include "OpenGL.h"
#include <string>

using namespace std;

class Texture
{
public:
    Texture();
    virtual ~Texture();

    //Free OpenGL memory
    virtual void release();

    virtual void bind() const = 0;

protected:
    GLuint id;
};

class QImage;

class Texture2D : public Texture
{
public:
    void load(const QImage& img);
    static void save(const std::string& filename);
    static QImage currentFrame();

    virtual void bind() const;
};

#endif // TEXTURE_H
