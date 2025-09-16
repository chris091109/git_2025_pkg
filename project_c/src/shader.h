#ifndef SHADER_H
#define SHADER_H

#include <stdbool.h>
#include <cglm/cglm.h>

typedef struct
{
    unsigned int shader_id;
} Shader;

bool shader_init(Shader *shader, const char *vert, const char *frag);
void shader_use(Shader* shader);
void shader_cleanup(Shader *shader);

void shader_mat4(Shader *shader, const char *name, mat4 value);
void shader_vec3(Shader *shader, const char *name, vec3 value);
void shader_int(Shader *shader, const char *name, int value);
#endif
