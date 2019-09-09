#include "Window.h"

int main(void) {
   
   Window* window = mk_Window(1280, 720, "OpenGL", false);
   
	while (WindowIsRunning(window)) {
		glClear(GL_COLOR_BUFFER_BIT);
		glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
	}
    
    return 0;
}