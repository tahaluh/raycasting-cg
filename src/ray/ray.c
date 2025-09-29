#include "ray.h"
#include <math.h>
#include "../scene/scene.h"

RayStepResult ray_step(const Ray *ray, float *current_dist, float max_dist, float min_dist)
{
    if (!ray || !current_dist)
        return (RayStepResult){-1, 0};

    vec3 pos = add(ray->origin, mul(ray->direction, *current_dist));
    SdfResult sdf_result = scene_sdf(pos, min_dist);
    float dist_to_surface = sdf_result.min_dist;

    if (dist_to_surface < min_dist) // hit
    {
        return (RayStepResult){1, sdf_result.obj};
    }
    if (*current_dist > max_dist) // miss
    {
        return (RayStepResult){-1, 0};
    }

    // adaptive step size
    float step_size;
    if (dist_to_surface > 0.1f)
    {
        // larger steps
        step_size = dist_to_surface * 0.95f;
    }
    else if (dist_to_surface > 0.01f)
    {
        // moderate steps
        step_size = dist_to_surface * 0.8f;
    }
    else
    {
        // small careful steps
        step_size = dist_to_surface * 0.5f;
    }

    // minimum step
    if (step_size < min_dist)
    {
        step_size = min_dist;
    }

    *current_dist += step_size;
    return (RayStepResult){0, 0};
}
