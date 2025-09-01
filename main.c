#include <GL/glut.h>
#include <stdlib.h>

static unsigned char *frame = NULL; // W*H*3 bytes (RGB)

#define W 640
#define H 360

static void render_solid(void)
{
    if (!frame)
        frame = (unsigned char *)malloc(W * H * 3);
    for (int j = 0; j < H; ++j)
    {
        for (int i = 0; i < W; ++i)
        {
            int idx = (j * W + i) * 3;
            frame[idx + 0] = 0;   // R
            frame[idx + 1] = 0;   // G
            frame[idx + 2] = 255; // B
        }
    }
}

void display()
{
    if (!frame)
        render_solid();

    glClear(GL_COLOR_BUFFER_BIT);
    glRasterPos2f(-1.0f, -1.0f);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glDrawPixels(W, H, GL_RGB, GL_UNSIGNED_BYTE, frame);
    glFlush();
}

void reshape(int w, int h)
{
    glViewport(0, 0, w, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-1, 1, -1, 1, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowSize(W, H);
    glutCreateWindow("Raytracing Gravidade");
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutMainLoop();
    return 0;
}
