#ifndef SPHERE_H
#define SPHERE_H
#include "../core/vec3.h"

typedef struct
{
    vec3 c;
    float r;
    vec3 color;
} sphere_t;

float hit_sphere(vec3 orig, vec3 dir, const sphere_t *s);

#endif
