#ifndef PHYSICS_H_
#define PHYSICS_H_

#include <stdlib.h>
#include <string.h>

#define QUAD_TREE_CAPACITY 4
#define MIN_RAD 5
#define MAX_RAD 20
#define MAX_INIT_VAL 0.1

typedef struct quad_tree {
    float qx, qy, qw, qh;
    int size, allocated, *id;
    float *x, *y, *w, *h;
    struct quad_tree **sub_trees;
} quad_tree_t;

typedef struct world_state {
    float wx, wy, ww, wh, num;
    float *x, *y, *dx, *dy, *r;
    quad_tree_t *quad_tree;
} world_state_t;

float rand_f();

quad_tree_t *create_quad_tree(float, float, float, float);

int insert_quad_tree(quad_tree_t *, float, float, float, float, int);

void delete_quad_tree(quad_tree_t *);

world_state_t *create_random_world_state(float, float, float, float, int);

quad_tree_t *create_quad_tree_from_world_state(world_state_t *);

#endif // PHYSICS_H_
