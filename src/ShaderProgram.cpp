#include <iostream>
#include <stdexcept>

#include "../include/ShaderProgram.hpp"
#include "../include/util.hpp"

ShaderProgram::ShaderProgram() {
    this->shaderProgram_ = glCreateProgram();
}

void ShaderProgram::AddShader(std::string shaderFilename, GLenum ShaderType)
{
    std::string shaderText;
    if (!ReadFile(shaderFilename.c_str(), shaderText))
    {
        fprintf(stderr, "Error creating shader type %d\n", ShaderType);
        exit(1);
    }

    GLuint ShaderObj = glCreateShader(ShaderType);
    if (ShaderObj == 0)
    {
        fprintf(stderr, "Error creating shader type %d\n", ShaderType);
        exit(1);
    }
    shaderObjects_.push_back(ShaderObj);

    const GLchar *p[1];
    p[0] = shaderText.c_str();

    GLint Lengths[1] = {(GLint)shaderText.size()};

    glShaderSource(ShaderObj, 1, p, Lengths);
    glCompileShader(ShaderObj);

    GLint success;
    glGetShaderiv(ShaderObj, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        GLchar InfoLog[1024];
        glGetShaderInfoLog(ShaderObj, 1024, NULL, InfoLog);
        fprintf(stderr, "Error compiling shader type %d: '%s'\n", ShaderType, InfoLog);
        exit(1);
    }

    glAttachShader(this->shaderProgram_, ShaderObj);
}

void ShaderProgram::Compile()
{
    GLint Success = 0;
    GLchar ErrorLog[1024] = {0};

    glLinkProgram(shaderProgram_);

    glGetProgramiv(shaderProgram_, GL_LINK_STATUS, &Success);
    if (Success == 0)
    {
        glGetProgramInfoLog(shaderProgram_, sizeof(ErrorLog), NULL, ErrorLog);
        fprintf(stderr, "Error linking shader program: '%s'\n", ErrorLog);
        exit(1);
    }

    glValidateProgram(shaderProgram_);
    glGetProgramiv(shaderProgram_, GL_VALIDATE_STATUS, &Success);
    if (!Success)
    {
        glGetProgramInfoLog(shaderProgram_, sizeof(ErrorLog), NULL, ErrorLog);
        fprintf(stderr, "Invalid shader program: '%s'\n", ErrorLog);
        exit(1);
    }
}

GLint ShaderProgram::GetUniformLocation(const char *uniformName)
{
    GLint uniformLocation = glGetUniformLocation(shaderProgram_, uniformName);
    if (uniformLocation == -1)
        throw std::runtime_error(std::string("Error getting uniform location of ") + uniformName + '\n');

    return uniformLocation;
}

void ShaderProgram::Enable()
{
    glUseProgram(shaderProgram_);
}