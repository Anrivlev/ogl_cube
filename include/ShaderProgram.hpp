#pragma once

#include <GL/glew.h>
#include <GL/gl.h>
#include <string>
#include <vector>

class ShaderProgram
{
public:
    ShaderProgram();
    void AddShader(std::string shaderFilename, GLenum ShaderType);
    GLint GetUniformLocation(const char *uniformName);
    void Compile();
    void Enable();

protected:
    GLuint shaderProgram_ = 0;
    std::vector<GLuint> shaderObjects_;
};