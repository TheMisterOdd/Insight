#ifndef _INPUT_H_
#define _INPUT_H_

#include <stdbool.h>
#include <GLFW/glfw3.h>

typedef struct {
	GLFWwindow* window;
	bool keys[GLFW_KEY_LAST];
	bool mouseButtons[GLFW_MOUSE_BUTTON_LAST];
} Insight_Input; /* */

/*! Returns a pointer to an input object in memory. */
INSIGHT_API Insight_Input* insight_mkinput(GLFWwindow* window);

/*! Returns 'true' if a key is pressed and mantained, if not, it returns false. */
INSIGHT_API bool insight_iskeydown(Insight_Input* self, int key);

/*! Returns 'true' if a key is pressed, if not, it returns false. */
INSIGHT_API bool insight_iskeypressed(Insight_Input* self, int key);

/*! Returns 'true' if a key is released, if not, it returns false. */
INSIGHT_API bool insight_iskeyreleased(Insight_Input* self, int key);

/*! Returns 'true' if a mouse button is pressed and mantained, if not, it returns false. */
INSIGHT_API bool insight_ismousebuttondown(Insight_Input* self, int button);

/*! Returns 'true' if a mouse button is pressed, if not, it returns false. */
INSIGHT_API bool insight_ismousebuttonpressed(Insight_Input* self, int button);

/*! Returns 'true' if a mouse button is released, if not, it returns false. */
INSIGHT_API bool insight_ismousebuttonreleased(Insight_Input* self, int button);

/*! Gives the x and y screen cordinates of the mouse. */
INSIGHT_API void insight_getmousepos(Insight_Input* self, double* x, double* y);

/*! Updates the given input coordinate. */
INSIGHT_API void insight_inputupdate(Insight_Input* self);

/*! Deletes the memory of the given input pointer. */
INSIGHT_API void insight_inputfinalize(Insight_Input* self);

#ifdef INSIGHT_INPUT_IMPL
#include <stdio.h>
#include <assert.h>
#include <malloc.h>

INSIGHT_API Insight_Input* insight_mkinput(GLFWwindow* window) {
	Insight_Input* self = (Insight_Input*)malloc(sizeof(Insight_Input));
	assert(self);
	self->window = window;

	return self;
}

INSIGHT_API bool insight_iskeydown(Insight_Input* self, int key) {
	return glfwGetKey(self->window, key) == 1;
}

INSIGHT_API bool insight_iskeypressed(Insight_Input* self, int key) {
	return (insight_iskeydown(self, key) && !self->keys[key]);
}

INSIGHT_API bool insight_iskeyreleased(Insight_Input* self, int key) {
	return (!insight_iskeydown(self, key) && self->keys[key]);
}

INSIGHT_API bool insight_ismousebuttondown(Insight_Input* self, int button) {
	return glfwGetMouseButton(self->window, button) == 1;
}

INSIGHT_API bool insight_ismousebuttonpressed(Insight_Input* self, int button) {
	return (insight_ismousebuttondown(self, button) && !self->mouseButtons[button]);
}

INSIGHT_API bool insight_ismousebuttonreleased(Insight_Input* self, int button) {
	return (!insight_ismousebuttondown(self, button) && self->mouseButtons[button]);
}

INSIGHT_API void insight_getmousepos(Insight_Input* self, double* x, double* y) {
	glfwGetCursorPos(self->window, x, y);
}

INSIGHT_API void insight_inputupdate(Insight_Input* self) {
	int i;
	for (i = 32; i < GLFW_KEY_LAST; i++) {
		self->keys[i] = insight_iskeydown(self, i);
	}

	for (i = 0; i < GLFW_MOUSE_BUTTON_LAST; i++) {
		self->mouseButtons[i] = insight_ismousebuttondown(self, i);
	}
}

INSIGHT_API void insight_inputfinalize(Insight_Input* self) {
	self->window = NULL;
	free(self);
}

#endif /* !INSIGHT_INPUT_IMPL */

#endif /* !_INPUT_H_ */

