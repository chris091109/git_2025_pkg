#ifndef STATE_H
#define STATE_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdbool.h>

typedef enum 
{
    WINDOWED, FULLSCREEN, MINIMIZED
} Window_State;

typedef struct
{
    GLFWwindow *window;
    GLFWmonitor *monitor;
    const GLFWvidmode* mode;

    // window
    int window_width;
    int window_height;
    const char *window_title;
    Window_State window_state;
    bool window_active;
    float window_aspect_ratio;
    bool window_resized;

    // graphics
    int msaa;
    bool vsync;

    // mouse
    double mouse_x;
    double mouse_y;
    bool mouse_hide;

    // camera
    float fov;
    int max_render_distance;
} State;
#endif
