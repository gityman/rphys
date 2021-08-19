#include <sys/time.h>
#include <stdlib.h>
#include <stdio.h>
#include <errno.h>

#include "physics.h"
#include "render.h"

render_state_t *render_state;
world_state_t *world_state;

int main(int argc, char **argv) {
    struct timeval start, stop;
    gettimeofday(&start, NULL);
    srand(start.tv_usec);
    float dt = 0.;
    render_state = create_render_state();
    world_state = create_random_world_state(-400, -400, 800, 800, NUM_INITIAL);
    if (render_state->window == NULL) {
        fprintf(stderr, "Couldn't create GLFW window.");
        exit(EXIT_FAILURE);
    }

    while (!check_exit_glfw(render_state)) {
        gettimeofday(&start, NULL);
        world_state_t *new_world_state = physics_tick(world_state, dt);
        delete_world_state(world_state);
        world_state = new_world_state;
        render_tick(render_state, world_state);
        gettimeofday(&stop, NULL);
        dt = (double) (stop.tv_usec - start.tv_usec) / 1000000. + (double) (stop.tv_sec - start.tv_sec);
        printf("%f\n", dt * 1000.);
    }

    destroy_render_state(render_state);
}
