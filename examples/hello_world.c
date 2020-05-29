#include "Window.h"

int main(void) {
   
   	Window w;
	NewWindow(&w, 1280, 720, "", false);
   
	while (WindowIsRunning(&w)) {
		glClear(GL_COLOR_BUFFER_BIT);
		glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
	}
    	
	WindowTerminate(&w);
    	return 0;
}
