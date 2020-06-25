#include "Insight.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static char* CPU_INFO;


INSIGHT_API void (*insight_init_ptr)(void*, window_t*) = NULL;

INSIGHT_API void (*insight_update_ptr)(void*, window_t*) = NULL;

INSIGHT_API void (*insight_has_resized_ptr)(void*, int, int) = NULL;

INSIGHT_API void (*insight_draw_ptr)(void*, window_t*) = NULL;

INSIGHT_API void (*insight_input_ptr)(void*, window_t*) = NULL;

INSIGHT_API void (*insight_terminate_ptr)(void*) = NULL;

INSIGHT_API void* _insight_objects = NULL;

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

INSIGHT_API bool insight_glfw_init()
{
	return (!glfwInit() || !window_get_system_info());
}

INSIGHT_API void insight_engine(void* objects, const char* appname, uint8_t flags)
{
	_insight_objects = objects;
	if (insight_glfw_init()) 
	{
		fprintf(stderr, "[Error]: Cannot create a GLFW window context.");
		return;
	}

	bool if_fullscreen = false;
	if (flags & FULLSCREEN) {
		if_fullscreen = true;
	}
	window_t* wnd = insight_window_init(WINDOW_DEFAULT_WIDTH, WINDOW_DEFAULT_HEIGHT, appname, if_fullscreen);

	

	if (wnd == NULL) 
	{
		fprintf(stderr, "[Error]: Cannot create a GLFW window context.");
		goto terminate;
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
	terminate:

	if (insight_terminate_ptr) 
	{
		insight_terminate_ptr(objects);
	}

	window_terminate(wnd);
}

INSIGHT_API const char* insight_cpu_info()
{
	return CPU_INFO;
}

