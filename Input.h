#pragma once

#include "Window.h"

typedef struct
{
	GLFWwindow* window;
	int* keys;
	int* mouseButtons;

}Input;

Input* NewInput(GLFWwindow* window)
{
	Input* self = (Input*)malloc(sizeof(Input));

	self->window = window;
	self->keys = (int*)malloc(sizeof(int) * GLFW_KEY_LAST);
	self->mouseButtons = (int*)malloc(sizeof(int) * GLFW_MOUSE_BUTTON_LAST);

	return self;
}

int InputIsKeyDown(Input* self, uint16_t key)
{
	return glfwGetKey(self->window, key) == 1;
}

int InputIsKeyPressed(Input* self, uint16_t key)
{
	return (InputIsKeyDown(self, key) && !self->keys[key]);
}

int InputIsKeyReleased(Input* self, uint16_t key)
{
	return (!InputIsKeyDown(self, key) && self->keys[key]);
}

int InputIsMouseButtonDown(Input* self, uint8_t button)
{
	return glfwGetMouseButton(self->window, button) == 1;
}

int InputIsMouseButtonPressed(Input* self, uint8_t button)
{
	return (InputIsMouseButtonDown(self, button) && !self->mouseButtons[button]);
}

int InputIsMouseButtonReleased(Input* self, uint8_t button)
{
	return (!InputIsMouseButtonDown(self, button) && self->mouseButtons[button]);
}

void InputUpdate(Input* self)
{
	for (uint16_t i = 32; i < GLFW_KEY_LAST; i++)
	{
		self->keys[i] = InputIsKeyDown(self, i);
	}

	for (uint8_t i = 0; i < GLFW_MOUSE_BUTTON_LAST; i++)
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
