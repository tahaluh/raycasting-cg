#include "camera.h"
#include <math.h>

#ifndef PI
#define PI 3.14159265358979323846f
#endif

void camera_init(Camera *camera)
{
    camera->position = V(0.0f, 0.0f, 0.0f);
    camera->yaw = -90.0f;
    camera->pitch = 0.0f;
    camera->move_speed = 2.5f;
    camera->rotation_speed = 45.0f; // degrees per second

    camera_update_vectors(camera);
}

void camera_update_vectors(Camera *camera)
{
    // front from yaw and pitch
    vec3 front;
    front.x = cosf(camera->yaw * PI / 180.0f) * cosf(camera->pitch * PI / 180.0f);
    front.y = sinf(camera->pitch * PI / 180.0f);
    front.z = sinf(camera->yaw * PI / 180.0f) * cosf(camera->pitch * PI / 180.0f);
    camera->front = norm(front);

    // up
    vec3 world_up = V(0.0f, 1.0f, 0.0f);

    // right and up
    camera->right = norm(cross(camera->front, world_up));
    camera->up = norm(cross(camera->right, camera->front));
}

void camera_move_forward(Camera *camera, float delta_time)
{
    vec3 velocity = mul(camera->front, camera->move_speed * delta_time);
    camera->position = add(camera->position, velocity);
}

void camera_move_backward(Camera *camera, float delta_time)
{
    vec3 velocity = mul(camera->front, camera->move_speed * delta_time);
    camera->position = sub(camera->position, velocity);
}

void camera_move_left(Camera *camera, float delta_time)
{
    vec3 velocity = mul(camera->right, camera->move_speed * delta_time);
    camera->position = sub(camera->position, velocity);
}

void camera_move_right(Camera *camera, float delta_time)
{
    vec3 velocity = mul(camera->right, camera->move_speed * delta_time);
    camera->position = add(camera->position, velocity);
}

void camera_move_up(Camera *camera, float delta_time)
{
    vec3 velocity = mul(camera->up, camera->move_speed * delta_time);
    camera->position = add(camera->position, velocity);
}

void camera_move_down(Camera *camera, float delta_time)
{
    vec3 velocity = mul(camera->up, camera->move_speed * delta_time);
    camera->position = sub(camera->position, velocity);
}

void camera_rotate_left(Camera *camera, float delta_time)
{
    camera->yaw -= camera->rotation_speed * delta_time;
    camera_update_vectors(camera);
}

void camera_rotate_right(Camera *camera, float delta_time)
{
    camera->yaw += camera->rotation_speed * delta_time;
    camera_update_vectors(camera);
}

void camera_rotate_up(Camera *camera, float delta_time)
{
    camera->pitch += camera->rotation_speed * delta_time;
    if (camera->pitch > 89.0f)
        camera->pitch = 89.0f;
    camera_update_vectors(camera);
}

void camera_rotate_down(Camera *camera, float delta_time)
{
    camera->pitch -= camera->rotation_speed * delta_time;
    if (camera->pitch < -89.0f)
        camera->pitch = -89.0f;
    camera_update_vectors(camera);
}

vec3 camera_get_ray_direction(Camera *camera, float screen_x, float screen_y, float aspect, float scale)
{
    float x = screen_x * aspect * scale;
    float y = screen_y * scale;

    vec3 ray_dir = add(camera->front, add(mul(camera->right, x), mul(camera->up, y)));
    return norm(ray_dir);
}