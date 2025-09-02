#include "ray.h"
#include <math.h>
#include "../scene/scene.h"

int ray_step(const Ray *ray, float *current_dist, float max_dist, float min_dist)
{
    if (!ray || !current_dist)
        return -1;

    vec3 pos = add(ray->origin, mul(ray->direction, *current_dist));
    float dist_to_surface = scene_sdf(pos);

    if (dist_to_surface < min_dist) // hit
    {
        return 1;
    }
    if (*current_dist > max_dist) // miss
    {
        return -1;
    }

    *current_dist += dist_to_surface;
    return 0;
}
