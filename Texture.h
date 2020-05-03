#pragma once

#include "Window.h"

#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#endif

#ifndef STB_IMAGE_WRITE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <stb_image_write.h>
#endif

#include <time.h>

typedef struct
{
	int width, height, channels;
	unsigned int texture;
}Texture;

Texture* NewTexture(const char* path, int antialias)
{
	printf("Loading new texture '%s'...\n", path);
	Texture* self = (Texture*)malloc(sizeof(Texture));
	assert(self != NULL);

	glGenTextures(1, &self->texture);

	stbi_set_flip_vertically_on_load(1);
	unsigned char* img = stbi_load(path, &self->width, &self->height, &self->channels, STBI_rgb_alpha);
	stbi_set_flip_vertically_on_load(0);

	if (!img)
	{
		char* err = (char*)malloc(sizeof(char) * 128);
		strerror_s(err, 128, errno);

		fprintf(stderr, "Error: %s: %s\n", path, err);
		free(err);

		return NULL;
	}

	glBindTexture(GL_TEXTURE_2D, self->texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, self->width, self->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, img);
	glGenerateMipmap(GL_TEXTURE_2D);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, antialias ? GL_LINEAR_MIPMAP_LINEAR : GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, antialias ? GL_LINEAR : GL_NEAREST);

	glBindTexture(GL_TEXTURE_2D, 0);
	stbi_image_free(img);

	return self;
}

Texture* NewTextureSkyBox(char** faces_files, unsigned int lenght)
{
	printf("Loading new skybox texture...\n");
	Texture* self = (Texture*)malloc(sizeof(Texture));
	unsigned char *img = 0;

	glGenTextures(1, &self->texture);
	glBindTexture(GL_TEXTURE_CUBE_MAP, self->texture);

	for (unsigned int i = 0; i < lenght; i++)
	{
		img = stbi_load(faces_files[i], &self->width, &self->height, &self->channels, STBI_rgb);
		if (!img)
		{
			char* err = (char*)malloc(sizeof(char) * 128);
			strerror_s(err, 128, errno);

			fprintf(stderr, "Error: %s: %s\n", faces_files[i], err);
			free(err);

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

static int TextureMakeScreenshot()
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

	free(data);
	free(_Buffer);

	return saved;
}

void TextureBind(Texture* self, unsigned int sampler)
{
	glActiveTexture(GL_TEXTURE0 + sampler);
	glBindTexture(GL_TEXTURE_2D, self->texture);
	
}

void TextureTerminate(Texture* self) 
{
	glDeleteTextures(1, &self->texture);
	self->channels = 0;
	self->width = 0;
	self->height = 0;
	
	free(self);
}
