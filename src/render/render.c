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

            float current_dist = 0.0f;
            Body *hit_body = NULL;
            int hit = 0;

            for (int step = 0; step < 64; ++step)
            {
                RayStepResult result = ray_step(&ray, &current_dist, 5.0f, 0.01f);
                if (result.hit == 1)
                {
                    hit = 1;
                    hit_body = result.body;
                    break;
                }
                if (result.hit == -1)
                {
                    break;
                }
            }

            int pixel_index = (j * W + i) * 3;
            if (hit && hit_body)
            {
                // hit point
                vec3 hit_point = add(ray.origin, mul(ray.direction, current_dist));
                vec3 normal = calculate_normal(hit_point, hit_body);
                vec3 view_dir = norm(sub(ray.origin, hit_point));

                ShadingInfo shading = {
                    .point = hit_point,
                    .normal = normal,
                    .view_dir = view_dir,
                    .material = hit_body->material};

                // lights
                int light_count;
                const Light *scene_lights = scene_get_lights(&light_count);
                vec3 final_color = calculate_lighting(&shading, scene_lights, light_count);

                // distance attenuation
                float attenuation = 1.0f / (1.0f + current_dist * 0.05f);
                final_color = mul(final_color, attenuation);

                // Clamp to [0,1] and convert to [0,255]
                frame[pixel_index + 0] = (unsigned char)(fminf(final_color.x, 1.0f) * 255);
                frame[pixel_index + 1] = (unsigned char)(fminf(final_color.y, 1.0f) * 255);
                frame[pixel_index + 2] = (unsigned char)(fminf(final_color.z, 1.0f) * 255);
            }
            else
            {
                // black
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
