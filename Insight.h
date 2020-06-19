#ifndef _INSIGHT_H_
#define _INSIGHT_H_

#define GLFW_DLL

#include "Window.h"

#include <intrin.h>

const char* insight_cpu_info();

/*! Function that initialices only GLFW. */
bool insight_glfw_init();

/*! Function that initialices Insight, GLFW and create a window with the user given functions. */
void insight_engine(const char* appname, uint8_t flags);

/*! Function given by the user to set up variables, behaviour or similar. */
extern void (*insight_init_ptr)(window_t* wnd);

/*! Function given by the user to update variables, behaviour or similar. */
extern void (*insight_update_ptr)();

/*! Function given by the user that is called, whenever the window is resized. */
extern void (*insight_has_resized_ptr)(int new_width, int new_height);

/*! Function given by the user to draw. */
extern void (*insight_draw_ptr)(void);

/*! Function given by the user, to handle input. */
extern void (*insight_input_ptr)(window_t* wnd);

/*! Function given by the user, to free memory and end executions. */
extern void (*insight_terminate_ptr)(void);

#endif // !_INSIGHT_H_

