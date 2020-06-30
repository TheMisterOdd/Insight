#ifndef _WINDOW_H_
#define _WINDOW_H_

#define GLFW_DLL

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdbool.h>

#include "Input.h"

#define WINDOW_DEFAULT_WIDTH 1280
#define WINDOW_DEFAULT_HEIGHT 720

struct window_t
{
	/*! The width, in screen coordinates. */
	int width;

	/*! The height, in screen coordinates. */
	int height;

	/*! The fullscreen, gives a value depending if the screen is on fullscreen or not. */
	bool fullscreen;

	double deltaTime, lastTime;

	/*! Mouse, keyboard and controller handeler. */
	input_t* input;

	/*! Window object. */
	GLFWwindow* wnd_hndl;

	/*! Video mode type. */
	const GLFWvidmode* vidMode;

};

/*! Returns a pointer to a window in memory */
INSIGHT_API struct window_t* window_init(int width, int height, const char* title, bool fullscreen);

/*! Checks if the window is running or not */
INSIGHT_API bool window_is_running(struct window_t* self);

/*! Sets new size of the window */
INSIGHT_API void window_set_size(struct window_t* self, int width, int height);

/*! The memory of the window is freed */
INSIGHT_API void window_finalize(struct window_t* self);

INSIGHT_API bool window_has_resized;
/*
 * ==============================================================
 *
 *                          IMPLEMENTATION
 *
 * ===============================================================
 */
#ifdef INSIGHT_WINDOW_IMPL

static void window_resize_callback(GLFWwindow* window, int fbW, int fbH)
{
	glViewport(0, 0, fbW, fbH);
	window_has_resized = true;
}

static void window_error_callback(int err, const char* description)
{
	fprintf(stderr, "Error: %d: %s\n", err, description);
}


INSIGHT_API struct window_t* window_init(int width, int height, const char* title, bool fullscreen)
{
	struct window_t* self = (struct window_t*)malloc(sizeof(struct window_t));
	if (self == NULL)
	{
		fprintf(stderr, "[Error]: 'Insight_Window*', cannot be 'NULL'. File: %s:%d\n", __FILE__, __LINE__);
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
	if (self->wnd_hndl == NULL)
	{
		fprintf(stderr, "[Error]: Cannot create a GLFW window context.");
		return NULL;
	}

	glfwSetWindowPos(self->wnd_hndl, (self->vidMode->width - self->width) / 2, (self->vidMode->height - self->height) / 2);
	glfwSetFramebufferSizeCallback(self->wnd_hndl, window_resize_callback);
	glfwSetErrorCallback(window_error_callback);

	self->input = Input(self->wnd_hndl);

	glfwMakeContextCurrent(self->wnd_hndl);

	if (!gladLoadGL())
	{
		fprintf(stderr, "Could not create a OpenGL context\n");
		return NULL;
	}

	/* OpenGL Related: */
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_DEPTH_TEST);


	printf(
		"OpenGL %s\nGLFW %s\n%s\n%s\n",
		glGetString(GL_VERSION),
		glfwGetVersionString(),
		glGetString(GL_RENDERER),
		glGetString(GL_VENDOR)
	);

	return self;
}

INSIGHT_API bool window_is_running(struct window_t* self)
{
	double currentTime = glfwGetTime();
	self->deltaTime = currentTime - self->lastTime;
	self->lastTime = currentTime;

	Input_Update(self->input);

	glFlush();
	glfwSwapBuffers(self->wnd_hndl);
	glfwPollEvents();

	glfwGetFramebufferSize(self->wnd_hndl, &self->width, &self->height);
	return !glfwWindowShouldClose(self->wnd_hndl);
}

INSIGHT_API void window_set_size(struct window_t* self, int width, int height)
{
	glfwSetWindowSize(self->wnd_hndl, width, height);
	glfwSetWindowPos(self->wnd_hndl, (self->vidMode->width - width) / 2, (self->vidMode->height - height) / 2);
}

INSIGHT_API void window_finalize(struct window_t* self)
{
	Input_Finalize(self->input);
	glfwDestroyWindow(self->wnd_hndl);
}


#endif /* !INSIGHT_WINDOW_IMPL */

#endif /* !_WINDOW_H_ */

