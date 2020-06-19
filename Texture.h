#ifndef _TEXTURE_H_
#define _TEXTURE_H_

#include "Core.h"

#include <glad/glad.h>
#include <stdbool.h>
#include <time.h>

/*! Texture type. */
typedef struct
{
	int width, height, channels;
	unsigned int texture;
} texture_t;

/*! Returns a pointer to a texture object in memory. */
INSIGHT_API texture_t* texture_init(const char* path);

/*! Returns a pointer to a texture object for a skybox in memory. */
INSIGHT_API texture_t* texture_skybox_init(char** faces_files, unsigned int lenght);

/*! Uses the given texture, for handeling it. */
INSIGHT_API void texture_bind(texture_t* self, unsigned int sampler);

/*! Stop using the current binded texture. */
INSIGHT_API void texture_unbind();

/*! Delete the memory of the given texture. */
INSIGHT_API void texture_terminate(texture_t* self);

/*! Makes a screenshot an writes it into a png file. */
INSIGHT_API bool texture_make_screenshot();

#endif // !_TEXTURE_H_


