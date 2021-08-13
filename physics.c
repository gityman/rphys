#include "physics.h"

int insert_quad_tree(quad_tree_t *quad_tree, float x, float y, float w, float h, int id) {
    if (x < quad_tree->qx || y < quad_tree->qy || x + w >= quad_tree->qx + quad_tree->qw || y + h >= quad_tree->qy + quad_tree->qh) {
        return 0;
    }
    if (quad_tree->size < QUAD_TREE_CAPACITY) {
        quad_tree->x[quad_tree->size] = x;
        quad_tree->y[quad_tree->size] = y;
        quad_tree->w[quad_tree->size] = w;
        quad_tree->h[quad_tree->size] = h;
        quad_tree->id[quad_tree->size] = id;
        quad_tree->size++;
        return 1;
    }
    if (quad_tree->sub_trees == NULL) {
        quad_tree->sub_trees = malloc(4 * sizeof(quad_tree_t *));
        int qx0 = quad_tree->qx;
        int qy0 = quad_tree->qy;
        int qx1 = quad_tree->qx + quad_tree->qw / 2;
        int qy1 = quad_tree->qy + quad_tree->qh / 2;
        int qx2 = quad_tree->qx + quad_tree->qw;
        int qy2 = quad_tree->qy + quad_tree->qh;
        quad_tree->sub_trees[0] = create_quad_tree(qx0, qy0, qx1 - qx0, qy1 - qy0);
        quad_tree->sub_trees[1] = create_quad_tree(qx1, qy0, qx2 - qx1, qy1 - qy0);
        quad_tree->sub_trees[2] = create_quad_tree(qx0, qy1, qx1 - qx0, qy2 - qy1);
        quad_tree->sub_trees[3] = create_quad_tree(qx1, qy1, qx2 - qx1, qy2 - qy1);
    }
    int sub_insert = insert_quad_tree(quad_tree->sub_trees[0], x, y, w, h, id)
        || insert_quad_tree(quad_tree->sub_trees[1], x, y, w, h, id)
        || insert_quad_tree(quad_tree->sub_trees[2], x, y, w, h, id)
        || insert_quad_tree(quad_tree->sub_trees[3], x, y, w, h, id);
    if (sub_insert > 0) return 1;
    while (quad_tree->size >= quad_tree->allocated) {
        quad_tree->allocated *= 2;
        quad_tree->x = realloc(quad_tree->x, quad_tree->allocated * sizeof(float));
        quad_tree->y = realloc(quad_tree->y, quad_tree->allocated * sizeof(float));
        quad_tree->w = realloc(quad_tree->w, quad_tree->allocated * sizeof(float));
        quad_tree->h = realloc(quad_tree->h, quad_tree->allocated * sizeof(float));
        quad_tree->id = realloc(quad_tree->id, quad_tree->allocated * sizeof(float));
    }
    quad_tree->x[quad_tree->size] = x;
    quad_tree->y[quad_tree->size] = y;
    quad_tree->w[quad_tree->size] = w;
    quad_tree->h[quad_tree->size] = h;
    quad_tree->id[quad_tree->size] = id;
    quad_tree->size++;
    return 1;
}

quad_tree_t *create_quad_tree(int qx, int qy, int qw, int qh) {
    quad_tree_t *quad_tree = malloc(sizeof(quad_tree_t));
    quad_tree->qx = qx;
    quad_tree->qy = qy;
    quad_tree->qw = qw;
    quad_tree->qh = qh;
    quad_tree->size = 0;
    quad_tree->allocated = QUAD_TREE_CAPACITY;
    quad_tree->x = malloc(quad_tree->allocated * sizeof(float));
    quad_tree->y = malloc(quad_tree->allocated * sizeof(float));
    quad_tree->w = malloc(quad_tree->allocated * sizeof(float));
    quad_tree->h = malloc(quad_tree->allocated * sizeof(float));
    quad_tree->id = malloc(quad_tree->allocated * sizeof(float));
    quad_tree->sub_trees = NULL;
    return quad_tree;
    return NULL;
}

void delete_quad_tree(quad_tree_t *quad_tree) {
    if (quad_tree == NULL) return;
    free(quad_tree->x);
    free(quad_tree->y);
    free(quad_tree->w);
    free(quad_tree->h);
    free(quad_tree->id);
    if (quad_tree->sub_trees != NULL) {
        for (int i = 0; i < 4; i++) {
            delete_quad_tree(quad_tree->sub_trees[i]);
        }
        free(quad_tree->sub_trees);
    }
    free(quad_tree);
}
