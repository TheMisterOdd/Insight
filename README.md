# Insight
Insight is lightweight game engine and framework written in ANSI C and licensed under public domain. The engine is designed to be fast and could be used in any project and it could be easily implemented in C++. The only dependecies it needs are an OpenGL library and `stb_image` and `stb_image_write header` files.

## Features:
* Lightweight 
* Use only what you need
* Written in C89, ANSI C
* Small codebase (~1kLOC)
* Simple and portable
* Documentation (In development)

## Building
This library is written in a single header file, there is no need of buildage.

You would need to implement the headers files like this:
```c
#define INSIGHT_WINDOW_IMPL
#include "Window.h"		/* Implements all the window functions */
```

## Examples
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


