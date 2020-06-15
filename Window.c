#include "Window.h"

#include <stdio.h>
#include <malloc.h>

#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#endif


static void window_resize_callback(GLFWwindow* window, int fbW, int fbH) 
{
	glViewport(0, 0, fbW, fbH);
}

static void window_error_callback(int err, const char* desctiption)
{
	fprintf(stderr, "Error: %d: %s\n", err, desctiption);
}

static int window_get_system_info()
{
	int CPUInfo[4] = { -1 };
	unsigned   nExIds, i = 0;
	char* CPUBrandString = (char*)malloc(sizeof(char) * 0x40);

	// Get the information associated with each extended ID.
	__cpuid(CPUInfo, 0x80000000);
	nExIds = CPUInfo[0];
	for (i = 0x80000000; i <= nExIds; ++i)
	{
		__cpuid(CPUInfo, i);
		// Interpret CPU brand string
		if (i == 0x80000002)
			memcpy(CPUBrandString, CPUInfo, sizeof(CPUInfo));
		else if (i == 0x80000003)
			memcpy(CPUBrandString + 16, CPUInfo, sizeof(CPUInfo));
		else if (i == 0x80000004)
			memcpy(CPUBrandString + 32, CPUInfo, sizeof(CPUInfo));
	}
	memcpy(&CPU_INFO, &CPUBrandString, sizeof(char**));
	
	return CPU_INFO != NULL ? 1 : 0;
}

_Bool Insight_Init() 
{
	if (!glfwInit() || !window_get_system_info())
	{
		return 0;
	}

	return 1;
}

Insight_Window* Insight_NewWindow(int width, int height, const char* title, _Bool fullscreen)
{
	Insight_Window* self = (Insight_Window*)malloc(sizeof(Insight_Window));
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

	Insight_InputCreate(&self->input, self->wnd_hndl);

	glfwMakeContextCurrent(self->wnd_hndl);

	if (!gladLoadGL())
	{
		fprintf(stderr, "Could not create a OpenGL context\n");
		return NULL;
	}

	// OpenGL Related:
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);


	printf(
		"%s\nOpenGL %s\nGLFW %s\n%s\n%s\n",
		CPU_INFO,
		glGetString(GL_VERSION),
		glfwGetVersionString(),
		glGetString(GL_RENDERER),
		glGetString(GL_VENDOR)
	);

	return self;
}

_Bool Insight_WindowIsRunning(Insight_Window* self)
{
	float currentTime = (float)glfwGetTime();
	self->deltaTime = currentTime - self->lastTime;
	self->lastTime = currentTime;

	Insight_InputUpdate(&self->input);

	glFlush();
	glfwSwapBuffers(self->wnd_hndl);
	glfwPollEvents();

	glfwGetFramebufferSize(self->wnd_hndl, &self->width, &self->height);
	return !glfwWindowShouldClose(self->wnd_hndl);
}

void Insight_WindowSetSize(Insight_Window* self, int width, int height)
{
	glfwSetWindowSize(self->wnd_hndl, width, height);
	glfwSetWindowPos(self->wnd_hndl, (self->vidMode->width - width) / 2, (self->vidMode->height - height) / 2);
}

void Insight_WindowTerminate(Insight_Window* self)
{
	Insight_InputTerminate(&self->input);
	glfwDestroyWindow(self->wnd_hndl);
}

GLFWcursor* Insight_WindowSetCursor(Insight_Window* self, const char* path)
{
	GLFWimage* image = (GLFWimage*)malloc(sizeof(GLFWimage));
	image[0].pixels = stbi_load(path, &image[0].width, &image[0].height, NULL, STBI_rgb_alpha);
	GLFWcursor* cursor = glfwCreateCursor(image, 0, 0);

	glfwSetCursor(self->wnd_hndl, cursor);
	free(image);

	return cursor;
}

void Insight_WindowSetIcon(Insight_Window* self, const char* path)
{
	GLFWimage* image = (GLFWimage*)malloc(sizeof(GLFWimage));
	image[0].pixels = stbi_load(path, &image[0].width, &image[0].height, NULL, STBI_rgb_alpha);
	glfwSetWindowIcon(self->wnd_hndl, 1, image);

	free(image);
}
