#ifndef RAY_H
#define RAY_H
#include "../core/vec3.h"
#include "../body/body.h"

typedef struct
{
    vec3 origin;
    vec3 direction;
} Ray;

typedef struct
{
    int hit;
    Body *body;
} RayStepResult;

RayStepResult ray_step(const Ray *ray, float *current_dist, float max_dist, float min_dist);

#endif
