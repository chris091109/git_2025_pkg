#include "window.h"
#include "state.h"
#include <GLFW/glfw3.h>
#include <stdbool.h>
#include <cglm/cglm.h>

double true_mouse_x = 0;
double true_mouse_y = 0;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);

bool window_create(State *state)
{
    if (!glfwInit())
        return false;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_SAMPLES, state->msaa);

    state->monitor = glfwGetPrimaryMonitor();
    state->mode = glfwGetVideoMode(state->monitor);

    switch (state->window_state)
    {
        case FULLSCREEN:
            // Create fullscreen window with monitor's native resolution
            state->window = glfwCreateWindow(
                state->mode->width, 
                state->mode->height, 
                state->window_title, 
                state->monitor, 
                NULL
            );
            // Update state with actual fullscreen dimensions
            state->window_width = state->mode->width;
            state->window_height = state->mode->height;
            break;
            
        case MINIMIZED:
            // Create hidden window (will be minimized)
            glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);
            state->window = glfwCreateWindow(
                state->window_width, 
                state->window_height, 
                state->window_title, 
                NULL, 
                NULL
            );
            // Minimize the window after creation
            if (state->window) {
                glfwIconifyWindow(state->window);
            }
            break;
            
        case WINDOWED:
        default:
            // Create normal windowed window
            state->window = glfwCreateWindow(
                state->window_width, 
                state->window_height, 
                state->window_title, 
                NULL, 
                NULL
            );
            // Center the window on screen
            if (state->window) {
                int xpos = (state->mode->width - state->window_width) / 2;
                int ypos = (state->mode->height - state->window_height) / 2;
                glfwSetWindowPos(state->window, xpos, ypos);
            }
            break;
    } 

    if (!state->window)
    {
        glfwTerminate();
        return false;
    }
    glfwMakeContextCurrent(state->window);

    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        glfwTerminate();
        return false;
    }

    glfwSetWindowUserPointer(state->window, state);
    glfwSetFramebufferSizeCallback(state->window, framebuffer_size_callback);

    if (state->msaa > 0)
        glEnable(GL_MULTISAMPLE);

    glEnable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);


    glfwSwapInterval(state->vsync ? 1:0);
    glViewport(0,0,state->window_width, state->window_height);

    return true;
}
bool window_should_close(State *state)
{
    return glfwWindowShouldClose(state->window);
}
void window_start_frame(State *state)
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    state->window_aspect_ratio = (float)state->window_width / 
        (float)state->window_height;
}
void window_end_frame(State *state)
{

    glfwSwapBuffers(state->window);
    glfwPollEvents();

    glfwGetCursorPos(state->window, &true_mouse_x, &true_mouse_y);
    
    // Convert and store in state
    state->mouse_x = (float)true_mouse_x;
    state->mouse_y = state->window_height - (float)true_mouse_y;

    glfwSetInputMode(state->window, GLFW_CURSOR, state->mouse_hide ? 
            GLFW_CURSOR_HIDDEN : GLFW_CURSOR_NORMAL);

    state->window_resized = false;
}
void window_cleanup(State *state)
{
    if (state->window) glfwDestroyWindow(state->window);
    glfwTerminate();
}

void window_toggle_fullscreen(State *state)
{
    if (state->window_state == FULLSCREEN)
    {
        // Switch to windowed mode
        int width = 600; // Default windowed size or remember previous
        int height = 600;
        
        // Get monitor for centering
        GLFWmonitor* monitor = glfwGetPrimaryMonitor();
        const GLFWvidmode* mode = glfwGetVideoMode(monitor);
        
        // Center window
        int xpos = (mode->width - width) / 2;
        int ypos = (mode->height - height) / 2;
        
        glfwSetWindowMonitor(state->window, NULL, xpos, ypos, width, height, 0);
        state->window_state = WINDOWED;
        state->window_width = width;
        state->window_height = height;
    } else
    {
        // Switch to fullscreen mode
        GLFWmonitor* monitor = glfwGetPrimaryMonitor();
        const GLFWvidmode* mode = glfwGetVideoMode(monitor);
        
        glfwSetWindowMonitor(state->window, monitor, 0, 0, 
                           mode->width, mode->height, mode->refreshRate);
        
        state->window_state = FULLSCREEN;
        state->window_width = mode->width;
        state->window_height = mode->height;
    }
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    State* state = (State*)glfwGetWindowUserPointer(window);
    glViewport(0, 0, width, height);
    state->window_resized = true;
}


