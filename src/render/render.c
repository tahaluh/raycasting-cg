#include "render.h"
#include "../core/vec3.h"
#include "../scene/scene.h"
#include <stdlib.h>
#include <math.h>
#include <../ray/ray.h>

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
            // create a ray
            vec3 dir = norm(V(((2.f * ((i + 0.5f) / (float)W) - 1.f)) * aspect * scale,
                              (1.f - 2.f * ((j + 0.5f) / (float)H)) * scale,
                              -1.f));
            Ray ray = {cam_pos, dir};

            float current_dist = 0.0f;
            int hit = 0;
            for (int step = 0; step < 100; ++step)
            {
                int result = ray_step(&ray, &current_dist, 100.0f, 0.01f);
                if (result == 1)
                {
                    hit = 1;
                    break;
                }
                if (result == -1)
                {
                    break;
                }
            }

            int pixel_index = (j * W + i) * 3;
            if (hit)
            {
                // paint red
                frame[pixel_index + 0] = 255 / current_dist;
                frame[pixel_index + 1] = 0;
                frame[pixel_index + 2] = 0;
            }
            else
            {
                // paint black
                frame[pixel_index + 0] = 0;
                frame[pixel_index + 1] = 0;
                frame[pixel_index + 2] = 0;
            }
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
