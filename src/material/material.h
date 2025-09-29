#ifndef MATERIAL_H
#define MATERIAL_H
#include "../core/vec3.h"

typedef struct
{
    vec3 color;         // Base color (albedo)
    float ambient;      // Ambient light factor (0.0 - 1.0)
    float diffuse;      // Diffuse reflection factor (0.0 - 1.0)
    float specular;     // Specular reflection factor (0.0 - 1.0)
    float shininess;    // Specular shininess exponent (1.0 - 128.0)
    float reflectivity; // Mirror reflection factor (0.0 - 1.0)
} Material;

#endif
