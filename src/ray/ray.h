#ifndef RAY_H
#define RAY_H
#include "../core/vec3.h"

typedef struct
{
    vec3 origin;
    vec3 direction;
} Ray;

int ray_step(const Ray *ray, float *current_dist, float max_dist, float min_dist);

#endif
