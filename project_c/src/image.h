#ifndef IMAGE_H
#define IMAGE_H

#include "cglm/types.h"
#include "shader.h"
#include <glad/glad.h>
#include <stdbool.h>

typedef struct
{
    int width, height, nrChannels;
    unsigned int VBO, VAO, EBO;
    unsigned int texture;
    GLenum format;
    bool filp_vertical;
    Shader *image_shader;
} Image;

bool image_create(Image *image, Shader *shader, const char *path);
void image_draw(Image *image, vec2 loc, float scale);
void image_cleanup(Image *image);

#endif
