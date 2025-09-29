#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "./render/render.h"
#include "./core/input.h"
#include "./lighting/lighting.h"
#include "./scene/scene.h"

static unsigned long g_frame = 0;
static int g_prev_ms = 0;

#define WIN_W 500
#define WIN_H 300
#define TARGET_FPS 60
#define FRAME_TIME_MS 0 // ~16.67ms for 60 FPS (rounded down)

static void display(void)
{
    int now = glutGet(GLUT_ELAPSED_TIME);
    int dt = (g_prev_ms == 0) ? 0 : (now - g_prev_ms);

    // FPS limiting
    if (dt < FRAME_TIME_MS && g_prev_ms != 0)
    {
        glutPostRedisplay();
        return;
    }

    // input
    float delta_time = dt / 1000.0f; // seconds
    input_process(delta_time);

    // animation
    scene_update_animation();

    render_scene();

    const unsigned char *frame = render_get_frame();
    glClear(GL_COLOR_BUFFER_BIT);
    glRasterPos2f(-1.f, -1.f);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glDrawPixels(WIN_W, WIN_H, GL_RGB, GL_UNSIGNED_BYTE, frame);
    glFlush();

    double fps = (dt > 0) ? 1000.0 / dt : 0.0;
    vec3 cam_pos = render_get_camera_position();
    printf("[frame %lu] dt=%d ms, fps=%.1f, pos=(%.2f, %.2f, %.2f) reflections=%s animation=%s\n",
           g_frame, dt, fps, cam_pos.x, cam_pos.y, cam_pos.z,
           get_reflections_enabled() ? "ON" : "OFF",
           scene_is_animation_paused() ? "PAUSED" : "PLAYING");
    fflush(stdout);

    g_prev_ms = now;
    g_frame++;

    glutPostRedisplay();
}

static void reshape(int w, int h)
{
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-1, 1, -1, 1, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

static void cleanup(void)
{
    render_free();
}

int main(int argc, char **argv)
{
    atexit(cleanup);

    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(WIN_W, WIN_H);
    glutCreateWindow("Raytracing Gravity");

    render_init(WIN_W, WIN_H);
    input_init();

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);

    // input
    glutKeyboardFunc(input_keyboard_down);
    glutKeyboardUpFunc(input_keyboard_up);
    glutSpecialFunc(input_special_down);
    glutSpecialUpFunc(input_special_up);

    glutMainLoop();
    return 0;
}
