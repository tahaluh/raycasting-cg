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

SdfResult scene_sdf(vec3 p, float min_threshold);
const Light *scene_get_lights(int *count);

void scene_toggle_animation(void);
void scene_update_animation(void);
int scene_is_animation_paused(void);

void scene_toggle_light(int light_index);
int scene_is_light_enabled(int light_index);

#endif
