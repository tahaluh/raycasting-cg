#ifndef BODY_H
#define BODY_H
#include "../core/vec3.h"
#include "../material/material.h"

typedef struct
{
    vec3 centre;
    float radius;
    Material material;
} Body;

#endif
