#ifndef _INPUT_H_
#define _INPUT_H_

#include "Core.h"

#include <GLFW/glfw3.h>
#include <stdbool.h>

typedef struct 
{
	GLFWwindow* window;
	bool keys[GLFW_KEY_LAST];
	bool mouseButtons[GLFW_MOUSE_BUTTON_LAST];

} input_t;

/*! Returns a pointer to an input object in memory. */
INSIGHT_API input_t* insight_input_init(GLFWwindow* window);

/*! Returns 'true' if a key is pressed and mantained, if not, it returns false. */
INSIGHT_API bool input_is_key_down(input_t* self, int key);

/*! Returns 'true' if a key is pressed, if not, it returns false. */
INSIGHT_API bool input_is_key_pressed(input_t* self, int key);

/*! Returns 'true' if a key is released, if not, it returns false. */
INSIGHT_API bool input_is_key_released(input_t* self, int key);

INSIGHT_API bool input_is_mouse_button_down(input_t* self, int button);

INSIGHT_API bool input_is_mouse_button_pressed(input_t* self, int button);

INSIGHT_API bool input_is_mouse_button_released(input_t* self, int button);

INSIGHT_API void input_get_mouse_pos(input_t* self, double* x, double* y);

INSIGHT_API void input_update(input_t* self);

INSIGHT_API void input_terminate(input_t* self);

#endif

