#ifndef INPUT_H
#define INPUT_H

#include <stdbool.h>

// Key states
typedef struct
{
    bool w_pressed;
    bool a_pressed;
    bool s_pressed;
    bool d_pressed;
    bool up_pressed;
    bool down_pressed;
    bool left_pressed;
    bool right_pressed;
    bool q_pressed;
    bool e_pressed;
} InputState;

void input_init(void);

InputState *input_get_state(void);

void input_keyboard_down(unsigned char key, int x, int y);
void input_keyboard_up(unsigned char key, int x, int y);
void input_special_down(int key, int x, int y);
void input_special_up(int key, int x, int y);

void input_process(float delta_time);

#endif