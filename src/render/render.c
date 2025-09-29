#include "render.h"
#include <stdlib.h>
#include <math.h>
#include "../core/vec3.h"
#include "../core/camera.h"
#include "../scene/scene.h"
#include "../ray/ray.h"
#include "../body/body.h"
#include "../lighting/lighting.h"

#ifndef PI
#define PI 3.14159265358979323846f
#endif

static int W = 0, H = 0;
static unsigned char *frame = NULL;

Camera g_camera;
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

    // camera
    camera_init(&g_camera);
}

const unsigned char *render_get_frame(void) { return frame; }

void render_scene(void)
{
    if (!frame || W <= 0 || H <= 0)
        return;

    float aspect = (float)W / (float)H;
    float scale = tanf((fov_deg * 0.5f) * PI / 180.0f);

// thread
#pragma omp parallel for collapse(2) schedule(guided, 8)
    for (int j = 0; j < H; ++j)
    {
        for (int i = 0; i < W; ++i)
        {
            // create a ray camera
            float screen_x = (2.f * ((i + 0.5f) / (float)W) - 1.f);
            float screen_y = (1.f - 2.f * ((j + 0.5f) / (float)H));
            vec3 dir = camera_get_ray_direction(&g_camera, screen_x, screen_y, aspect, scale);
            Ray ray = {g_camera.position, dir};

            int pixel_index = (j * W + i) * 3;

            // ray tracing
            vec3 final_color = trace_ray(&ray, 3, 20.0f, 0.001f); // depth = 3

            // [0,1] to [0,255]
            frame[pixel_index + 0] = (unsigned char)(fminf(final_color.x, 1.0f) * 255);
            frame[pixel_index + 1] = (unsigned char)(fminf(final_color.y, 1.0f) * 255);
            frame[pixel_index + 2] = (unsigned char)(fminf(final_color.z, 1.0f) * 255);
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

vec3 render_get_camera_position(void)
{
    return g_camera.position;
}
