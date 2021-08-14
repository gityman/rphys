#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

#include "physics.h"
#include "render.h"

render_state_t *render_state;
world_state_t *world_state;

int main(int argc, char **argv) {
    render_state = create_render_state();
    world_state = create_random_world_state(-200, -200, 400, 400, 100);
    if (render_state->window == NULL) {
        fprintf(stderr, "Couldn't create GLFW window.");
        exit(EXIT_FAILURE);
    }

    while (!check_exit_glfw(render_state)) {
        delete_quad_tree(world_state->quad_tree);
        world_state->quad_tree = create_quad_tree_from_world_state(world_state);
        render_tick(render_state, world_state);
    }

    destroy_render_state(render_state);
}
