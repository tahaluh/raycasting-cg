#include "ray.h"
#include <math.h>
#include "../scene/scene.h"

RayStepResult ray_step(const Ray *ray, float *current_dist, float max_dist, float min_dist)
{
    if (!ray || !current_dist)
        return (RayStepResult){-1, 0};

    vec3 pos = add(ray->origin, mul(ray->direction, *current_dist));
    SdfResult sdf_result = scene_sdf(pos);
    float dist_to_surface = sdf_result.min_dist;

    if (dist_to_surface < min_dist) // hit
    {
        return (RayStepResult){1, sdf_result.obj};
    }
    if (*current_dist > max_dist) // miss
    {
        return (RayStepResult){-1, 0};
    }

    *current_dist += dist_to_surface;
    return (RayStepResult){0, 0};
}
