#ifndef _INPUT_H_
#define _INPUT_H_

#include <assert.h>
#include <cglm/cglm.h>

#include <GLFW/glfw3.h>

typedef struct 
{
	GLFWwindow* window;
	_Bool keys[GLFW_KEY_LAST];
	_Bool mouseButtons[GLFW_MOUSE_BUTTON_LAST];

} Insight_Input;

void Insight_InputCreate(Insight_Input* self, GLFWwindow* window);

_Bool Insight_IsKeyDown(Insight_Input* self, int key);

_Bool Insight_IsKeyPressed(Insight_Input* self, int key);

_Bool Insight_IsKeyReleased(Insight_Input* self, int key);

_Bool Insight_IsMouseButtonDown(Insight_Input* self, int button);

_Bool Insight_IsMouseButtonPressed(Insight_Input* self, int button);

_Bool Insight_IsMouseButtonReleased(Insight_Input* self, int button);

void Insight_GetMousePos(Insight_Input* self, double* x, double* y);

void Insight_InputUpdate(Insight_Input* self);

void Insight_InputTerminate(Insight_Input* self);

#endif

