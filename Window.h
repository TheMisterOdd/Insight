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

static float lastFrame = 0.0f, deltaTime = 0.0f;

static void GLFWwindowResizeCallback(GLFWwindow* window, int fbW, int fbH)
{
	glViewport(0, 0, fbW, fbH);
}

static void GLFWwindowErrCallback(int err, const char* description)
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

const void GLFWwindowSetCursor(GLFWwindow* window, const char* path)
{
	GLFWimage* image = (GLFWimage*)malloc(sizeof(GLFWimage));
	image[0].pixels = stbi_load(path, &image[0].width, &image[0].height, NULL, STBI_rgb_alpha);
	GLFWcursor* cursor = glfwCreateCursor(image, 0, 0);

	glfwSetCursor(window, cursor);
	free(image);
}

const void GLFWwindowSetIcon(GLFWwindow* window, const char* path)
{
	GLFWimage* image = (GLFWimage*)malloc(sizeof(GLFWimage));
	image[0].pixels = stbi_load(path, &image[0].width, &image[0].height, NULL, STBI_rgb_alpha);

	glfwSetWindowIcon(window, 1, image);
	free(image);
}

Window* mk_Window(int width, int height, const char* title, bool fullscreen)
{
	Window* self = (Window*)malloc(sizeof(Window));
	self->width = width;
	self->height = height;
	self->fullscreen = fullscreen;
	
	if (!glfwInit())  // init glfw
	{
		fprintf(stderr, "Could not create a GLFW context\n");
		exit(-1);
	}

	glfwWindowHint(GLFW_SAMPLES, 8);

	self->window = glfwCreateWindow(self->width, self->height, title, self->fullscreen ? glfwGetPrimaryMonitor() : NULL, NULL);
	
	if (self->window == NULL)
	{
		fprintf(stderr, "Could not create a GLFW window context\n");
		exit(-1);
	}
	self->vidMode = (GLFWvidmode*)glfwGetVideoMode(glfwGetPrimaryMonitor());
	glfwSetWindowPos(self->window, (self->vidMode->width - self->width) / 2, (self->vidMode->height - self->height) / 2);

	glfwSetWindowSizeCallback(self->window, GLFWwindowResizeCallback);
	glfwSetErrorCallback(GLFWwindowErrCallback);

	glfwGetFramebufferSize(self->window, &self->width, &self->height);
	self->input = mk_Input(self->window);

	glfwMakeContextCurrent(self->window);
	if (!gladLoadGL())
	{
		fprintf(stderr, "Could not create a OpenGL context\n");
		exit(-1);
	}

	// OpenGL Related:
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_DEPTH);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_MULTISAMPLE);

	// Printing useful information of the APIs
	fprintf(stdout, "OpenGL %s\n", glGetString(GL_VERSION));
	fprintf(stdout, "GLFW %s\n", glfwGetVersionString());

	// Printing useful hardware specs
	fprintf(stdout, "%s\n", glGetString(GL_VENDOR));
	fprintf(stdout, "%s\n", glGetString(GL_RENDERER));


	return self;
}

bool WindowIsRunning(Window* self)
{
	float currentFrame = (float)glfwGetTime();
	deltaTime = currentFrame - lastFrame;
	lastFrame = currentFrame;

	glFlush();
	glfwSwapBuffers(self->window);
	glfwPollEvents();

	glfwGetFramebufferSize(self->window, &self->width, &self->height);

	return !glfwWindowShouldClose(self->window);
}

float GetDelta() 
{
	return deltaTime;
}

void WindowTerminate(Window* self) {

	InputTerminate(self->input);
	free(self->input);

	glfwDestroyWindow(self->window);
	glfwTerminate();
}
