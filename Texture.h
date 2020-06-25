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

/*
 * ==============================================================
 *
 *                          IMPLEMENTATION
 *
 * ===============================================================
 */
#ifdef INSIGHT_TEXTURE_IMPL
#include <stb_image.h>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>


INSIGHT_API texture_t* texture_init(const char* path)
{
	printf("Loading new texture '%s'...\n", path);
	texture_t* self = (texture_t*)malloc(sizeof(texture_t));
	assert(self);

	glGenTextures(1, &self->texture);

	stbi_set_flip_vertically_on_load(1);
	unsigned char* img = stbi_load(path, &self->width, &self->height, &self->channels, STBI_rgb_alpha);
	stbi_set_flip_vertically_on_load(0);

	if (!img)
	{
		fprintf(stderr, "[Error]: Cannot load given file '%s'\n", path);
		return NULL;
	}

	glBindTexture(GL_TEXTURE_2D, self->texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, self->width, self->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, img);
	glGenerateMipmap(GL_TEXTURE_2D);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glBindTexture(GL_TEXTURE_2D, 0);
	stbi_image_free(img);

	return self;
}

INSIGHT_API texture_t* texture_skybox_init(char** faces_files, unsigned int lenght)
{
	printf("Loading new skybox texture...\n");
	texture_t* self = (texture_t*)malloc(sizeof(texture_t));
	unsigned char* img = 0;

	glGenTextures(1, &self->texture);
	glBindTexture(GL_TEXTURE_CUBE_MAP, self->texture);

	unsigned int i;
	for (i = 0; i < lenght; i++)
	{
		img = stbi_load(faces_files[i], &self->width, &self->height, &self->channels, STBI_rgb);
		if (!img)
		{
			fprintf(stderr, "[Error]: Cannot load given file '%s'\n", faces_files[i]);
			return 0;
		}

		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, self->width, self->height, 0, GL_RGB, GL_UNSIGNED_BYTE, img);
		stbi_image_free(img);
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

	return self;
}

INSIGHT_API void texture_bind(texture_t* self, unsigned int sampler)
{
	glActiveTexture(GL_TEXTURE0 + sampler);
	glBindTexture(GL_TEXTURE_2D, self->texture);
}

INSIGHT_API void texture_unbind()
{
	glBindTexture(GL_TEXTURE_2D, 0);
}

INSIGHT_API void texture_terminate(texture_t* self)
{
	glDeleteTextures(1, &self->texture);
	self->channels = 0;
	self->width = 0;
	self->height = 0;

	free(self);
}

INSIGHT_API bool texture_make_screenshot()
{
	time_t t = time(NULL);
	struct tm tm;
	localtime_s(&tm, &t);

	int viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);

	int x = viewport[0];
	int y = viewport[1];
	int width = viewport[2];
	int height = viewport[3];

	unsigned char* data = (unsigned char*)malloc((size_t)(width * height * 3)); // 3 components (R, G, B)

	if (!data)
		return 0;

	glPixelStorei(GL_PACK_ALIGNMENT, 1);
	glReadPixels(x, y, width, height, GL_RGB, GL_UNSIGNED_BYTE, data);

	char* _Buffer = (char*)malloc(sizeof(char) * 512);
	sprintf_s(_Buffer, 512, "%d-%d-%d_%d.%d.%02d.png", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);

	stbi_flip_vertically_on_write(1);
	int saved = stbi_write_png(_Buffer, width, height, 3, data, 0);
	stbi_flip_vertically_on_write(0);

	free(data);
	free(_Buffer);

	return saved;
}

#endif // INSIGHT_TEXTURE_IMPL



