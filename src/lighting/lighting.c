#include "lighting.h"
#include "../scene/scene.h"
#include "../ray/ray.h"
#include <math.h>

#define SHADOWS_ENABLED 1
#define MAX_REFLECTION_DEPTH 3

static int REFLECTIONS_ENABLED = 0;

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
    vec3 ambient_color = V(material->color.x * light->color.x,
                           material->color.y * light->color.y,
                           material->color.z * light->color.z);
    return mul(ambient_color, light->intensity * material->ambient);
}

static vec3 calculate_directional_light(const Light *light, const ShadingInfo *shading)
{
    vec3 light_dir = norm(mul(light->direction, -1.0f)); // Reverse direction

    // shadows
    if (is_in_shadow(shading->point, shading->normal, light_dir, 1000.0f))
    {
        return V(0, 0, 0);
    }

    // Diffuse
    float diff = fmaxf(dot(shading->normal, light_dir), 0.0f);
    vec3 diffuse_color = V(shading->material.color.x * light->color.x,
                           shading->material.color.y * light->color.y,
                           shading->material.color.z * light->color.z);
    vec3 diffuse = mul(diffuse_color, shading->material.diffuse * diff * light->intensity);

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

    // shadows
    if (is_in_shadow(shading->point, shading->normal, light_dir, distance))
    {
        return V(0, 0, 0);
    }

    // diffuse
    float diff = fmaxf(dot(shading->normal, light_dir), 0.0f);
    vec3 diffuse_color = V(shading->material.color.x * light->color.x,
                           shading->material.color.y * light->color.y,
                           shading->material.color.z * light->color.z);
    vec3 diffuse = mul(diffuse_color, shading->material.diffuse * diff * light->intensity * attenuation);

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

int is_in_shadow(vec3 point, vec3 normal, vec3 light_dir, float light_distance)
{
    if (!SHADOWS_ENABLED)
        return 0;

    float epsilon = 0.01f; // 0.001f é muito pequeno
    vec3 ray_origin = add(point, mul(normal, epsilon));
    Ray shadow_ray = {ray_origin, light_dir};

    float current_dist = 0.0f;
    const float min_dist = 0.001f;
    const int max_steps = 64;

    for (int step = 0; step < max_steps; ++step)
    {
        if (current_dist >= light_distance - epsilon)
        {
            // reached light
            return 0;
        }

        RayStepResult result = ray_step(&shadow_ray, &current_dist, light_distance, min_dist);

        if (result.hit == 1)
        {
            // hit
            return 1;
        }

        if (result.hit == -1)
        {
            return 0;
        }
    }

    return 0;
}

void set_reflections_enabled(int enabled)
{
    REFLECTIONS_ENABLED = enabled;
}

int get_reflections_enabled(void)
{
    return REFLECTIONS_ENABLED;
}

vec3 trace_ray(const Ray *ray, int depth, float max_dist, float min_dist)
{
    if (depth <= 0)
    {
        return V(0, 0, 0); // black
    }

    RayMarchResult march_result = ray_march(ray, max_dist, min_dist);

    if (!march_result.hit || !march_result.hit_body)
    {
        return V(0, 0, 0);
    }

    vec3 hit_point = march_result.hit_point;
    Body *hit_body = march_result.hit_body;
    vec3 normal = calculate_normal(hit_point, hit_body);
    vec3 view_dir = norm(sub(ray->origin, hit_point));

    ShadingInfo shading = {
        .point = hit_point,
        .normal = normal,
        .view_dir = view_dir,
        .material = hit_body->material};

    // base lighting
    int light_count;
    const Light *scene_lights = scene_get_lights(&light_count);
    vec3 base_color = calculate_lighting(&shading, scene_lights, light_count);

    // distance attenuation
    float attenuation = 1.0f / (1.0f + march_result.distance * 0.05f);
    base_color = mul(base_color, attenuation);

    // reflections and reflective material
    if (REFLECTIONS_ENABLED && hit_body->material.reflectivity > 0.0f)
    {
        // reflection ray
        vec3 incident = norm(ray->direction);
        vec3 reflect_dir = sub(incident, mul(normal, 2.0f * dot(incident, normal)));

        // origin offser
        vec3 reflect_origin = add(hit_point, mul(normal, 0.01f));
        Ray reflect_ray = {reflect_origin, reflect_dir};

        // recursive trace
        vec3 reflect_color = trace_ray(&reflect_ray, depth - 1, max_dist, min_dist);

        // combine colors
        float reflectivity = hit_body->material.reflectivity;
        base_color = add(mul(base_color, 1.0f - reflectivity), mul(reflect_color, reflectivity));
    }

    return base_color;
}