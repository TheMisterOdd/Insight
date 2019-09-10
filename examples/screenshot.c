#include "Window.h"
#include "Texture.h"

int main(void) {
   
   Window* window = mk_Window(1280, 720, "OpenGL", false);
   
	while (WindowIsRunning(window)) 
	{
		glClear(GL_COLOR_BUFFER_BIT);
		glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
		
		if (Screenshot(window)) // Take an screenshoot from the buffer and checks if it was succesfully taken
			break; // if the screenshot was success, breaks the loop
	}
    
    return 0;
}
