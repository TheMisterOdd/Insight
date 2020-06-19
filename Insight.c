#include "Insight.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static char* CPU_INFO;

void (*insight_init_ptr)(window_t* wnd) = NULL;

void (*insight_update_ptr)() = NULL;

void (*insight_has_resized_ptr)(int new_width, int new_height) = NULL;

void (*insight_draw_ptr)(void) = NULL;

void (*insight_input_ptr)(window_t* wnd) = NULL;

void (*insight_terminate_ptr)(void) = NULL;

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

_Bool insight_glfw_init()
{
	return (!glfwInit() || !window_get_system_info());
}

void insight_engine(const char* appname, uint8_t flags)
{
	if (insight_glfw_init()) 
	{
		fprintf(stderr, "[Error]: Cannot create a GLFW window context.");
		return;
	}
	window_t* wnd = insight_window_init(1280, 720, appname, 0);
	if (wnd == NULL) 
	{
		fprintf(stderr, "[Error]: Cannot create a GLFW window context.");
		goto terminate;
	}
	
	/* --- User Init --- */
	insight_init_ptr(wnd);

	while (window_is_running(wnd))
	{
		printf("\rDisplay: %dx%d, FPS: %.0f", wnd->width, wnd->height, 1.0f / wnd->deltaTime);
		

		/* --- User Loop Update --- */
		insight_update_ptr();
		insight_draw_ptr();
		insight_input_ptr(wnd);
	}

	/* --- User Terminate --- */
	terminate:
	insight_terminate_ptr();
	window_terminate(wnd);
}


const char* insight_cpu_info()
{
	return CPU_INFO;
}

