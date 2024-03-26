#include <iostream>
#include "../include/stb_image.h"

#include "../include/Texture.hpp"

Texture::Texture(std::string &filename)
{
    filename_ = filename;
}

Texture::~Texture()
{
}

void Texture::Load()
{
    stbi_set_flip_vertically_on_load(1);
    int width, height, channels;
    unsigned char *image_data = stbi_load(filename_.c_str(), &width, &height, &channels, 0);
    if (!image_data)
    {
        std::cout << "Unable to load texture " << filename_ << std::endl;
        exit(1);
    }

    glGenTextures(1, &textureObj_);
    glBindTexture(GL_TEXTURE_2D, textureObj_);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image_data);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_GENERATE_MIPMAP, GL_FALSE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glBindTexture(GL_TEXTURE_2D, 0);

    stbi_image_free(image_data);
}

void Texture::Bind(GLenum TextureUnit)
{
    glActiveTexture(TextureUnit);
    glBindTexture(GL_TEXTURE_2D, textureObj_);
}

GLuint Texture::getTexture() 
{
    return this->textureObj_;
}
