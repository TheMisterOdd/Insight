#ifndef _WINDOW_H_
#define _WINDOW_H_

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Input.h"

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

	/*! Mouse, keyboard and controller handeler. */
	input_t* input;

	/*! Window object. */
	GLFWwindow* wnd_hndl;

	/*! Video mode type. */
	const GLFWvidmode* vidMode;

} window_t;

/*! Returns a pointer to a window in memory */
window_t* insight_window_init(int width, int height, const char* title, _Bool fullscreen);

/*! Checks if the window is running or not */
_Bool window_is_running(window_t* self);

/*! Sets new size of the window */
void window_set_size(window_t* self, int width, int height);

/*! The memory of the window is freed */
void window_terminate(window_t* self);

/*! Sets window cursor */
GLFWcursor* window_set_cursor(window_t* self, const char* path);

/*! Sets window icon */
void window_set_icon(window_t* self, const char* path);

#endif