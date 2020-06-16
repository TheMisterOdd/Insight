#ifndef _INPUT_H_
#define _INPUT_H_

#include <GLFW/glfw3.h>

typedef struct 
{
	GLFWwindow* window;
	_Bool keys[GLFW_KEY_LAST];
	_Bool mouseButtons[GLFW_MOUSE_BUTTON_LAST];

} input_t;

input_t* insight_input_init(GLFWwindow* window);

_Bool input_is_key_down(input_t* self, int key);

_Bool input_is_key_pressed(input_t* self, int key);

_Bool input_is_key_released(input_t* self, int key);

_Bool input_is_mouse_button_down(input_t* self, int button);

_Bool input_is_mouse_button_pressed(input_t* self, int button);

_Bool input_is_mouse_button_released(input_t* self, int button);

void input_get_mouse_pos(input_t* self, double* x, double* y);

void input_update(input_t* self);

void input_terminate(input_t* self);

#endif

