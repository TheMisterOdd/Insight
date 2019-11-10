#pragma once

#include "Window.h"

typedef struct
{
	GLFWwindow* window;
	bool* keys;
	bool* mouseButtons;

}Input;

Input* mk_Input(GLFWwindow* window)
{
	Input* self = (Input*)malloc(sizeof(Input));

	self->window = window;
	self->keys = (bool*)malloc(sizeof(bool) * GLFW_KEY_LAST);
	self->mouseButtons = (bool*)malloc(sizeof(bool) * GLFW_MOUSE_BUTTON_LAST);

	return self;
}

bool InputIsKeyDown(Input* self, int key)
{
	return glfwGetKey(self->window, key) == 1;
}

bool InputIsKeyPressed(Input* self, int key)
{
	return (InputIsKeyDown(self, key) && !self->keys[key]);
}

bool InputIsKeyReleased(Input* self, int key)
{
	return (!InputIsKeyDown(self, key) && self->keys[key]);
}

bool InputIsMouseButtonDown(Input* self, int button)
{
	return glfwGetMouseButton(self->window, button) == 1;
}

bool InputIsMouseButtonPressed(Input* self, int button)
{
	return (InputIsMouseButtonDown(self, button) && !self->mouseButtons[button]);
}

bool InputIsMouseButtonReleased(Input* self, int button)
{
	return (!InputIsMouseButtonDown(self, button) && self->mouseButtons[button]);
}

void InputUpdate(Input* self)
{
	for (int i = 32; i < GLFW_KEY_LAST; i++)
	{
		self->keys[i] = InputIsKeyDown(self, i);
	}

	for (int i = 0; i < GLFW_MOUSE_BUTTON_LAST; i++)
	{
		self->mouseButtons[i] = InputIsMouseButtonDown(self, i);
	}
}

double* InputGetMousePos(Input* self)
{
	double x, y;
	glfwGetCursorPos(self->window, &x, &y);

	double* position = (double*)malloc(sizeof(double) * 2);

	position[0] = x;
	position[1] = y;

	return position;
}

void InputTerminate(Input* self)
{
	free(self->keys);
	free(self->mouseButtons);
	free(self);
}
