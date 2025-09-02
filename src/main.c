#include <GL/glut.h>
#include <stdlib.h>
#include "render.h"

#define WIN_W 640
#define WIN_H 360

static void display(void)
{
    render_scene();
    const unsigned char *frame = render_get_frame();
    glClear(GL_COLOR_BUFFER_BIT);
    glRasterPos2f(-1.f, -1.f);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glDrawPixels(WIN_W, WIN_H, GL_RGB, GL_UNSIGNED_BYTE, frame);
    glFlush();
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
