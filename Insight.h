#ifndef _INSIGHT_H_
#define _INSIGHT_H_

#define GLFW_DLL

#include "Window.h"
#include "Core.h"

#include <intrin.h>
#include <stdbool.h>
#include <stdint.h>


/*! Get CPU name. */
INSIGHT_API const char* insight_cpu_info();

/*! Function that initialices only GLFW. */
INSIGHT_API bool insight_glfw_init();

/*! Function that initialices Insight, GLFW and create a window with the user given functions. */
INSIGHT_API void insight_engine(const char* appname, uint8_t flags);

/*! Function given by the user to set up variables, behaviour or similar. */
INSIGHT_API extern void (*insight_init_ptr)(window_t* wnd);

/*! Function given by the user to update variables, behaviour or similar. */
INSIGHT_API extern void (*insight_update_ptr)(window_t* wnd);

/*! Function given by the user that is called, whenever the window is resized. */
INSIGHT_API extern void (*insight_has_resized_ptr)(int new_width, int new_height);

/*! Function given by the user to draw. */
INSIGHT_API extern void (*insight_draw_ptr)(void);

/*! Function given by the user, to handle input. */
INSIGHT_API extern void (*insight_input_ptr)(window_t* wnd);

/*! Function given by the user, to free memory and end executions. */
INSIGHT_API extern void (*insight_terminate_ptr)(void);

#endif // !_INSIGHT_H_

