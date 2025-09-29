#include "lighting.h"
#include <math.h>

vec3 calculate_normal(vec3 point, const Body *body)
{
    float epsilon = 0.001f;
    vec3 normal = V(
        body_sdf(add(point, V(epsilon, 0, 0)), body) - body_sdf(sub(point, V(epsilon, 0, 0)), body),
        body_sdf(add(point, V(0, epsilon, 0)), body) - body_sdf(sub(point, V(0, epsilon, 0)), body),
        body_sdf(add(point, V(0, 0, epsilon)), body) - body_sdf(sub(point, V(0, 0, epsilon)), body));
    return norm(normal);
}

static vec3 calculate_ambient_light(const Light *light, const Material *material)
{
    return mul(mul(material->color, light->intensity), material->ambient);
}

static vec3 calculate_directional_light(const Light *light, const ShadingInfo *shading)
{
    vec3 light_dir = norm(mul(light->direction, -1.0f)); // Reverse direction

    // Diffuse
    float diff = fmaxf(dot(shading->normal, light_dir), 0.0f);
    vec3 diffuse = mul(mul(shading->material.color, shading->material.diffuse), diff * light->intensity);

    // Specular (Phong)
    vec3 reflect_dir = sub(mul(shading->normal, 2.0f * dot(light_dir, shading->normal)), light_dir);
    float spec = powf(fmaxf(dot(shading->view_dir, reflect_dir), 0.0f), shading->material.shininess);
    vec3 specular = mul(light->color, shading->material.specular * spec * light->intensity);

    return add(diffuse, specular);
}

static vec3 calculate_point_light(const Light *light, const ShadingInfo *shading)
{
    vec3 light_dir = norm(sub(light->position, shading->point));
    float distance = sqrtf(dot(sub(light->position, shading->point), sub(light->position, shading->point)));
    float attenuation = 1.0f / (1.0f + 0.1f * distance + 0.01f * distance * distance);

    // diffuse
    float diff = fmaxf(dot(shading->normal, light_dir), 0.0f);
    vec3 diffuse = mul(mul(shading->material.color, shading->material.diffuse), diff * light->intensity * attenuation);

    // specular (Phong)
    vec3 reflect_dir = sub(mul(shading->normal, 2.0f * dot(light_dir, shading->normal)), light_dir);
    float spec = powf(fmaxf(dot(shading->view_dir, reflect_dir), 0.0f), shading->material.shininess);
    vec3 specular = mul(light->color, shading->material.specular * spec * light->intensity * attenuation);

    return add(diffuse, specular);
}

vec3 calculate_lighting(const ShadingInfo *shading, const Light *lights, int num_lights)
{
    vec3 final_color = V(0, 0, 0);

    for (int i = 0; i < num_lights; i++)
    {
        switch (lights[i].type)
        {
        case LIGHT_AMBIENT:
            final_color = add(final_color, calculate_ambient_light(&lights[i], &shading->material));
            break;
        case LIGHT_DIRECTIONAL:
            final_color = add(final_color, calculate_directional_light(&lights[i], shading));
            break;
        case LIGHT_POINT:
            final_color = add(final_color, calculate_point_light(&lights[i], shading));
            break;
        }
    }

    return final_color;
}