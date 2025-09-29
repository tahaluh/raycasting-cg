#ifndef SCENE_H
#define SCENE_H

#include "../core/vec3.h"
#include "../body/body.h"

typedef struct
{
    float min_dist;
    Body *obj;
} SdfResult;

SdfResult scene_sdf(vec3 p);

#endif
