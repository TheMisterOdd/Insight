#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <errno.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Input.h"

#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#endif

#define len(x) (sizeof(x) / sizeof(*x))

static float deltaTime = 0.0f, lastTime = 0.0f;

static void __GLFWwindowResizeCallback(GLFWwindow* window, int fbW, int fbH)
{
	glViewport(0, 0, fbW, fbH);
}

static void __GLFWwindowErrCallback(int err, const char* description)
{
	fprintf(stderr, "ERROR: %s\n", description);
}

typedef struct
{
	int width, height;
	bool fullscreen;

	GLFWwindow* window;
	GLFWvidmode* vidMode;
	Input* input;

}Window;

void WindowSetCursor(GLFWwindow* window, const char* path)
{
	GLFWimage* image = (GLFWimage*)malloc(sizeof(GLFWimage));
	image[0].pixels = stbi_load(path, &image[0].width, &image[0].height, NULL, STBI_rgb_alpha);
	GLFWcursor* cursor = glfwCreateCursor(image, 0, 0);

	glfwSetCursor(window, cursor);
	free(image);
}

void WindowSetIcon(GLFWwindow* window, const char* path)
{
	GLFWimage* image = (GLFWimage*)malloc(sizeof(GLFWimage));
	image[0].pixels = stbi_load(path, &image[0].width, &image[0].height, NULL, STBI_rgb_alpha);

	glfwSetWindowIcon(window, 1, image);

	free(image);
}

/*! @brief Creates a new window with its context.
 *
 *  @param[in] width Defines the window width.
 *  @param[in] heigth Defines the window height.
 *  @param[in] tile Defines the window title.
 *  @param[in] fullscreen Defines if the is going to be fullscreen or not.

 *  @return The allocated window.
 *
 *  @errors Context could not be created
 */
Window* NewWindow(int width, int height, const char* title, bool fullscreen)
{
	Window* self = (Window*)malloc(sizeof(Window));
	self->width = width;
	self->height = height;
	self->fullscreen = fullscreen;

	if (!glfwInit())  // init glfw
	{
		fprintf(stderr, "Could not create a GLFW context\n");
		return NULL;
	}
	
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_SAMPLES, 8);

	self->window = glfwCreateWindow(self->width, self->height, title, self->fullscreen ? glfwGetPrimaryMonitor() : NULL, NULL);

	if (self->window == NULL)
	{
		fprintf(stderr, "Could not create a GLFW window context\n");
		return NULL;
	}
	self->vidMode = (GLFWvidmode*)glfwGetVideoMode(glfwGetPrimaryMonitor());
	glfwSetWindowPos(self->window, (self->vidMode->width - self->width) / 2, (self->vidMode->height - self->height) / 2);

	glfwSetWindowSizeCallback(self->window, __GLFWwindowResizeCallback);
	glfwSetErrorCallback(__GLFWwindowErrCallback);

	glfwGetFramebufferSize(self->window, &self->width, &self->height);
	self->input = NewInput(self->window);

	glfwMakeContextCurrent(self->window);
	if (!gladLoadGL())
	{
		fprintf(stderr, "Could not create a OpenGL context\n");
		return NULL;
	}

	// OpenGL Related:
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_DEPTH);
	glEnable(GL_MULTISAMPLE);
	glEnable(GL_BLEND);

	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glDepthFunc(GL_LESS);

	// Printing useful information of the APIs
	printf("OpenGL %s\n", glGetString(GL_VERSION));
	printf("GLFW %s\n", glfwGetVersionString());

	// Printing useful hardware specs
	printf("%s\n", glGetString(GL_VENDOR));
	printf("%s\n", glGetString(GL_RENDERER));


	return self;
}

/*! @brief Checks if the window is running and doesn't need to be closed.
 *
 *  @param[in] self Is the window that is going to be used in the function.
 *
 *  @return If the window is running or not.
 */
bool WindowIsRunning(Window* self)
{
	return !glfwWindowShouldClose(self->window);
}

void WindowPollEvents(Window* self)
{
	float currentTime = (float)glfwGetTime();
	deltaTime = currentTime - lastTime;
	lastTime = currentTime;

	InputUpdate(self->input);

	glFlush();
	glfwSwapBuffers(self->window);
	glfwPollEvents();

	glfwGetFramebufferSize(self->window, &self->width, &self->height);
}

void WindowSetSize(Window* self, int width, int height)
{
	glfwSetWindowSize(self->window, width, height);
	glfwSetWindowPos(self->window, (self->vidMode->width - width) / 2, (self->vidMode->height - height) / 2);
}

void WindowTerminate(Window* self) {

	InputTerminate(self->input);

	glfwDestroyWindow(self->window);
	glfwTerminate();

	free(self);
}
