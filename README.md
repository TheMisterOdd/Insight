# Insight Engine, free and OpenSource Renderer. Using OpenGL
### 3D cross-platform game framework
Insight is a **C and C++**, multiplatform game framework to create 3D games. It provides a varaity of new classes to handle different aspects like **window, input, shaders, GUI...**

Unfortunately we have not managed to port the engine to Android or HTML, nor translate it into any other language, we will try this in later versions.

### Free and open source
Insight is completely free and open source with no licence. No strings attached, no royalties, nothing. The users' games are theirs, down to the last line of engine code. We don't want to make profit from this project.

### Example code
Easy init:
```c
#include "Insight.h"
#include "Window.h"

int main(void) {
   
   insight_glfw_init();
   window_t* wnd = insight_window_init(1280, 720, "OpenGL", false);
   assert(wnd);
   
   while (window_is_running(wnd)) {

	glClear(GL_COLOR_BUFFER_BIT);
	glClearColor(1.0f, 0.0f, 0.0f, 1.0f);

    }
    
    window_terminate(wnd);
    return 0;
}
```

Insight Init:
```c
#include "Insight.h"

void init(window_t* wnd) { /* Init all your variables and define specific behaviour here!!! */ }

void update() { /* Update all your variables and define specific behaviour here!!! */ }

void has_resized(int width, int height) { /* Check if has window has resized here!!! */ }

void draw() { /* Draw everything here!!! */ }

void input(window_t* wnd) { /* Check input here!!! */ }

void terminate() { /* Delete memory here!!! */ }

int main(void)
{
	insight_init_ptr        = (void*)init;
	insight_update_ptr      = (void*)update;
	insight_has_resized_ptr = (void*)has_resized;
	insight_draw_ptr        = (void*)draw;
	insight_input_ptr       = (void*)input;
	insight_terminate_ptr   = (void*)terminate;
	

	insight_engine("Insight", 0);

	return 0;
}
```
### Output
<img width="854" height="480" src="https://raw.githubusercontent.com/AlKiam/Insight3D/master/image/image-1.png" alt="window output"/>

## Dependencies
  * **GLFW** https://www.glfw.org/
  * **OpenGL** https://glad.dav1d.de/ or http://glew.sourceforge.net/
  * **stb_image** https://github.com/nothings/stb/blob/master/stb_image.h
  * **stb_image_write** https://github.com/nothings/stb/blob/master/stb_image_write.h
  * **cglm** https://github.com/recp/cglm
  
## Compilation (With GCC)
**If you are using GLAD**
```cmd
gcc main.c glad.c -I[path to the includes] -L[path to the linkers] -glfw -opengl32
```
**If you are using GLEW**
```
gcc main.c -I[path to the includes] -L[path to the linkers] -glfw -opengl32 -glew32
```

##### Examples
<img width="854" height="480" src="https://raw.githubusercontent.com/AlKiam/Insight3D/master/image/image-2.png" alt="window output"/>

<img width="854" height="480" src="https://raw.githubusercontent.com/AlKiam/Insight3D/master/image/image-3.png" alt="window output"/>

<img width="854" height="480" src="https://raw.githubusercontent.com/AlKiam/Insight3D/master/image/waves.png" alt="window output"/>

<img width="854" height="480" src="https://raw.githubusercontent.com/AlKiam/Insight3D/master/image/photorealistic.png" alt="window output"/>


