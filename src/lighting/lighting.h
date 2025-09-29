#ifndef LIGHTING_H
#define LIGHTING_H

#include "../core/vec3.h"
#include "../material/material.h"
#include "../body/body.h"

typedef enum
{
    LIGHT_AMBIENT,
    LIGHT_DIRECTIONAL,
    LIGHT_POINT
} LightType;

typedef struct
{
    LightType type;
    vec3 color;
    float intensity;
    vec3 position;  // point lights
    vec3 direction; // directional lights
} Light;

typedef struct
{
    vec3 point; // hit point
    vec3 normal;
    vec3 view_dir; // dir to camera
    Material material;
} ShadingInfo;

vec3 calculate_lighting(const ShadingInfo *shading, const Light *lights, int num_lights);

vec3 calculate_normal(vec3 point, const Body *body);

int is_in_shadow(vec3 point, vec3 normal, vec3 light_dir, float light_distance);

#endif