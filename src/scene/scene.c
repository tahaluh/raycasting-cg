#include "scene.h"

sphere_t g_sphere = {
    .c = {0, 0, -3},
    .r = 0.7f,
    .color = {1, 0.2f, 0.2f}};

plane_t g_plane = {
    .n = {0, 1, 0},
    .d = 0.7f,
    .color = {0.3f, 0.9f, 0.3f}};
