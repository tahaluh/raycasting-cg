#ifndef CAMERA_H
#define CAMERA_H

#include "../core/vec3.h"

typedef struct
{
    vec3 position;
    vec3 front;
    vec3 up;
    vec3 right;
    float yaw;
    float pitch;
    float move_speed;
    float rotation_speed;
} Camera;

void camera_init(Camera *camera);

void camera_update_vectors(Camera *camera);

// move
void camera_move_forward(Camera *camera, float delta_time);
void camera_move_backward(Camera *camera, float delta_time);
void camera_move_left(Camera *camera, float delta_time);
void camera_move_right(Camera *camera, float delta_time);
void camera_move_up(Camera *camera, float delta_time);
void camera_move_down(Camera *camera, float delta_time);

// rotate
void camera_rotate_left(Camera *camera, float delta_time);
void camera_rotate_right(Camera *camera, float delta_time);
void camera_rotate_up(Camera *camera, float delta_time);
void camera_rotate_down(Camera *camera, float delta_time);

// get ray direction
vec3 camera_get_ray_direction(Camera *camera, float screen_x, float screen_y, float aspect, float scale);

#endif