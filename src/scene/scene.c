#include "scene.h"
#include "../lighting/lighting.h"
#include <math.h>

// define render bodies
static Body bodies[] = {
    // Blue sphere
    {
        .type = BODY_SPHERE,
        .centre = {0, 0, -3},
        .geometry = {.radius = 1.0f},
        .material = {.color = {1.0f, 1.0f, 1.0f}, 0.1f, 0.8f, 0.3f, 32.0f, 0.1f}}, // white sphere
    {
        .type = BODY_PLANE,
        .centre = {0, -2, 0},
        .geometry = {.plane = {.normal = {0, -1, 0}, .distance = 1.0f}},
        .material = {.color = {1.0f, 1.0f, 1.0f}, 1.0f, 0.7f, 0.1f, 8.0f, 0.0f}}, // floor
};
static const int num_bodies = sizeof(bodies) / sizeof(bodies[0]);

// define scene lights
static Light lights[] = {
    // ambient
    {
        .type = LIGHT_AMBIENT,
        .color = {1.0f, 1.0f, 1.0f},
        .intensity = 0.4f},
    // directional
    {
        .type = LIGHT_DIRECTIONAL,
        .color = {1.0f, 1.0f, 1.0f},
        .intensity = 1.0f,
        .direction = {1, 1, 1}},
    // point
    {
        .type = LIGHT_POINT,
        .color = {0.0f, 0.0f, 1.0f},
        .intensity = 0.9f,
        .position = {-5, -2, -2}}};
static const int num_lights = sizeof(lights) / sizeof(lights[0]);

// shortest distance to any body
SdfResult scene_sdf(vec3 p, float min_threshold)
{
    float min_dist = body_sdf(p, &bodies[0]);
    Body *object = &bodies[0];

    if (min_dist < min_threshold)
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

            if (min_dist < min_threshold)
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
    *count = num_lights;
    return lights;
}
