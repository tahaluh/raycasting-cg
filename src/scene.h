#ifndef SCENE_H
#define SCENE_H
#include "vec3.h"

typedef struct
{
    vec3 c;
    float r;
    vec3 color;
} sphere_t;

extern sphere_t g_sphere;

float hit_sphere(vec3 orig, vec3 dir, const sphere_t *s);

#endif
