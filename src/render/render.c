// src/render/render.c
#include "render.h"
#include "../core/vec3.h"
#include "../scene/scene.h"
#include <stdlib.h>
#include <math.h>

#ifndef PI
#define PI 3.14159265358979323846f
#endif

static int W = 0, H = 0;
static unsigned char *frame = NULL;

static vec3 cam_pos = {0, 0, 0};
static float fov_deg = 60.0f;

void render_init(int width, int height)
{
    W = width;
    H = height;
    if (frame)
    {
        free(frame);
        frame = NULL;
    }
    frame = (unsigned char *)malloc((size_t)W * (size_t)H * 3);
}

const unsigned char *render_get_frame(void) { return frame; }

void render_scene(void)
{
    if (!frame || W <= 0 || H <= 0)
        return;

    float aspect = (float)W / (float)H;
    float scale = tanf((fov_deg * 0.5f) * PI / 180.0f);

    for (int j = 0; j < H; ++j)
    {
        for (int i = 0; i < W; ++i)
        {
            float x = ((2.f * ((i + 0.5f) / (float)W) - 1.f)) * aspect * scale;
            float y = (1.f - 2.f * ((j + 0.5f) / (float)H)) * scale;
            vec3 dir = norm(V(x, y, -1.f));

            float t = hit_sphere(cam_pos, dir, &g_sphere);

            float r = 0.f, g = 0.f, b = 0.f;
            if (t > 0.f)
            {
                r = g_sphere.color.x;
                g = g_sphere.color.y;
                b = g_sphere.color.z;
            }

            int idx = (j * W + i) * 3;
            frame[idx + 0] = (unsigned char)(255.f * r);
            frame[idx + 1] = (unsigned char)(255.f * g);
            frame[idx + 2] = (unsigned char)(255.f * b);
        }
    }
}

void render_free(void)
{
    if (frame)
    {
        free(frame);
        frame = NULL;
    }
}
