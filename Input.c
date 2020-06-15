#include "Input.h"

void Insight_InputCreate(Insight_Input* self, GLFWwindow* window)
{
	if (self == NULL)
	{
		fprintf(stderr, "[Error]: 'self', cannot be 'NULL'. File: %s:%d\n", __FILE__, __LINE__);
		return;
	}

	self->window = window;
}

_Bool Insight_IsKeyDown(Insight_Input* self, int key)
{
	return glfwGetKey(self->window, key) == 1;
}

_Bool Insight_IsKeyPressed(Insight_Input* self, int key)
{
	return (Insight_IsKeyDown(self, key) && !self->keys[key]);
}

_Bool Insight_IsKeyReleased(Insight_Input* self, int key)
{
	return (!Insight_IsKeyDown(self, key) && self->keys[key]);
}

_Bool Insight_IsMouseButtonDown(Insight_Input* self, int button)
{
	return glfwGetMouseButton(self->window, button) == 1;
}

_Bool Insight_IsMouseButtonPressed(Insight_Input* self, int button)
{
	return (Insight_IsMouseButtonDown(self, button) && !self->mouseButtons[button]);
}

_Bool Insight_IsMouseButtonReleased(Insight_Input* self, int button)
{
	return (!Insight_IsMouseButtonDown(self, button) && self->mouseButtons[button]);;
}

void Insight_GetMousePos(Insight_Input* self, double* x, double* y)
{
	glfwGetCursorPos(self->window, x, y);
}

void Insight_InputUpdate(Insight_Input* self)
{
	for (uint16_t i = 32; i < GLFW_KEY_LAST; i++) 
	{
		self->keys[i] = Insight_IsKeyDown(self, i);
	}

	for (uint16_t i = 0; i < GLFW_MOUSE_BUTTON_LAST; i++)
	{

		self->mouseButtons[i] = Insight_IsMouseButtonDown(self, i);
	}
}

void Insight_InputTerminate(Insight_Input* self)
{
	self->window = NULL;
	self = NULL;
}
