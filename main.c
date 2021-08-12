#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

#include "render.h"

render_state_t *render_state;

int main(int argc, char **argv) {
    render_state = create_render_state();
    if (render_state->window == NULL) {
        fprintf(stderr, "Couldn't create GLFW window.");
        exit(EXIT_FAILURE);
    }

    while (!check_exit_glfw(render_state)) {

    }

    destroy_render_state(render_state);
}
