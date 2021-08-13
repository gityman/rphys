#ifndef RENDER_H_
#define RENDER_H_

#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include <GLFW/glfw3.h>

#include "physics.h"

typedef struct render_state {
    int width, height;
    GLFWwindow *window;
} render_state_t;

render_state_t *create_render_state();

int check_exit_glfw(render_state_t *);

void draw_regular_poly(render_state_t *, float, float, float, int);

void draw_circle(render_state_t *, float, float, float);

void draw_quad_tree(render_state_t *, quad_tree_t *);

void render_tick(render_state_t *);

void destroy_render_state(render_state_t *);

#endif // RENDER_H_
