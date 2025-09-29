#ifndef BODY_H
#define BODY_H
#include "../core/vec3.h"
#include "../material/material.h"

typedef enum
{
    BODY_SPHERE,
    BODY_PLANE,
    BODY_CUBE,
    BODY_TRIANGLE
} BodyType;

typedef struct
{
    vec3 p1, p2, p3; // vertices
} TriangleData;

typedef struct
{
    vec3 normal;
    float distance;
} PlaneData;

typedef struct
{
    vec3 size;
} CubeData;

typedef union
{
    float radius;          // sphere
    PlaneData plane;       // plane
    CubeData cube;         // cube
    TriangleData triangle; // triangle
} BodyGeometry;

typedef struct
{
    BodyType type;
    vec3 centre;
    BodyGeometry geometry;
    Material material;
} Body;

float body_sdf(vec3 p, const Body *body);

#endif
