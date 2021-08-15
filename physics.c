#include "physics.h"

float rand_f() {
    return ((float) (rand() % 10000)) / 10000.;
}

quad_tree_t *create_quad_tree(float qx, float qy, float qw, float qh) {
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

query_result_t query_quad_tree(quad_tree_t *quad_tree, int x, int y, int w, int h) {
    if (x + w < quad_tree->qx
        || x >= quad_tree->qx + quad_tree->qw
        || y + h < quad_tree->qy
        || y >= quad_tree->qy + quad_tree->qh) {
        return (query_result_t) {NULL, 0, 0};
    }
    query_result_t query_result = (query_result_t) {NULL, 0, 0};
    for (int i = 0; i < quad_tree->size; i++) {
        if (!(x + w < quad_tree->x[i]
            || x >= quad_tree->x[i] + quad_tree->w[i]
            || y + h < quad_tree->y[i]
            || y >= quad_tree->y[i] + quad_tree->h[i])) {
            while (query_result.size >= query_result.allocated) {
                if (query_result.allocated <= 0) query_result.allocated = 1;
                else query_result.allocated *= 2;
                query_result.ids = realloc(query_result.ids, query_result.allocated * sizeof(int));
            }
            query_result.ids[query_result.size++] = quad_tree->id[i];
        }
    }
    if (quad_tree->sub_trees == NULL) return query_result;
    for (int i = 0; i < 4; i++) {
        query_result_t sub_query_result = query_quad_tree(quad_tree->sub_trees[i], x, y, w, h);
        for (int n = 0; n < sub_query_result.size; n++) {
            while (query_result.size >= query_result.allocated) {
                if (query_result.allocated <= 0) query_result.allocated = 1;
                else query_result.allocated *= 2;
                query_result.ids = realloc(query_result.ids, query_result.allocated * sizeof(int));
            }
            query_result.ids[query_result.size++] = sub_query_result.ids[n];
        }
        free(sub_query_result.ids);
    }
    return query_result;
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

world_state_t *create_random_world_state(float wx, float wy, float ww, float wh, int num) {
    world_state_t *world_state = create_blank_world_state(wx, wy, ww, wh, num);
    for (int i = 0; i < num; i++) {
        world_state->r[i] = rand_f() * (MAX_RAD - MIN_RAD) + MIN_RAD;
        world_state->m[i] = rand_f() * (MAX_MASS - MIN_MASS) + MIN_MASS;
        world_state->x[i] = rand_f() * (ww - 2 * world_state->r[i]) + wx + world_state->r[i];
        world_state->y[i] = rand_f() * (wh - 2 * world_state->r[i]) + wy + world_state->r[i];
        world_state->dx[i] = rand_f() * 2 * MAX_INIT_VAL - MAX_INIT_VAL;
        world_state->dy[i] = rand_f() * 2 * MAX_INIT_VAL - MAX_INIT_VAL;
    }
    return world_state;
}

world_state_t *create_blank_world_state(float wx, float wy, float ww, float wh, int num) {
    world_state_t *world_state = malloc(sizeof(world_state_t));
    world_state->wx = wx;
    world_state->wy = wy;
    world_state->ww = ww;
    world_state->wh = wh;
    world_state->num = num;
    world_state->quad_tree = NULL;
    world_state->x = malloc(num * sizeof(float));
    world_state->y = malloc(num * sizeof(float));
    world_state->dx = malloc(num * sizeof(float));
    world_state->dy = malloc(num * sizeof(float));
    world_state->r = malloc(num * sizeof(float));
    world_state->m = malloc(num * sizeof(float));
    return world_state;
}

quad_tree_t *create_quad_tree_from_world_state(world_state_t *world_state) {
    quad_tree_t *quad_tree = create_quad_tree(world_state->wx, world_state->wy, world_state->ww, world_state->wh);
    for (int i = 0; i < world_state->num; i++) {
        insert_quad_tree(quad_tree, world_state->x[i] - world_state->r[i], world_state->y[i] - world_state->r[i], world_state->r[i] * 2, world_state->r[i] * 2, i);
    }
    return quad_tree;
}

float fast_inv_sqrt(float in) {
    long bits;
    float half = in * 0.5;
    bits = *(long *) &in;
    bits = 0x5f3759df - (bits >> 1);
    in = *(float *) &bits;
    in = in * (1.5 - (half * in * in));
    in = in * (1.5 - (half * in * in));
    return in;
}

world_state_t *physics_tick(world_state_t *world_state, float dt) {
    world_state_t *new_world_state = create_blank_world_state(world_state->wx, world_state->wy, world_state->ww, world_state->wh, world_state->num);
    new_world_state->quad_tree = world_state->quad_tree;
    for (int i = 0; i < world_state->num; i++) {
        new_world_state->x[i] = dt * world_state->dx[i] + world_state->x[i];
        new_world_state->y[i] = dt * world_state->dy[i] + world_state->y[i];
        new_world_state->dx[i] = world_state->dx[i];
        new_world_state->dy[i] = world_state->dy[i] - GRAVITY;
        new_world_state->r[i] = world_state->r[i];
        new_world_state->m[i] = world_state->m[i];
        if (new_world_state->x[i] - new_world_state->r[i] < new_world_state->wx) {
            new_world_state->x[i] = new_world_state->r[i] + new_world_state->wx;
            new_world_state->dx[i] *= -FRICTION;
        }
        if (new_world_state->y[i] - new_world_state->r[i] < new_world_state->wy) {
            new_world_state->y[i] = new_world_state->r[i] + new_world_state->wy;
            new_world_state->dy[i] *= -FRICTION;
        }
        if (new_world_state->x[i] + new_world_state->r[i] >= new_world_state->wx + new_world_state->ww) {
            new_world_state->x[i] = new_world_state->wx + new_world_state->ww - new_world_state->r[i];
            new_world_state->dx[i] *= -FRICTION;
        }
        if (new_world_state->y[i] + new_world_state->r[i] >= new_world_state->wy + new_world_state->wh) {
            new_world_state->y[i] = new_world_state->wy + new_world_state->wh - new_world_state->r[i];
            new_world_state->dy[i] *= -FRICTION;
        }
    }
    for (int i = 0; i < world_state->num; i++) {
        query_result_t query_result = query_quad_tree(new_world_state->quad_tree, new_world_state->x[i] - new_world_state->r[i], new_world_state->y[i] - new_world_state->r[i], new_world_state->r[i] * 2, new_world_state->r[i] * 2);
        for (int qi = 0; qi < query_result.size; qi++) {
            int qid = query_result.ids[qi];
            if (i == qid) continue;
            float dx = new_world_state->x[i] - new_world_state->x[qid];
            float dy = new_world_state->y[i] - new_world_state->y[qid];
            float ar = new_world_state->r[i] + new_world_state->r[qid];
            float dist = dx * dx + dy * dy - ar * ar;
            if (dist < 0) {
                float old_dx_i = new_world_state->dx[i];
                float old_dy_i = new_world_state->dy[i];
                float old_dx_qid = new_world_state->dx[qid];
                float old_dy_qid = new_world_state->dy[qid];
                new_world_state->dx[i] = FRICTION * (new_world_state->dx[i] * (new_world_state->m[i] - new_world_state->m[qid]) + (2 * new_world_state->m[qid] * old_dx_qid)) / (new_world_state->m[i] + new_world_state->m[qid]);
                new_world_state->dy[i] = FRICTION * (new_world_state->dy[i] * (new_world_state->m[i] - new_world_state->m[qid]) + (2 * new_world_state->m[qid] * old_dy_qid)) / (new_world_state->m[i] + new_world_state->m[qid]);
                new_world_state->dx[qid] = FRICTION * (new_world_state->dx[qid] * (new_world_state->m[qid] - new_world_state->m[i]) + (2 * new_world_state->m[i] * old_dx_i)) / (new_world_state->m[i] + new_world_state->m[qid]);
                new_world_state->dy[qid] = FRICTION * (new_world_state->dy[qid] * (new_world_state->m[qid] - new_world_state->m[i]) + (2 * new_world_state->m[i] * old_dy_i)) / (new_world_state->m[i] + new_world_state->m[qid]);
                new_world_state->x[i] += dt * new_world_state->dx[i];
                new_world_state->y[i] += dt * new_world_state->dy[i];
                new_world_state->x[qid] += dt * new_world_state->dx[qid];
                new_world_state->y[qid] += dt * new_world_state->dy[qid];
            }
        }
        free(query_result.ids);
    }
    return new_world_state;
}

void delete_world_state(world_state_t *world_state) {
    free(world_state->x);
    free(world_state->y);
    free(world_state->dx);
    free(world_state->dy);
    free(world_state->r);
    free(world_state->m);
    free(world_state);
}
