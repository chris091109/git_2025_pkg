#ifndef MODEL_H
#define MODEL_H

#include <stdbool.h>
#include <glad/glad.h>
#include <cglm/cglm.h>
#include "shader.h"


typedef struct
{
    unsigned int VAO,VBO,EBO;
    Shader *model_shader;

} Model;

bool model_init(Model *model, float *vertices, int *indices);
void model_draw(Model *model, vec3 loc);
void model_cleanup(Model *model);

#endif
