#ifndef CAMERA_H
#define CAMERA_H

#include <stdbool.h>
#include "shader.h"
#include "state.h"

typedef struct
{
    bool ortho;
    mat4 view, projection;
    Shader *camera_shader;
    State *camera_state;

} Camera;

bool camera_init(Camera *camera, State *state, Shader *shader);
void camera_use(Camera *camera);

#endif
