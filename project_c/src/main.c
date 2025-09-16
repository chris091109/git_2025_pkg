#include "state.h"
#include "window.h"
#include "shader.h"
#include "image.h"
#include <cglm/cglm.h>

#include <ft2build.h>
#include FT_FREETYPE_H

int main()
{
    State state = 
    {
        .window_width = 600,
        .window_height = 600,
        .window_title = "OpenGL",
        .window_state = FULLSCREEN,
        .vsync = true
    };
    window_create(&state);    

    Shader shader;
    shader_init(&shader, "content/shader.vert", "content/shader.frag");
    shader_use(&shader);
    
    // Use ORTHOGRAPHIC projection instead of perspective
    mat4 projection;
    glm_ortho(0.0f, (float)state.window_width, 0.0f, (float)state.window_height, -1.0f, 1.0f, projection);
    shader_mat4(&shader, "projection", projection);

    // Identity view matrix (no camera movement needed for 2D)
    mat4 view;
    glm_mat4_identity(view);
    shader_mat4(&shader, "view", view);

    shader_int(&shader, "ourTexture", 0);

    Image image;
    image_create(&image, &shader, "content/x.png");

    while (!window_should_close(&state))
    {
        window_start_frame(&state);
        
        // Update projection only if window is resized
        if (state.window_resized) {
            glm_ortho(0.0f, (float)state.window_width, 0.0f,
                    (float)state.window_height, -1.0f, 1.0f, projection);
            shader_use(&shader);
            shader_mat4(&shader, "projection", projection);
        }
        
        image_draw(&image, (vec2){state.mouse_x, state.mouse_y}, 0.2f);

        window_end_frame(&state);
    }

    image_cleanup(&image);
    shader_cleanup(&shader);
    window_cleanup(&state);
}
