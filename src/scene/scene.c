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
        .material = {.color = {0.0f, 1.0f, 0.0f}, 0.1f, 0.8f, 0.3f, 32.0f, 0.1f}}, // Azul mais visível
    {
        .type = BODY_PLANE,
        .centre = {0, -1, 0},
        .geometry = {.plane = {.normal = {0, 1, 0}, .distance = 1.0f}},
        .material = {.color = {0.8f, 0.8f, 0.8f}, 0.2f, 0.7f, 0.1f, 8.0f, 0.0f}}, // Grey plane
};
static const int num_bodies = sizeof(bodies) / sizeof(bodies[0]);

// define scene lights
static Light lights[] = {
    // ambient
    {
        .type = LIGHT_AMBIENT,
        .color = {1.0f, 1.0f, 1.0f},
        .intensity = 0.2f},
    // directional (sun-like)
    {
        .type = LIGHT_DIRECTIONAL,
        .color = {1.0f, 1.0f, 0.9f},
        .intensity = 0.8f,
        .direction = {-1, -1, -1}},
    // point (accent light)
    {
        .type = LIGHT_POINT,
        .color = {1.0f, 0.8f, 0.6f},
        .intensity = 1.0f,
        .position = {2, 2, -1}}};
static const int num_lights = sizeof(lights) / sizeof(lights[0]);

// shortest distance to any body
SdfResult scene_sdf(vec3 p)
{
    float min_dist = body_sdf(p, &bodies[0]);
    Body *object = &bodies[0];

    for (int i = 1; i < num_bodies; ++i)
    {
        float d = body_sdf(p, &bodies[i]);
        if (d < min_dist)
        {
            min_dist = d;
            object = &bodies[i];
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
