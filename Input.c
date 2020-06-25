#include "Input.h"

#include <stdio.h>
#include <assert.h>
#include <malloc.h>

INSIGHT_API input_t* insight_input_init(GLFWwindow* window)
{
	input_t* self = (input_t*)malloc(sizeof(input_t));
	assert(self);

	self->window = window;

	return self;
}

INSIGHT_API _Bool input_is_key_down(input_t* self, int key)
{
	return glfwGetKey(self->window, key) == 1;
}

INSIGHT_API _Bool input_is_key_pressed(input_t* self, int key)
{
	return (input_is_key_down(self, key) && !self->keys[key]);
}

INSIGHT_API _Bool input_is_key_released(input_t* self, int key)
{
	return (!input_is_key_down(self, key) && self->keys[key]);
}

INSIGHT_API _Bool input_is_mouse_button_down(input_t* self, int button)
{
	return glfwGetMouseButton(self->window, button) == 1;
}

INSIGHT_API _Bool input_is_mouse_button_pressed(input_t* self, int button)
{
	return (input_is_mouse_button_down(self, button) && !self->mouseButtons[button]);
}

INSIGHT_API _Bool input_is_mouse_button_released(input_t* self, int button)
{
	return (!input_is_mouse_button_down(self, button) && self->mouseButtons[button]);;
}

INSIGHT_API void input_get_mouse_pos(input_t* self, double* x, double* y)
{
	glfwGetCursorPos(self->window, x, y);
}

INSIGHT_API void input_update(input_t* self)
{
	int i;
	for (i = 32; i < GLFW_KEY_LAST; i++)
	{
		self->keys[i - 32] = input_is_key_down(self, i);
	}

	for (i = 0; i < GLFW_MOUSE_BUTTON_LAST; i++)
	{

		self->mouseButtons[i] = input_is_mouse_button_down(self, i);
	}
}

INSIGHT_API void input_terminate(input_t* self)
{
	self->window = NULL;
	free(self);
}
