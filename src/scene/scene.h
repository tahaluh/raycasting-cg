#ifndef SCENE_H
#define SCENE_H

#include "../core/vec3.h"
#include "../body/body.h"
#include "../lighting/lighting.h"

typedef struct
{
    float min_dist;
    Body *obj;
} SdfResult;

SdfResult scene_sdf(vec3 p);
const Light *scene_get_lights(int *count);

#endif
