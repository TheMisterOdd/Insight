#ifndef _WINDOW_H_
#define _WINDOW_H_

#define GLFW_DLL

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdbool.h>
#include <string.h>

#include "Input.h"

#define WINDOW_DEFAULT_WIDTH 854
#define WINDOW_DEFAULT_HEIGHT 480

typedef struct {
	/*! The width, in screen coordinates. */
	int width;

	/*! The height, in screen coordinates. */
	int height;

	/*! The fullscreen, gives a value depending if the screen is on fullscreen or not. */
	bool fullscreen;

	double deltaTime, lastTime;

	/*! Mouse, keyboard and controller handeler. */
	Insight_Input* input;

	/*! Window object. */
	GLFWwindow* wnd_hndl;

	/*! Video mode type. */
	const GLFWvidmode* vidMode;

} Insight_Window;

#define INSIGHT_TRUE  1
#define INSIGHT_FALSE 0

/*! Returns a pointer to a window in memory */
INSIGHT_API Insight_Window* insight_window(int width, int height, const char* title, int fullscreen);

/*! Checks if the window is running or not */
INSIGHT_API int insight_window_running(Insight_Window* self);

/*! Sets new size of the window */
INSIGHT_API void insight_window_size(Insight_Window* self, int width, int height);

/*! Sets if the window should close or not */
INSIGHT_API void insight_set_window_should_close(Insight_Window* self, int value);

/*! The memory of the window is freed */
INSIGHT_API void insight_window_finalize(Insight_Window* self);



INSIGHT_API int window_has_resized;
/*
 * ==============================================================
 *
 *                          IMPLEMENTATION
 *
 * ===============================================================
 */
#ifdef INSIGHT_WINDOW_IMPL

#include <stdio.h>
#include <malloc.h>

static void window_resize_callback(GLFWwindow* window, int fbW, int fbH) {
	glViewport(0, 0, fbW, fbH);
	window_has_resized = true;
}

static void window_error_callback(int err, const char* description) {
	fprintf(stderr, "Error: %d: %s\n", err, description);
}


INSIGHT_API Insight_Window* insight_window(int width, int height, const char* title, int fullscreen) {
	Insight_Window* self = (Insight_Window*)malloc(sizeof(Insight_Window));
	if (self == NULL) {
		fprintf(stderr, "[Error]: malloc() failed to alloc window. File: %s:%d\n", __FILE__, __LINE__);
		return NULL;
	}

	self->width = width;
	self->height = height;
	self->fullscreen = title;

	self->vidMode = glfwGetVideoMode(glfwGetPrimaryMonitor());

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 6);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


	self->wnd_hndl = glfwCreateWindow(width, height, title, fullscreen ? glfwGetPrimaryMonitor() : NULL, NULL);
	if (self->wnd_hndl == NULL) {
		fprintf(stderr, "[Error]: Cannot create a GLFW window context.");
		return NULL;
	}

	glfwSetWindowPos(self->wnd_hndl, (self->vidMode->width - self->width) / 2, (self->vidMode->height - self->height) / 2);
	glfwSetFramebufferSizeCallback(self->wnd_hndl, window_resize_callback);
	glfwSetErrorCallback(window_error_callback);


	self->input = insight_mkinput(self->wnd_hndl);

	glfwMakeContextCurrent(self->wnd_hndl);

	if (!gladLoadGL()) {
		fprintf(stderr, "Could not create a OpenGL context\n");
		return NULL;
	}

	/* OpenGL Related: */
#ifdef _DEBUG
	printf(
		"OpenGL %s\nGLFW %s\n%s\n%s\n",
		glGetString(GL_VERSION),
		glfwGetVersionString(),
		glGetString(GL_RENDERER),
		glGetString(GL_VENDOR)
	);
#endif
	return self;
}

INSIGHT_API int insight_window_running(Insight_Window* self) {
	double currentTime = glfwGetTime();
	self->deltaTime = currentTime - self->lastTime;
	self->lastTime = currentTime;

	insight_inputupdate(self->input);

	glFlush();
	glfwSwapBuffers(self->wnd_hndl);
	glfwPollEvents();

	glfwGetFramebufferSize(self->wnd_hndl, &self->width, &self->height);
	return !glfwWindowShouldClose(self->wnd_hndl);
}

INSIGHT_API void insight_window_size(Insight_Window* self, int width, int height) {
	glfwSetWindowSize(self->wnd_hndl, width, height);
	glfwSetWindowPos(self->wnd_hndl, (self->vidMode->width - width) / 2, (self->vidMode->height - height) / 2);
}

INSIGHT_API void insight_set_window_should_close(Insight_Window* self, int value) {
	glfwSetWindowShouldClose(self->wnd_hndl, value);
}

INSIGHT_API void insight_window_finalize(Insight_Window* self) {
	insight_inputfinalize(self->input);
	glfwDestroyWindow(self->wnd_hndl);
	free(self);
}


#endif /* !INSIGHT_WINDOW_IMPL */

#endif /* !_WINDOW_H_ */
