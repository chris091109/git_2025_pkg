#include "camera.h"

bool camera_init(Camera *camera, State *state, Shader *shader)
{
    camera->camera_state = state;
    camera->camera_shader = shader;

    mat4 projection;
    glm_ortho(0.0f, (float)camera->camera_state->window_width, 0.0f,
            (float)camera->camera_state->window_height, -1.0f, 1.0f, projection);
    shader_mat4(camera->camera_shader, "projection", camera->projection);

    mat4 view;
    glm_mat4_identity(view);
    shader_mat4(camera->camera_shader, "view", camera->view);
    return true;
}
void camera_use(Camera *camera)
{
    if (camera->camera_state->window_resized) {
            glm_ortho(0.0f, (float)camera->camera_state->window_width, 0.0f,
                    (float)camera->camera_state->window_height,
                    -1.0f, 1.0f, camera->projection);
            shader_use(camera->camera_shader);
            shader_mat4(camera->camera_shader, "projection", camera->projection);
        }
}
