#include "Window.h"
#include "Insight.h"

#include <stdio.h>
#include <malloc.h>

#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#endif


static void window_resize_callback(GLFWwindow* window, int fbW, int fbH) 
{
	glViewport(0, 0, fbW, fbH);
	insight_has_resized_ptr(fbW, fbH);
}

static void window_error_callback(int err, const char* desctiption)
{
	fprintf(stderr, "Error: %d: %s\n", err, desctiption);
}

INSIGHT_API window_t* insight_window_init(int width, int height, const char* title, bool fullscreen)
{
	window_t* self = (window_t*)malloc(sizeof(window_t));
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

	self->input = insight_input_init(self->wnd_hndl);

	glfwMakeContextCurrent(self->wnd_hndl);

	if (!gladLoadGL())
	{
		fprintf(stderr, "Could not create a OpenGL context\n");
		return NULL;
	}

	// OpenGL Related:
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_DEPTH_TEST);


	printf(
		"%s\nOpenGL %s\nGLFW %s\n%s\n%s\n",
		insight_cpu_info(),
		glGetString(GL_VERSION),
		glfwGetVersionString(),
		glGetString(GL_RENDERER),
		glGetString(GL_VENDOR)
	);

	return self;
}

INSIGHT_API bool window_is_running(window_t* self)
{
	float currentTime = (float)glfwGetTime();
	self->deltaTime = currentTime - self->lastTime;
	self->lastTime = currentTime;

	input_update(self->input);

	glFlush();
	glfwSwapBuffers(self->wnd_hndl);
	glfwPollEvents();

	glfwGetFramebufferSize(self->wnd_hndl, &self->width, &self->height);
	return !glfwWindowShouldClose(self->wnd_hndl);
}

INSIGHT_API void window_set_size(window_t* self, int width, int height)
{
	glfwSetWindowSize(self->wnd_hndl, width, height);
	glfwSetWindowPos(self->wnd_hndl, (self->vidMode->width - width) / 2, (self->vidMode->height - height) / 2);
}

INSIGHT_API void window_terminate(window_t* self)
{
	input_terminate(self->input);
	glfwDestroyWindow(self->wnd_hndl);
}

INSIGHT_API GLFWcursor* window_set_cursor(window_t* self, const char* path)
{
	GLFWimage* image = (GLFWimage*)malloc(sizeof(GLFWimage));
	image[0].pixels = stbi_load(path, &image[0].width, &image[0].height, NULL, STBI_rgb_alpha);
	GLFWcursor* cursor = glfwCreateCursor(image, 0, 0);

	glfwSetCursor(self->wnd_hndl, cursor);
	free(image);

	return cursor;
}

INSIGHT_API void window_set_icon(window_t* self, const char* path)
{
	GLFWimage* image = (GLFWimage*)malloc(sizeof(GLFWimage));
	image[0].pixels = stbi_load(path, &image[0].width, &image[0].height, NULL, STBI_rgb_alpha);
	glfwSetWindowIcon(self->wnd_hndl, 1, image);

	free(image);
}
