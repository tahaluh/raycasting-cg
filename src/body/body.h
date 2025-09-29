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
    vec3 p1, p2, p3; // Triangle vertices
} TriangleData;

typedef struct
{
    vec3 normal;    // Plane normal
    float distance; // Distance from origin
} PlaneData;

typedef struct
{
    vec3 size; // Half-extents (width/2, height/2, depth/2)
} CubeData;

typedef union
{
    float radius;          // For sphere
    PlaneData plane;       // For plane
    CubeData cube;         // For cube
    TriangleData triangle; // For triangle
} BodyGeometry;

typedef struct
{
    BodyType type;
    vec3 centre;
    BodyGeometry geometry;
    Material material;
} Body;

// Function to calculate SDF for a body
float body_sdf(vec3 p, const Body *body);

#endif
