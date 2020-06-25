#ifndef _WINDOW_H_
#define _WINDOW_H_

#define GLFW_DLL

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Input.h"
#include "Core.h"

#define WINDOW_DEFAULT_WIDTH 1280
#define WINDOW_DEFAULT_HEIGHT 720

typedef struct
{
	/*! The width, in screen coordinates. */
	int width;

	/*! The height, in screen coordinates. */
	int height;

	/*! The fullscreen, gives a value depending if the screen is on fullscreen or not. */
	bool fullscreen;

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
INSIGHT_API window_t* insight_window_init(int width, int height, const char* title, bool fullscreen);

/*! Checks if the window is running or not */
INSIGHT_API bool window_is_running(window_t* self);

/*! Sets new size of the window */
INSIGHT_API void window_set_size(window_t* self, int width, int height);

/*! The memory of the window is freed */
INSIGHT_API void window_terminate(window_t* self);

/*! Sets window cursor */
INSIGHT_API GLFWcursor* window_set_cursor(window_t* self, const char* path);

/*! Sets window icon */
INSIGHT_API void window_set_icon(window_t* self, const char* path);

#endif