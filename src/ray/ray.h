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

typedef struct
{
    int hit; // 1 = hit, 0 = miss, -1 = error
    Body *hit_body;
    float distance;
    vec3 hit_point;
} RayMarchResult;

RayStepResult ray_step(const Ray *ray, float *current_dist, float max_dist, float min_dist);
RayMarchResult ray_march(const Ray *ray, float max_dist, float min_dist);

#endif
