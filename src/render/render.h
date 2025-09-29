#ifndef RENDER_H
#define RENDER_H
#include <stdint.h>
#include "../core/vec3.h"

void render_init(int width, int height);
void render_scene(void);
const uint8_t *render_get_frame(void);
void render_free(void);

// Get camera position for debugging
vec3 render_get_camera_position(void);

#endif
