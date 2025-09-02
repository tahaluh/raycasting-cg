#ifndef RENDER_H
#define RENDER_H
#include <stdint.h>

void render_init(int width, int height);
void render_scene(void);
const uint8_t *render_get_frame(void);
void render_free(void);

#endif
