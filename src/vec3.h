#ifndef VEC3_H
#define VEC3_H
#include <math.h>

typedef struct
{
    float x, y, z;
} vec3;

static inline vec3 V(float x, float y, float z)
{
    vec3 a = {x, y, z};
    return a;
}
static inline vec3 add(vec3 a, vec3 b) { return V(a.x + b.x, a.y + b.y, a.z + b.z); }
static inline vec3 sub(vec3 a, vec3 b) { return V(a.x - b.x, a.y - b.y, a.z - b.z); }
static inline vec3 mul(vec3 a, float s) { return V(a.x * s, a.y * s, a.z * s); }
static inline float dot(vec3 a, vec3 b) { return a.x * b.x + a.y * b.y + a.z * b.z; }
static inline vec3 norm(vec3 a)
{
    float l = sqrtf(dot(a, a));
    return l > 0 ? mul(a, 1.0f / l) : a;
}

#endif
