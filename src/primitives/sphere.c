#include "sphere.h"
#include <math.h>

float hit_sphere(vec3 orig, vec3 dir, const sphere_t *s)
{
    vec3 oc = sub(orig, s->c);
    float a = dot(dir, dir);
    float b = 2.0f * dot(oc, dir);
    float c = dot(oc, oc) - s->r * s->r;
    float disc = b * b - 4 * a * c;
    if (disc < 0.0f)
        return -1.0f;
    float sq = sqrtf(disc);
    float t1 = (-b - sq) / (2 * a);
    float t2 = (-b + sq) / (2 * a);
    if (t1 > 0.0f)
        return t1;
    if (t2 > 0.0f)
        return t2;
    return -1.0f;
}
