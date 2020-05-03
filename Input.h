#pragma once

#include <assert.h>
#include "Window.h"

#include <cglm.h>

typedef struct
{
	GLFWwindow* window;
	int* keys;
	int* mouseButtons;

}Input;

Input* NewInput(GLFWwindow* window)
{
	Input* self = (Input*)malloc(sizeof(Input));
	assert(self != NULL);

	self->window = window;
	self->keys = (int*)malloc(sizeof(int) * GLFW_KEY_LAST);
	self->mouseButtons = (int*)malloc(sizeof(int) * GLFW_MOUSE_BUTTON_LAST);

	return self;
}

int InputIsKeyDown(Input* self, int key)
{
	return glfwGetKey(self->window, key) == 1;
}

int InputIsKeyPressed(Input* self, int key)
{
	return (InputIsKeyDown(self, key) && !self->keys[key]);
}

int InputIsKeyReleased(Input* self, int key)
{
	return (!InputIsKeyDown(self, key) && self->keys[key]);
}

int InputIsMouseButtonDown(Input* self, int button)
{
	return glfwGetMouseButton(self->window, button) == 1;
}

int InputIsMouseButtonPressed(Input* self, int button)
{
	return (InputIsMouseButtonDown(self, button) && !self->mouseButtons[button]);
}

int InputIsMouseButtonReleased(Input* self, int button)
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

void InputGetMousePos(Input* self, vec2 dest)
{
	glfwGetCursorPos(self->window, (float*)&dest[0], (float*)&dest[1]);
}

void InputTerminate(Input* self)
{
	free(self->keys);
	free(self->mouseButtons);
	free(self);
}
