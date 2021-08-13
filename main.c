#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

#include "physics.h"
#include "render.h"

render_state_t *render_state;

int main(int argc, char **argv) {
    render_state = create_render_state();
    if (render_state->window == NULL) {
        fprintf(stderr, "Couldn't create GLFW window.");
        exit(EXIT_FAILURE);
    }

    while (!check_exit_glfw(render_state)) {
        quad_tree_t *quad_tree = create_quad_tree(0, 0, 100, 100);
        for (int i = 0; i < 100; i++) insert_quad_tree(quad_tree, 20, 20, 10, 20, 0);
        delete_quad_tree(quad_tree);
        render_tick(render_state);
    }

    destroy_render_state(render_state);
}
