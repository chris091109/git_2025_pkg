#ifndef WINDOW_H
#define WINDOW_H

#include <glad/glad.h>
#include "state.h"
#include <GLFW/glfw3.h>
#include <stdbool.h>

bool window_create(State *state);
bool window_should_close(State *state);
void window_start_frame(State *state);
void window_end_frame(State *state);
void window_cleanup(State *state);
void window_toggle_fullscreen(State *state);

#endif
