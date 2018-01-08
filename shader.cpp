// Copyright (C) 2007  Gael Guennebaud

#include <iostream>
#include <string>
#include <fstream>
#include <assert.h>

#include "shader.h"

using namespace std;

string loadSourceFromFile(const string& filename)
{
    string source = "";

    ifstream in(filename.c_str(),std::ios::in);
    if(!in)
    {
        cerr << "File not found " << filename << endl;
        return source;
    }
    const int maxBuffersize = 2048;
    char buffer[maxBuffersize];
    while(in.getline(buffer, maxBuffersize))
    {
        source += string(buffer) + "\n";
    }

    return source;
}

//--------------------------------------------------------------------------------
bool Shader::load_from_files(const string& fileV, const string& fileF)
{
    string vsrc = loadSourceFromFile(fileV);
    string fsrc = loadSourceFromFile(fileF);
    return load_sources(vsrc,fsrc);
}
//--------------------------------------------------------------------------------
bool Shader::load_sources(const string& vsrc, const string& fsrc)
{
    bool allIsOk = true;

    program_id = glCreateProgram();

    // vertex shader
    {
        GLuint shaderID = glCreateShader(GL_VERTEX_SHADER);

        //QByteArray vsrc8 = vsrc.toAscii();
        const GLchar * arbSource = vsrc.c_str();

        glShaderSource(shaderID, 1, (const GLchar **)&arbSource, 0);
        glCompileShader(shaderID);

        int compiled;
        glGetShaderiv(shaderID,GL_COMPILE_STATUS,&compiled);
        allIsOk = allIsOk && compiled;
        print_shader_info_log(shaderID);

        if(compiled)
            glAttachShader(program_id, shaderID);
    }

    // fragment shader
    {
        GLuint shaderID = glCreateShader(GL_FRAGMENT_SHADER);

        //QByteArray fsrc8 = fsrc.toAscii();
        const GLchar * arbSource = fsrc.c_str();

        glShaderSource(shaderID, 1, (const GLchar **)&arbSource, 0);
        glCompileShader(shaderID);
        int compiled;
        glGetShaderiv(shaderID,GL_COMPILE_STATUS,&compiled);

        allIsOk = allIsOk && compiled;
        print_shader_info_log(shaderID);

        if(compiled)
            glAttachShader(program_id, shaderID);
    }

    glLinkProgram(program_id);

    int isLinked;
    glGetProgramiv(program_id, GL_LINK_STATUS, &isLinked);
    allIsOk = allIsOk && isLinked;
    is_valid = isLinked == GL_TRUE;
    print_program_info_log(program_id);

    return allIsOk;
}
//--------------------------------------------------------------------------------
void Shader::activate(void) const
{
    assert(is_valid);
    glUseProgram(program_id);
}
//--------------------------------------------------------------------------------
int Shader::get_uniform_location(const char* name) const
{
    assert(is_valid);
    return glGetUniformLocation(program_id, name);
}
//--------------------------------------------------------------------------------
void Shader::set_sampler_unit(const char* sampler, int unit) const
{
    activate();
    glUniform1i(get_uniform_location(sampler), unit);
}
//--------------------------------------------------------------------------------
int Shader::get_attrib_location(const char* name) const
{
    assert(is_valid);
    return glGetAttribLocation(program_id, name);
}
//--------------------------------------------------------------------------------
void Shader::dump_infos() const {
    GLint nbAttrs;
    glGetProgramiv(program_id, GL_ACTIVE_ATTRIBUTES, &nbAttrs);

    GLchar name[128];
    GLint size;
    GLenum type;
    for(GLint i=0; i<nbAttrs; ++i) {
        glGetActiveAttrib(program_id, i, 128, NULL, &size, &type, name);
        std::cout << "Attrib " << glGetAttribLocation(program_id, name) << ": " << name << ", " << size << ", " << type << "\n";
    }
}
//--------------------------------------------------------------------------------
void Shader::print_program_info_log(GLuint objectID)
{
    int infologLength = 0, charsWritten = 0;
    GLchar *infoLog;
    glGetProgramiv(objectID,GL_INFO_LOG_LENGTH, &infologLength);
    if(infologLength > 0)
    {
        infoLog = new GLchar[infologLength];
        glGetProgramInfoLog(objectID, infologLength, &charsWritten, infoLog);
        if (charsWritten>0)
            cerr << "program info : \n" << infoLog << endl;
        delete[] infoLog;
    }
    else
    {
        cerr << "Shader linking: success" << endl;
    }
}
//--------------------------------------------------------------------------------
void Shader::print_shader_info_log(GLuint objectID)
{
    int infologLength = 0, charsWritten = 0;
    GLchar *infoLog;
    glGetShaderiv(objectID,GL_INFO_LOG_LENGTH, &infologLength);
    if(infologLength > 0)
    {
        infoLog = new GLchar[infologLength];
        glGetShaderInfoLog(objectID, infologLength, &charsWritten, infoLog);
        if (charsWritten>0)
            cerr << "Shader info : \n" << infoLog << endl;
        delete[] infoLog;
    }
    else
    {
        cerr << "Shader compilation: success" << endl;
    }
}
