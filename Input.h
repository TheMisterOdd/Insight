#ifndef _INPUT_H_
#define _INPUT_H_

#include "Core.h"
#include <stdbool.h>
#include <GLFW/glfw3.h>

typedef struct 
{
	GLFWwindow* window;
	bool keys[GLFW_KEY_LAST];
	bool mouseButtons[GLFW_MOUSE_BUTTON_LAST];

} input_t;

/*! Returns a pointer to an input object in memory. */
INSIGHT_API input_t* Input(GLFWwindow* window);

/*! Returns 'true' if a key is pressed and mantained, if not, it returns false. */
INSIGHT_API bool Input_IsKeyDown(input_t* self, int key);

/*! Returns 'true' if a key is pressed, if not, it returns false. */
INSIGHT_API bool Input_IsKeyPressed(input_t* self, int key);

/*! Returns 'true' if a key is released, if not, it returns false. */
INSIGHT_API bool Input_IsKeyReleased(input_t* self, int key);

/*! Returns 'true' if a mouse button is pressed and mantained, if not, it returns false. */
INSIGHT_API bool Input_IsMouseButtonDown(input_t* self, int button);

/*! Returns 'true' if a mouse button is pressed, if not, it returns false. */
INSIGHT_API bool Input_IsMouseButtonPressed(input_t* self, int button);

/*! Returns 'true' if a mouse button is released, if not, it returns false. */
INSIGHT_API bool Input_IsMouseButtonReleased(input_t* self, int button);

/*! Gives the x and y screen cordinates of the mouse. */
INSIGHT_API void Input_GetMousePos(input_t* self, double* x, double* y);

/*! Updates the given input coordinate. */
INSIGHT_API void Input_Update(input_t* self);

/*! Deletes the memory of the given input pointer. */
INSIGHT_API void Input_Finalize(input_t* self);

#ifdef INSIGHT_INPUT_IMPL
#include <stdio.h>
#include <assert.h>
#include <malloc.h>

INSIGHT_API input_t* Input(GLFWwindow* window)
{
	input_t* self = (input_t*)malloc(sizeof(input_t));
	assert(self);

	self->window = window;

	return self;
}

INSIGHT_API bool Input_IsKeyDown(input_t* self, int key)
{
	return glfwGetKey(self->window, key) == 1;
}

INSIGHT_API bool Input_IsKeyPressed(input_t* self, int key)
{
	return (Input_IsKeyDown(self, key) && !self->keys[key]);
}

INSIGHT_API bool Input_IsKeyReleased(input_t* self, int key)
{
	return (!Input_IsKeyDown(self, key) && self->keys[key]);
}

INSIGHT_API bool Input_IsMouseButtonDown(input_t* self, int button)
{
	return glfwGetMouseButton(self->window, button) == 1;
}

INSIGHT_API bool Input_IsMouseButtonPressed(input_t* self, int button)
{
	return (Input_IsMouseButtonDown(self, button) && !self->mouseButtons[button]);
}

INSIGHT_API bool Input_IsMouseButtonReleased(input_t* self, int button)
{
	return (!Input_IsMouseButtonDown(self, button) && self->mouseButtons[button]);;
}

INSIGHT_API void Input_GetMousePos(input_t* self, double* x, double* y)
{
	glfwGetCursorPos(self->window, x, y);
}

INSIGHT_API void Input_Update(input_t* self)
{
	int i;
	for (i = 32; i < GLFW_KEY_LAST; i++)
	{
		self->keys[i] = Input_IsKeyDown(self, i);
	}

	for (i = 0; i < GLFW_MOUSE_BUTTON_LAST; i++)
	{

		self->mouseButtons[i] = Input_IsMouseButtonDown(self, i);
	}
}

INSIGHT_API void Input_Finalize(input_t* self)
{
	self->window = NULL;
	free(self);
}

#endif /* !INSIGHT_INPUT_IMPL */

#endif /* !_INPUT_H_ */

