#ifndef _INSIGHT_H_
#define _INSIGHT_H_

#include "Window.h"
#include "Core.h"

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

typedef void (*Insight_Func)(void* objects, struct window_t* wnd);

/*! Get CPU name. */
INSIGHT_API char* insight_get_cpu();

/*! Function that initialices only GLFW. */
INSIGHT_API bool insight_glfw_init();

/*! Function that initialices Insight, GLFW and create a window with the user given functions. */
INSIGHT_API void insight_init_engine(void* objects, const char* appname, uint8_t flags);

/*! Function that sets the user given pointer into the 'insight_init_ptr' function. See 'insight_init_ptr' for more */
INSIGHT_API void insight_set_init_func(Insight_Func fn);

/*! Function that sets the user given pointer into the 'insight_update_ptr' function. See 'insight_update_ptr' for more */
INSIGHT_API void insight_set_update_func(Insight_Func fn);

/*! Function that sets the user given pointer into the 'insight_draw_ptr' function. See 'insight_draw_ptr' for more */
INSIGHT_API void insight_set_draw_func(Insight_Func fn);

/*! Function that sets the user given pointer into the 'insight_input_ptr' function. See 'insight_input_ptr' for more */
INSIGHT_API void insight_set_input_func(Insight_Func fn);

/*! Function that sets the user given pointer into the 'insight_terminate_ptr' function. See 'insight_terminate_ptr' for more */
INSIGHT_API void insight_set_finalize_func(void (*fn)(void*));

/*! Function given by the user to set up variables, behaviour or similar. */
INSIGHT_API extern void (*insight_init_ptr)(void* objects, struct window_t* wnd);

/*! Function given by the user to update variables, behaviour or similar. */
INSIGHT_API extern void (*insight_update_ptr)(void* objects, struct window_t* wnd);

/*! Function given by the user to draw. */
INSIGHT_API extern void (*insight_draw_ptr)(void* objects, struct window_t* wnd);

/*! Function given by the user, to handle input. */
INSIGHT_API extern void (*insight_input_ptr)(void* objects, struct window_t* wnd);

/*! Function given by the user, to free memory and end executions. */
INSIGHT_API extern void (*insight_finalize_ptr)(void* objects);

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

static char* CPU_INFO;

INSIGHT_API void (*insight_init_ptr)(void*, struct window_t*) = NULL;

INSIGHT_API void (*insight_update_ptr)(void*, struct window_t*) = NULL;

INSIGHT_API void (*insight_draw_ptr)(void*, struct window_t*) = NULL;

INSIGHT_API void (*insight_input_ptr)(void*, struct window_t*) = NULL;

INSIGHT_API void (*insight_finalize_ptr)(void*) = NULL;


static int window_get_system_info()
{
	int CPUInfo[4] = { -1 };
	unsigned   nExIds, i = 0;
	char* CPUBrandString = (char*)malloc(sizeof(char) * 0x40);

	/* Get the information associated with each extended ID. */
	__cpuid(CPUInfo, 0x80000000);
	nExIds = CPUInfo[0];
	for (i = 0x80000000; i <= nExIds; ++i)
	{
		__cpuid(CPUInfo, i);
		/* Interpret CPU brand string */
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

INSIGHT_API char* insight_get_cpu()
{
	return CPU_INFO;
}

INSIGHT_API bool insight_glfw_init()
{
	return (!glfwInit() || !window_get_system_info());
}

INSIGHT_API void insight_init_engine(void* objects, const char* appname, uint8_t flags)
{
	if (insight_glfw_init())
	{
		fprintf(stderr, "[Error]: Cannot create a GLFW window context.");
		return;
	}

	bool if_fullscreen = false;
	if (flags & FULLSCREEN) {
		if_fullscreen = true;
	}

	struct window_t* wnd = window_init(WINDOW_DEFAULT_WIDTH, WINDOW_DEFAULT_HEIGHT, appname, if_fullscreen);
	if (wnd == NULL)
	{
		fprintf(stderr, "[Error]: Cannot create a GLFW window context.");
		return;
	}

	/* --- User Init --- */
	if (insight_init_ptr)
	{
		insight_init_ptr(objects, wnd);
	}

	double time = glfwGetTime();
	printf("Load Time: %f\n", time);
	
	while (window_is_running(wnd))
	{
		if (glfwGetTime() - time > 0.5)
		{
			printf("\rDisplay: %dx%d, FPS: %.0f  ", wnd->width, wnd->height, 1.0f / wnd->deltaTime);
			time = glfwGetTime();
		}


		/* --- User Loop Update --- */
		if (insight_update_ptr)
		{
			insight_update_ptr(objects, wnd);
		}
		if (insight_draw_ptr)
		{
			insight_draw_ptr(objects, wnd);
		}
		if (insight_input_ptr)
		{
			insight_input_ptr(objects, wnd);
		}
	}

	/* --- User Terminate --- */
	if (insight_finalize_ptr)
	{
		insight_finalize_ptr(objects);
	}

	window_finalize(wnd);
}

INSIGHT_API void insight_set_init_func(Insight_Func fn)
{
	insight_init_ptr = fn;
}

INSIGHT_API void insight_set_update_func(Insight_Func fn)
{
	insight_update_ptr = fn;
}

INSIGHT_API void insight_set_draw_func(Insight_Func fn)
{
	insight_draw_ptr = fn;
}

INSIGHT_API void insight_set_input_func(Insight_Func fn)
{
	insight_input_ptr = fn;
}

INSIGHT_API void insight_set_finalize_func(void (*fn)(void*))
{
	insight_finalize_ptr = fn;
}



#endif /* INSIGHT_INSIGHT_IMPL */

#endif /* !_INSIGHT_H_ */

