#ifndef SHADER_H
#define SHADER_H

#include "glad/glad.h"
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>


class Shader
{
    private:
        auto get_uni(const std::string &uni_name) {  return glGetUniformLocation(shader_program, uni_name.c_str());  }
    public:
        unsigned int shader_program;

        void use() {    glUseProgram(shader_program);   }

        void set_uniform(const std::string &uni_name, const float (&values)[3]) {   glUniform3fv(get_uni(uni_name), 3, values);  }

        void set_uniform(const std::string &uni_name, const float value) {   glUniform1f(get_uni(uni_name), value);  }

        void set_uniform(const std::string &uni_name, const int value) {   glUniform1i(get_uni(uni_name), value);  }

        Shader(const std::string&,const std::string&);
        ~Shader() {   glDeleteProgram(shader_program);    }
};

Shader::Shader(const std::string &VERTEX_SHADER_PATH, const std::string &FRAGMENT_SHADER_PATH)
{
    std::string vertex_shader_source, fragment_shader_source;

    std::ifstream vertex_source_finder, fragment_source_finder;
    vertex_source_finder.exceptions(std::ios::failbit | std::ios::badbit);
    fragment_source_finder.exceptions(std::ios::failbit | std::ios::badbit);

    // Opens path and inserts data into string streams. If fails to open, shows error message.
    try
    {
        vertex_source_finder.open(VERTEX_SHADER_PATH);
        fragment_source_finder.open(FRAGMENT_SHADER_PATH);

        std::stringstream sstream_vertex, sstream_fragment;
        sstream_vertex << vertex_source_finder.rdbuf();
        sstream_fragment << fragment_source_finder.rdbuf();

        vertex_source_finder.close();
        fragment_source_finder.close();

        vertex_shader_source = sstream_vertex.str();
        fragment_shader_source = sstream_fragment.str();
    }
    catch (std::ifstream::failure e)
    {
        std::cout << "\nERROR::Could not find shader paths.\n";
    }
    int success;
    char info_log[512];

    // Vertex shader
    unsigned int vertex_shader = glCreateShader(GL_VERTEX_SHADER);

    const char *C_VERTEX_SHADER_SOURCE = vertex_shader_source.c_str();
    glShaderSource(vertex_shader,1,&C_VERTEX_SHADER_SOURCE,nullptr);
    glCompileShader(vertex_shader);

    // Error message if compilation goes wrong.
    glGetShaderiv(vertex_shader,GL_COMPILE_STATUS,&success);
    if (!success)
    {
        glGetShaderInfoLog(vertex_shader,512,nullptr, info_log);
        std::cout << "\nVERTEX SHADER COMPILE ERROR::" << info_log;
    }

    // Fragment shader
    unsigned int fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);

    const char *C_FRAGMENT_SHADER_SOURCE = fragment_shader_source.c_str();
    glShaderSource(fragment_shader,1,&C_FRAGMENT_SHADER_SOURCE,nullptr);
    glCompileShader(fragment_shader);

    // Error message if compilation goes wrong.
    glGetShaderiv(fragment_shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(fragment_shader,512,nullptr,info_log);
        std::cout << "\nFRAGMENT SHADER COMPILE ERROR::" << info_log;
    }

    // Shader program

    shader_program = glCreateProgram();
    glAttachShader(shader_program, vertex_shader);
    glAttachShader(shader_program, fragment_shader);
    glLinkProgram(shader_program);

    // Error message if linkage goes wrong.
    glGetProgramiv(shader_program, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(shader_program,512,nullptr,info_log);
        std::cout << "\nSHADER PROGRAM LINKAGE ERROR::" << info_log;
    }

    // Delete all shaders.
    glDeleteShader(vertex_shader);
    glDeleteShader(fragment_shader);
}

#endif