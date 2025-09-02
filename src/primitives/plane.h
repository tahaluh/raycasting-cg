#ifndef PLANE_H
#define PLANE_H
#include "../core/vec3.h"

typedef struct
{
    vec3 n;
    float d;
    vec3 color;
} plane_t;

float hit_plane(vec3 orig, vec3 dir, const plane_t *p);

#endif
