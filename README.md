# Insight
Insight is **lightweight game engine and framework** written in **C99** and licensed under public domain. The engine is designed to be fast and could be used in any project and, also, it could be easily implemented with C++. The only dependecies it needs are an OpenGL library and `stb_image` and `stb_image_write header` files.

## Features:
* Lightweight 
* Use only what you need
* Written in C99
* Small codebase (~1kLOC)
* Simple and portable
* Documentation (In development)

## Building
This library is written in a single header file, there is no need of buildage.

You would need to implement the headers files like this:
```c
#define INSIGHT_WINDOW_IMPL /* Implements all the window functions */
#include "Window.h" 
```

## Examples
Insight Init:
```c
#include "Insight.h"

void draw(void* objects, struct window_t* wnd) { 
	
	glClear(GL_COLOR_BUFFER_BIT);
	glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
	
}

int main(void)
{
	insight_set_draw_func(draw);
	insight_init_engine(NULL, "Insight", 0);

	return 0;
}
```
### Output
<img width="854" height="480" src="https://raw.githubusercontent.com/AlKiam/Insight3D/master/image/image-1.png" alt="window output"/>

## Dependencies and Suggestions:
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


