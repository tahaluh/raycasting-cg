#include "scene.h"
#include "../lighting/lighting.h"
#include <math.h>
#include <sys/time.h>
#include <stddef.h>

// animation
static int animation_paused = 1;
static double animation_start_time = 0.0;
static double pause_time = 0.0;

// light control
static int lights_enabled[10] = {1, 1, 1, 0, 0, 0, 0, 0, 0, 0};

#define ANIMATED_BALL_INDEX 1 // ball index (white sphere)
#define MAX_HEIGHT -5.0f
#define MIN_HEIGHT -0.0f

// define render bodies
static Body bodies[] = {
    // Floor plane
    {.type = BODY_PLANE, .centre = {0, -2, 0}, .geometry = {.plane = {.normal = {0, -1, 0}, .distance = 1.0f}}, .material = {.color = {0.8f, 0.8f, 0.8f}, 0.1f, 0.7f, 0.2f, 16.0f, 0.3f}}, // reflective floor
    // White sphere
    {
        .type = BODY_SPHERE,
        .centre = {0, MAX_HEIGHT, -3},
        .geometry = {.radius = 1.0f},
        .material = {.color = {0.9f, 0.9f, 0.9f}, 0.1f, 0.3f, 0.8f, 64.0f, 0.7f}}, // chrome sphere
    // yellow box
    {
        .type = BODY_CUBE,
        .centre = {-2.5f, -1.0f, -4.0f},
        .geometry = {.cube = {.size = {1.2f, 0.6f, 0.8f}}},                        // width, height, depth
        .material = {.color = {1.0f, 1.0f, 0.0f}, 0.1f, 0.7f, 0.2f, 16.0f, 0.0f}}, // yellow box

};
static const int num_bodies = sizeof(bodies) / sizeof(bodies[0]);

// define scene lights
static Light lights[] = {
    // 0: ambient
    {
        .type = LIGHT_AMBIENT,
        .color = {1.0f, 1.0f, 1.0f},
        .intensity = 0.4f},
    // 1: directional
    {
        .type = LIGHT_DIRECTIONAL,
        .color = {1.0f, 1.0f, 1.0f},
        .intensity = 1.0f,
        .direction = {1, 1, 1}},
    // 2: point blue
    {
        .type = LIGHT_POINT,
        .color = {0.0f, 0.0f, 1.0f},
        .intensity = 0.9f,
        .position = {-5, -2, -2}},
    // 3: point red
    {
        .type = LIGHT_POINT,
        .color = {1.0f, 0.2f, 0.2f},
        .intensity = 0.8f,
        .position = {5, 1, -2}},
    // 4: point green
    {
        .type = LIGHT_POINT,
        .color = {0.2f, 1.0f, 0.2f},
        .intensity = 0.8f,
        .position = {3, -1, -2}},
    // 5: directional purple
    {
        .type = LIGHT_DIRECTIONAL,
        .color = {0.8f, 0.2f, 0.8f},
        .intensity = 0.7f,
        .direction = {0, 1, 0}},
    // 6: point orange
    {
        .type = LIGHT_POINT,
        .color = {1.0f, 0.6f, 0.1f},
        .intensity = 0.5f,
        .position = {2, -1, -5}},
    // 7: directional cyan
    {
        .type = LIGHT_DIRECTIONAL,
        .color = {0.1f, 0.8f, 0.8f},
        .intensity = 0.4f,
        .direction = {0, 1, -1}},
    // 8: point yellow
    {
        .type = LIGHT_POINT,
        .color = {1.0f, 1.0f, 0.2f},
        .intensity = 0.6f,
        .position = {-3, 0, -1}},
    // 9: ambient warm
    {
        .type = LIGHT_AMBIENT,
        .color = {1.0f, 1.0f, 0.4f},
        .intensity = 10.0f}};
static const int num_lights = sizeof(lights) / sizeof(lights[0]);

// shortest distance to any body
SdfResult scene_sdf(vec3 p, float min_threshold)
{
    float min_dist = body_sdf(p, &bodies[0]);
    Body *object = &bodies[0];

    if (min_dist < min_threshold * 0.5f)
    {
        SdfResult res = {min_dist, object};
        return res;
    }

    for (int i = 1; i < num_bodies; ++i)
    {
        float d = body_sdf(p, &bodies[i]);
        if (d < min_dist)
        {
            min_dist = d;
            object = &bodies[i];

            if (min_dist < min_threshold * 0.5f)
            {
                break;
            }
        }
    }

    SdfResult res = {min_dist, object};
    return res;
}

const Light *scene_get_lights(int *count)
{
    // thread-local storage (cada thread tem seu próprio array)
    static __thread Light active_lights[10];
    int active_count = 0;

    for (int i = 0; i < num_lights && active_count < 10; i++)
    {
        if (lights_enabled[i])
        {
            active_lights[active_count] = lights[i];
            active_count++;
        }
    }

    *count = active_count;
    return active_lights;
}

// animation functions
static double get_current_time(void)
{
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec + tv.tv_usec / 1000000.0;
}

void scene_toggle_animation(void)
{
    if (animation_paused)
    {
        double current_time = get_current_time();
        animation_start_time += (current_time - pause_time);
        animation_paused = 0;
    }
    else
    {
        pause_time = get_current_time();
        animation_paused = 1;
    }
}

static float bezier_cubic(float t, float p0, float p1, float p2, float p3)
{
    float u = 1.0f - t;
    float tt = t * t;
    float uu = u * u;
    float uuu = uu * u;
    float ttt = tt * t;
    return uuu * p0 + 3.0f * uu * t * p1 + 3.0f * u * tt * p2 + ttt * p3;
}

static float calculate_bounce_height(float t)
{
    t = fmodf(t, 1.0f);

    if (t < 0.5f)
    {
        float s = t * 2.0f;

        float p0 = MAX_HEIGHT;
        float p3 = MIN_HEIGHT;

        float p1 = MAX_HEIGHT;
        float p2 = MAX_HEIGHT * 0.3f + MIN_HEIGHT * 0.7f;

        return bezier_cubic(s, p0, p1, p2, p3);
    }
    else
    {
        float s = (t - 0.5f) * 2.0f;

        float p0 = MIN_HEIGHT;
        float p3 = MAX_HEIGHT;

        float p1 = MIN_HEIGHT * 0.3f + MAX_HEIGHT * 0.7f;
        float p2 = MAX_HEIGHT;

        return bezier_cubic(s, p0, p1, p2, p3);
    }
}

void scene_update_animation(void)
{
    if (animation_paused)
        return;

    double current_time = get_current_time();

    if (animation_start_time == 0.0)
    {
        animation_start_time = current_time;
        return;
    }

    double elapsed = current_time - animation_start_time;
    float cycle_duration = 2.0f;
    float t = (float)(elapsed / cycle_duration);

    bodies[ANIMATED_BALL_INDEX].centre.y = calculate_bounce_height(t);
}
int scene_is_animation_paused(void)
{
    return animation_paused;
}

void scene_toggle_light(int light_index)
{
    if (light_index >= 0 && light_index < 10)
    {
        lights_enabled[light_index] = !lights_enabled[light_index];
        printf("Light %d %s\n", light_index, lights_enabled[light_index] ? "ON" : "OFF");
    }
}

int scene_is_light_enabled(int light_index)
{
    if (light_index >= 0 && light_index < 10)
    {
        return lights_enabled[light_index];
    }
    return 0;
}
