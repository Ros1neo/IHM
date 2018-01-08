#ifndef VBO_H
#define VBO_H

#include "OpenGL.h"
#include <eigen3/Eigen/Geometry>
#include <iostream>
#include <vector>

using namespace std;

class VBO
{
public:
    VBO(){bufferID = numeric_limits<unsigned int>::max();}
    virtual ~VBO()
    {
        release();
    }

    virtual void bind(int prg_id)=0;
    virtual void update(int start, int end)=0;

    virtual void disable()
    {
//        if(suppl_loc>=0) glDisableVertexAttribArray(suppl_loc);
    }

    virtual unsigned size()=0;

protected:
    virtual void init()=0;
    virtual void release()
    {
        if(bufferID != numeric_limits<unsigned int>::max())
            glDeleteBuffers(1,&bufferID);
    }

protected:
    unsigned int bufferID;
    int suppl_loc;
};


template<typename T>
class TypedVBO: public VBO
{
public:

    TypedVBO(std::vector<T> initial_values, std::string glsl_name);
    virtual void bind(int prg_id);
    virtual void update(int start, int end);

    virtual inline T get(unsigned idx);
    virtual inline void set(unsigned idx, T val);
    virtual inline void setValues(T val);
    virtual inline std::vector<T>& getValues();
    virtual unsigned size();
    virtual std::string& glsl_name();

protected:
    virtual void init();

protected:

    virtual GLenum getGLenum();
    virtual int getSize();

    std::vector<T> values;
    std::string name;
};

template<typename T>
TypedVBO<T>::TypedVBO(std::vector<T> initial_values, std::string glsl_name)
{
    values = initial_values;
    name = glsl_name;
}

template<typename T>
void TypedVBO<T>::init()
{
    glGenBuffers(1, &bufferID);GL_TEST_ERR;
    glBindBuffer(GL_ARRAY_BUFFER, bufferID);GL_TEST_ERR;

    glBufferData(GL_ARRAY_BUFFER,
                 values.size()*sizeof(T),
                 values.data(),
                 GL_DYNAMIC_DRAW);GL_TEST_ERR;
}

template<typename T>
void TypedVBO<T>::bind(int prg_id)
{
    if(bufferID == numeric_limits<unsigned int>::max()) init();
    glBindBuffer(GL_ARRAY_BUFFER, bufferID);
    suppl_loc = glGetAttribLocation(prg_id, name.c_str());
    if(suppl_loc >= 0)
    {
        glVertexAttribPointer(suppl_loc, getSize(), getGLenum(), GL_FALSE, 0, (void*)0);GL_TEST_ERR;
        glEnableVertexAttribArray(suppl_loc);GL_TEST_ERR;
    }
    glBindBuffer(GL_ARRAY_BUFFER,0);
    GL_TEST_ERR;
}

template<typename T>
void TypedVBO<T>::update(int start, int end)
{
    if(bufferID != numeric_limits<unsigned int>::max())
    {
        glBindBuffer(GL_ARRAY_BUFFER, bufferID);
        glBufferSubData(GL_ARRAY_BUFFER, start*sizeof(T), (end-start)*sizeof(T) , &values[start]);
    }
}

template<typename T>
T TypedVBO<T>::get(unsigned idx)
{
    return values[idx];
}

template<typename T>
void TypedVBO<T>::set(unsigned idx, T val)
{
    values[idx] = val;
}

template<typename T>
void TypedVBO<T>::setValues(T val)
{
    unsigned NofElements = values.size();
    values.clear();
    values = std::vector<T>(NofElements, val);
}

template<typename T>
std::vector<T>& TypedVBO<T>::getValues()
{
    return values;
}

template<typename T>
unsigned TypedVBO<T>::size()
{
    return values.size();
}

template<typename T>
std::string& TypedVBO<T>::glsl_name()
{
    return name;
}

template <typename T>
GLenum TypedVBO<T>::getGLenum()
{
    return GL_INVALID_ENUM;
}

template <typename T>
int TypedVBO<T>::getSize()
{
    return 0;
}


#endif // VBO_H
