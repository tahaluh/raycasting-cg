#ifndef RAY_H
#define RAY_H
#include "vec3.h"

typedef struct
{
    vec3 orig;
    vec3 dir;
} ray_t;

static inline vec3 ray_at(ray_t r, float t)
{
    return add(r.orig, mul(r.dir, t));
}

#endif
