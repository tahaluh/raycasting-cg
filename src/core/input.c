#include "input.h"
#include "camera.h"
#include "../lighting/lighting.h"
#include <GL/glut.h>
#include <stdio.h>

static InputState g_input_state = {0};
extern Camera g_camera; // in render.c

void input_init(void)
{
    g_input_state.w_pressed = false;
    g_input_state.a_pressed = false;
    g_input_state.s_pressed = false;
    g_input_state.d_pressed = false;
    g_input_state.up_pressed = false;
    g_input_state.down_pressed = false;
    g_input_state.left_pressed = false;
    g_input_state.right_pressed = false;
    g_input_state.q_pressed = false;
    g_input_state.e_pressed = false;
}

InputState *input_get_state(void)
{
    return &g_input_state;
}

void input_keyboard_down(unsigned char key, int x, int y)
{
    switch (key)
    {
    case 'w':
    case 'W':
        g_input_state.w_pressed = true;
        break;
    case 'a':
    case 'A':
        g_input_state.a_pressed = true;
        break;
    case 's':
    case 'S':
        g_input_state.s_pressed = true;
        break;
    case 'd':
    case 'D':
        g_input_state.d_pressed = true;
        break;
    case 'q':
    case 'Q':
        g_input_state.q_pressed = true;
        break;
    case 'e':
    case 'E':
        g_input_state.e_pressed = true;
        break;
    case 'r':
    case 'R':
        set_reflections_enabled(!get_reflections_enabled());
        break;
    case 27:
        exit(0);
        break;
    }
}

void input_keyboard_up(unsigned char key, int x, int y)
{
    switch (key)
    {
    case 'w':
    case 'W':
        g_input_state.w_pressed = false;
        break;
    case 'a':
    case 'A':
        g_input_state.a_pressed = false;
        break;
    case 's':
    case 'S':
        g_input_state.s_pressed = false;
        break;
    case 'd':
    case 'D':
        g_input_state.d_pressed = false;
        break;
    case 'q':
    case 'Q':
        g_input_state.q_pressed = false;
        break;
    case 'e':
    case 'E':
        g_input_state.e_pressed = false;
        break;
    }
}

void input_special_down(int key, int x, int y)
{
    switch (key)
    {
    case GLUT_KEY_UP:
        g_input_state.up_pressed = true;
        break;
    case GLUT_KEY_DOWN:
        g_input_state.down_pressed = true;
        break;
    case GLUT_KEY_LEFT:
        g_input_state.left_pressed = true;
        break;
    case GLUT_KEY_RIGHT:
        g_input_state.right_pressed = true;
        break;
    }
}

void input_special_up(int key, int x, int y)
{
    switch (key)
    {
    case GLUT_KEY_UP:
        g_input_state.up_pressed = false;
        break;
    case GLUT_KEY_DOWN:
        g_input_state.down_pressed = false;
        break;
    case GLUT_KEY_LEFT:
        g_input_state.left_pressed = false;
        break;
    case GLUT_KEY_RIGHT:
        g_input_state.right_pressed = false;
        break;
    }
}

void input_process(float delta_time)
{
    // Movement (WASD)
    if (g_input_state.w_pressed)
    {
        camera_move_forward(&g_camera, delta_time);
    }
    if (g_input_state.s_pressed)
    {
        camera_move_backward(&g_camera, delta_time);
    }
    if (g_input_state.a_pressed)
    {
        camera_move_left(&g_camera, delta_time);
    }
    if (g_input_state.d_pressed)
    {
        camera_move_right(&g_camera, delta_time);
    }

    // Vertical movement (Q/E)
    if (g_input_state.q_pressed)
    {
        camera_move_up(&g_camera, delta_time);
    }
    if (g_input_state.e_pressed)
    {
        camera_move_down(&g_camera, delta_time);
    }

    // Rotation (Arrow keys) - inverted for more intuitive controls
    if (g_input_state.left_pressed)
    {
        camera_rotate_left(&g_camera, delta_time);
    }
    if (g_input_state.right_pressed)
    {
        camera_rotate_right(&g_camera, delta_time);
    }
    if (g_input_state.up_pressed)
    {
        camera_rotate_down(&g_camera, delta_time);
    }
    if (g_input_state.down_pressed)
    {
        camera_rotate_up(&g_camera, delta_time);
    }
}
