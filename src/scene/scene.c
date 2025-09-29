#include "scene.h"
#include <math.h>

// define render bodies
static Body bodies[] = {
    {.centre = {0, 0, -3}, .radius = 1.0f, .material = {{0.0f, 0.0f, 1.0f}}},   // red
    {.centre = {0.5, 0, -1}, .radius = 0.5f, .material = {{1.0f, 0.0f, 0.0f}}}, // blue
};
static const int num_bodies = sizeof(bodies) / sizeof(bodies[0]);

static float sphere_sdf(vec3 p, vec3 centre, float radius)
{
    float dx = p.x - centre.x;
    float dy = p.y - centre.y;
    float dz = p.z - centre.z;
    return sqrtf(dx * dx + dy * dy + dz * dz) - radius;
}

// shortest distance to any body

SdfResult scene_sdf(vec3 p)
{
    float min_dist = sphere_sdf(p, bodies[0].centre, bodies[0].radius);
    Body *object = &bodies[0];

    for (int i = 1; i < num_bodies; ++i)
    {
        float d = sphere_sdf(p, bodies[i].centre, bodies[i].radius);
        if (d < min_dist)
        {
            min_dist = d;
            object = &bodies[i];
        }
    }

    SdfResult res = {min_dist, object};
    return res;
}
