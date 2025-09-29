#include "body.h"
#include <math.h>
#include <float.h>

static float sphere_sdf(vec3 p, vec3 centre, float radius)
{
    float dx = p.x - centre.x;
    float dy = p.y - centre.y;
    float dz = p.z - centre.z;
    return sqrtf(dx * dx + dy * dy + dz * dz) - radius;
}

static float plane_sdf(vec3 p, vec3 normal, float distance)
{
    return dot(p, normal) + distance;
}

static float cube_sdf(vec3 p, vec3 centre, vec3 size)
{
    vec3 d = sub(p, centre);
    d.x = fabsf(d.x) - size.x;
    d.y = fabsf(d.y) - size.y;
    d.z = fabsf(d.z) - size.z;

    float max_inside = fmaxf(fmaxf(d.x, d.y), d.z);
    vec3 outside = V(fmaxf(d.x, 0), fmaxf(d.y, 0), fmaxf(d.z, 0));
    return fminf(max_inside, 0.0f) + sqrtf(dot(outside, outside));
}

static float triangle_sdf(vec3 p, vec3 a, vec3 b, vec3 c)
{
    // Simplified triangle SDF - distance to closest point on triangle
    vec3 ba = sub(b, a);
    vec3 pa = sub(p, a);
    vec3 ac = sub(a, c);
    vec3 pc = sub(p, c);

    // Calculate normal using cross product
    vec3 nor = cross(ba, ac);

    // Simple distance to triangle plane
    float nor_length = sqrtf(dot(nor, nor));
    if (nor_length < 1e-6f)
    {
        // Degenerate triangle, return distance to point a
        return sqrtf(dot(pa, pa));
    }

    float plane_dist = fabsf(dot(nor, pa)) / nor_length;

    // Distance to triangle vertices (simplified)
    float dist_a = sqrtf(dot(pa, pa));
    float dist_c = sqrtf(dot(pc, pc));

    float min_dist = fminf(dist_a, dist_c);
    return fminf(plane_dist, min_dist);
}
float body_sdf(vec3 p, const Body *body)
{
    switch (body->type)
    {
    case BODY_SPHERE:
        return sphere_sdf(p, body->centre, body->geometry.radius);
    case BODY_PLANE:
        return plane_sdf(p, body->geometry.plane.normal, body->geometry.plane.distance);
    case BODY_CUBE:
        return cube_sdf(p, body->centre, body->geometry.cube.size);
    case BODY_TRIANGLE:
        return triangle_sdf(p, body->geometry.triangle.p1, body->geometry.triangle.p2, body->geometry.triangle.p3);
    default:
        return 1e30f; // Large number instead of INFINITY
    }
}