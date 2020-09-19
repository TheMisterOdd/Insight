#ifndef _INSIGHT_H_
#define _INSIGHT_H_

#define INSIGHT_API

#include "Window.h"

#include <intrin.h>
#include <stdbool.h>
#include <stdint.h>

#define INSIGHT_FLAG(x) (1 << x)

INSIGHT_API enum INSIGHT_WINDOW_FLAGS {
	FULLSCREEN         = INSIGHT_FLAG(0),
	FULLSCREEN_DESKTOP = INSIGHT_FLAG(1),
	MAXIMIZED          = INSIGHT_FLAG(2),
	BORDERLESS         = INSIGHT_FLAG(3),
	RESIZABLE          = INSIGHT_FLAG(4),
};

/*! Function that initialices Insight, GLFW and create a window with the user given functions. */
INSIGHT_API void insight_init(void* const objects, const char* appname, uint8_t flags);

/*! Function given by the user to set up variables, behaviour or similar. */
INSIGHT_API extern void (*insight_init_ptr)(void* objects, int width, int height);

/*! Function given by the user to update variables, behaviour or similar. */
INSIGHT_API extern void (*insight_update_ptr)(void* objects, double dt);

/*! Function given by the user to draw. */
INSIGHT_API extern void (*insight_draw_ptr)(void* objects, double dt);

/*! Function given by the user to handle input. */
INSIGHT_API extern void (*insight_input_ptr)(void* objects, Insight_Input* input, double dt);

/*! Function given by the user to resize the game. */
INSIGHT_API extern void (*insight_resize_ptr)(void* objects, int width, int height);

/*! Function given by the user, to free memory and end executions. */
INSIGHT_API extern void (*insight_exit_ptr)(void* objects);

/*
 * ==============================================================
 *
 *                          IMPLEMENTATION
 *
 * ===============================================================
 */
#ifdef INSIGHT_INSIGHT_IMPL
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


INSIGHT_API void (*insight_init_ptr)(void*, int, int) = NULL;
			 
INSIGHT_API void (*insight_update_ptr)(void*, double) = NULL;
			 
INSIGHT_API void (*insight_draw_ptr)(void*, double) = NULL;
			 
INSIGHT_API void (*insight_input_ptr)(void*, input_t*, double) = NULL;
			 
INSIGHT_API void (*insight_resize_ptr)(void*, int, int) = NULL;
			 
INSIGHT_API void (*insight_exit_ptr)(void*) = NULL;


INSIGHT_API void insight_init(void* objects, const char* appname, uint8_t flags) {
	if (!glfwInit()) {
		fprintf(stderr, "[Error]: Cannot create a GLFW window context.");
		return;
	}


	window_t* wnd = window_init(WINDOW_DEFAULT_WIDTH, WINDOW_DEFAULT_HEIGHT, appname, false);
	if (wnd == NULL) {
		fprintf(stderr, "[Error]: Cannot create a GLFW window context.");
		return;
	}

	/* --- User Init --- */
	insight_init_ptr(objects, wnd->width, wnd->height);
	printf("Load Time: %f\n", glfwGetTime());
	
	while (window_is_running(wnd)) {
		/* --- User Loop Update --- */
		if (window_has_resized) {
			insight_resize_ptr(objects, wnd->width, wnd->height);
			window_has_resized = false;
		}

		insight_update_ptr(objects, wnd->deltaTime);
		insight_input_ptr(objects, wnd->input, wnd->deltaTime);
		insight_draw_ptr(objects, wnd->deltaTime);
	}

	/* --- User Terminate --- */
	insight_exit_ptr(objects);

	window_finalize(wnd);
}

#endif /* INSIGHT_INSIGHT_IMPL */

#endif /* !_INSIGHT_H_ */

