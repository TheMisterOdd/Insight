#ifndef _TEXTURE_H_
#define _TEXTURE_H_

#include "Window.h"



#include <time.h>

typedef struct
{
	int width, height, channels;
	unsigned int texture;
} texture_t;

texture_t* texture_init(const char* path);

texture_t* texture_skybox_init(char** faces_files, unsigned int lenght);

void texture_bind(texture_t* self, unsigned int sampler);

void texture_unbind();

void texture_terminate(texture_t* self);

_Bool texture_make_screenshot();

#endif // !_TEXTURE_H_


