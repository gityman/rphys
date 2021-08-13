#ifndef PHYSICS_H_
#define PHYSICS_H_

#include <stdlib.h>
#include <string.h>

#define QUAD_TREE_CAPACITY 4

typedef struct quad_tree {
    float qx, qy, qw, qh;
    unsigned int size, allocated;
    float *x, *y, *r;
    struct quad_tree **sub_trees;
} quad_tree_t;

typedef struct world_state {
    int n, e, s, w;
} world_state_t;

int insert_quad_tree(quad_tree_t *, float, float, float);

quad_tree_t *create_quad_tree(int, float *, float *, float *, int, int, int, int);

#endif // PHYSICS_H_
