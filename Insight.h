#ifndef _INSIGHT_H_
#define _INSIGHT_H_

#include "Core.h"
#define INSIGHT_WINDOW_IMPL 
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

/*! Get CPU name. */
INSIGHT_API const char* insight_cpu_info();

/*! Function that initialices only GLFW. */
INSIGHT_API bool insight_glfw_init();

/*! Function that initialices Insight, GLFW and create a window with the user given functions. */
INSIGHT_API void insight_engine(void* objects, const char* appname, uint8_t flags);

/*! Function given by the user to set up variables, behaviour or similar. */
INSIGHT_API extern void (*insight_init_ptr)(void* objects, window_t* wnd);

/*! Function given by the user to update variables, behaviour or similar. */
INSIGHT_API extern void (*insight_update_ptr)(void* objects, window_t* wnd);

/*! Function given by the user that is called, whenever the window is resized. */
INSIGHT_API extern void (*insight_has_resized_ptr)(void* objects, int new_width, int new_height);

/*! Function given by the user to draw. */
INSIGHT_API extern void (*insight_draw_ptr)(void* objects, window_t* wnd);

/*! Function given by the user, to handle input. */
INSIGHT_API extern void (*insight_input_ptr)(void* objects, window_t* wnd);

/*! Function given by the user, to free memory and end executions. */
INSIGHT_API extern void (*insight_terminate_ptr)(void* objects);

/*! Pointer. */
INSIGHT_API void* _insight_objects;


#endif // !_INSIGHT_H_

