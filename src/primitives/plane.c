#include "plane.h"

float hit_plane(vec3 orig, vec3 dir, const plane_t *p)
{
    float denom = dot(p->n, dir);
    if (denom > -1e-6f && denom < 1e-6f)
        return -1.0f;
    float t = -(dot(p->n, orig) + p->d) / denom;
    return (t > 0.0f) ? t : -1.0f;
}
