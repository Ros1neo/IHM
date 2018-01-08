#include "vbo.h"

template <>
GLenum TypedVBO<int>::getGLenum()
{
    return GL_INT;
}

template <>
int TypedVBO<int>::getSize()
{
    return 1;
}

template <>
GLenum TypedVBO<float>::getGLenum()
{
    return GL_FLOAT;
}

template <>
int TypedVBO<float>::getSize()
{
    return 1;
}

template <>
GLenum TypedVBO<double>::getGLenum()
{
    return GL_DOUBLE;
}

template <>
int TypedVBO<double>::getSize()
{
    return 1;
}


template <>
GLenum TypedVBO<Eigen::Vector3f>::getGLenum()
{
    return GL_FLOAT;
}

template <>
int TypedVBO<Eigen::Vector3f>::getSize()
{
    return 3;
}


template <>
GLenum TypedVBO<Eigen::Vector3d>::getGLenum()
{
    return GL_DOUBLE;
}

template <>
int TypedVBO<Eigen::Vector3d>::getSize()
{
    return 3;
}

template <>
GLenum TypedVBO<Eigen::Vector2f>::getGLenum()
{
    return GL_FLOAT;
}

template <>
int TypedVBO<Eigen::Vector2f>::getSize()
{
    return 2;
}
