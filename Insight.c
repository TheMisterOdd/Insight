#include "Insight.h"

#include <malloc.h>
#include <stddef.h>
#include <string.h>

#include <stdio.h>

static window_t* insight_wnd;

static char* CPU_INFO;

void (*insight_init_ptr)(window_t* wnd) = NULL;

void (*insight_update_ptr)(void) = NULL;

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

_Bool insight_init(const char* title)
{
	if (!glfwInit() || !window_get_system_info())
	{
		return 0;
	}
	insight_wnd = insight_window_init(1280, 720, title, 0);
	if (insight_wnd == NULL)
		return 0;

	insight_init_ptr(insight_wnd);

	return 1;
}

void insight_update() 
{
	while (window_is_running(insight_wnd))
	{
		printf("\rDisplay: %dx%d, FPS: %.0f", insight_wnd->width, insight_wnd->height, 1.0f / insight_wnd->deltaTime);
		insight_update_ptr();
		insight_draw_ptr();
		insight_input_ptr(insight_wnd);
	}
}


void insight_terminate() 
{
	insight_terminate_ptr();
	window_terminate(insight_wnd);
}

const char* insight_cpu_info()
{
	return CPU_INFO;
}

