#ifndef _INSIGHT_H_
#define _INSIGHT_H_

#define GLFW_DLL

#include "Window.h"

#include <intrin.h>

const char* insight_cpu_info();

_Bool insight_init(const char* title);

void insight_update();

void insight_terminate();

/*! Function given by the user to set up variables, behaviour or similar. */
extern void (*insight_init_ptr)(window_t* wnd);

/*! Function given by the user to update variables, behaviour or similar. */
extern void (*insight_update_ptr)(void);

/*! Function given by the user to draw. */
extern void (*insight_draw_ptr)(void);

/*! Function given by the user, to handle input. */
extern void (*insight_input_ptr)(window_t* wnd);

/*! Function given by the user, to free memory and end executions. */
extern void (*insight_terminate_ptr)(void);

#endif // !_INSIGHT_H_

