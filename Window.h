#ifndef _WINDOW_H_
#define _WINDOW_H_

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Input.h"

static char* CPU_INFO;

typedef struct
{
	/*! The width, in screen coordinates. */
	int width;

	/*! The height, in screen coordinates. */
	int height;

	/*! The fullscreen, gives a value depending if the screen is on fullscreen or not. */
	_Bool fullscreen;

	float deltaTime;
	float lastTime;

	/*! Mouse and keyboard handeler. */
	Insight_Input input;

	/*! Window object. */
	GLFWwindow* wnd_hndl;

	/*! Video mode type. */
	const GLFWvidmode* vidMode;

} Insight_Window;

_Bool Insight_Init();

Insight_Window* Insight_NewWindow(int width, int height, const char* title, _Bool fullscreen);

_Bool Insight_WindowIsRunning(Insight_Window* self);

void Insight_WindowSetSize(Insight_Window* self, int width, int height);

void Insight_WindowTerminate(Insight_Window* self);

GLFWcursor* Insight_WindowSetCursor(Insight_Window* self, const char* path);

void Insight_WindowSetIcon(Insight_Window* self, const char* path);

#endif