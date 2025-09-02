#include <GL/glut.h>
#include <stdlib.h>
#include <stdio.h>
#include "./render/render.h"

static unsigned long g_frame = 0;
static int g_prev_ms = 0;

#define WIN_W 500
#define WIN_H 300

static void display(void)
{
    int now = glutGet(GLUT_ELAPSED_TIME);
    int dt = (g_prev_ms == 0) ? 0 : (now - g_prev_ms);

    render_scene();

    const unsigned char *frame = render_get_frame();
    glClear(GL_COLOR_BUFFER_BIT);
    glRasterPos2f(-1.f, -1.f);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glDrawPixels(WIN_W, WIN_H, GL_RGB, GL_UNSIGNED_BYTE, frame);
    glFlush();

    double fps = (dt > 0) ? 1000.0 / dt : 0.0;
    printf("[frame %lu] dt=%d ms, fps=%.1f\n", g_frame, dt, fps);
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

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutMainLoop();
    return 0;
}
