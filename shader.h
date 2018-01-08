
#ifndef _Shader_h_
#define _Shader_h_

#include <iostream>

#include <QString>

#include "OpenGL.h"



class Shader
{
public:
    Shader()
      : is_valid(false)
    {}

    bool load_from_files(const std::string& fileV, const std::string& fileF);

    bool load_sources(const std::string& vsrc, const std::string& fsrc);

    void activate() const;

    int get_uniform_location(const char* name) const;

    void set_sampler_unit(const char* samplerName, int textureUnit) const;
    int get_attrib_location(const char* name) const;

    int id() const { return program_id; }

    bool valid() const { return is_valid; }

    void dump_infos() const;
protected:

    bool is_valid;
    static void print_program_info_log(GLuint objectID);
    static void print_shader_info_log(GLuint objectID);
    GLuint program_id;
};

#endif // SHADER_H
