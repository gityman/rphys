#define GL_SILENCE_DEPRECATION

#include "render.h"

render_state_t *render_state_pt;

void error_callback(int error, const char* description) {
    fprintf(stderr, "Error #%d: %s\n", error, description);
}

void resize_callback(__attribute__((unused)) GLFWwindow* window, int iwidth, int iheight) {
    render_state_pt->width = iwidth;
    render_state_pt->height = iheight;
    glViewport(0, 0, render_state_pt->width, render_state_pt->height);
}

render_state_t *create_render_state() {
    render_state_t *ret = malloc(sizeof(render_state_t));
    if (!glfwInit()) return ret;
    glfwSetErrorCallback(error_callback);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_TRUE);
    ret->window = glfwCreateWindow(800, 800, "Russel's Physics Engine", NULL, NULL);
    if (ret->window) {
        glfwShowWindow(ret->window);
        glfwMakeContextCurrent(ret->window);
        glfwSetInputMode(ret->window, GLFW_STICKY_KEYS, GL_TRUE);
    }
    render_state_pt = ret;
    glfwGetFramebufferSize(ret->window, &ret->width, &ret->height);
    glViewport(0, 0, ret->width, ret->height);
    glfwSetFramebufferSizeCallback(ret->window, resize_callback);
    return ret;
}

int check_exit_glfw(render_state_t *render_state) {
    return glfwWindowShouldClose(render_state->window) || glfwGetKey(render_state->window, GLFW_KEY_ESCAPE);
}

void draw_regular_poly(render_state_t *render_state, float cx, float cy, float r, int segments) {
    printf("%d\n", segments);
    if (segments < 3) return;
    float theta = 2 * M_PI / segments;
    float tan_fac = tanf(theta);
    float rad_fac = cosf(theta);
    float x = r, y = 0;

    glBegin(GL_POLYGON);

    for (int i = 0; i < segments; i++) {
        glVertex2f(4 * (x + cx) / render_state->width, 4 * (y + cy) / render_state->height);
        float ox = x;
        x += -y * tan_fac;
        y += ox * tan_fac;
        x *= rad_fac;
        y *= rad_fac;
    }

    glEnd();
}

void draw_circle(render_state_t *render_state, float cx, float cy, float r) {
    draw_regular_poly(render_state, cx, cy, r, 10 * sqrtf(r));
}

void render_tick(render_state_t *render_state) {
    glClear(GL_COLOR_BUFFER_BIT);

    glfwSwapBuffers(render_state->window);
    glfwPollEvents();
}

void destroy_render_state(render_state_t *render_state) {
    glfwDestroyWindow(render_state->window);
    glfwTerminate();
    free(render_state);
}
