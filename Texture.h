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

typedef struct
{
	int width, height, channels;
	unsigned int texture;

}Texture;

Texture* mk_Texture(const char * path, bool antialias)
{

	Texture* self = (Texture*)malloc(sizeof(Texture));

	glGenTextures(1, &self->texture);
	unsigned char* img = stbi_load(path, &self->width, &self->height, &self->channels, STBI_rgb_alpha);

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

Texture* mk_TextureSkyBox(const char** faces_files, unsigned int lenght)
{
	Texture* self = (Texture*)malloc(sizeof(Texture));

	glGenTextures(1, &self->texture);

	unsigned char *img = 0;

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

void TextureBind(Texture* self, unsigned int sampler)
{
	glActiveTexture(GL_TEXTURE0 + sampler);
	glBindTexture(GL_TEXTURE_2D, self->texture);
}

bool Screenshoot(Window* window)
{
	int n = 3 * window->width * window->height;
	unsigned char* pixels = (unsigned char*)malloc(sizeof(unsigned char) * n);

	glClipControl(GL_UPPER_LEFT, 1);
	glPixelStorei(GL_PACK_ALIGNMENT, 1);

	glReadPixels(0, 0, window->width, window->height, GL_RGB, GL_UNSIGNED_BYTE, pixels);
	if (pixels == NULL)
	{
		fputs("Error: Unable to read pixels.", stderr);
		return false;
	}

	char* outfile = (char*)malloc(sizeof(char) * 128);
	sprintf_s(outfile, 128, "screenshot_%lf.png", glfwGetTime());
	stbi_write_png(outfile, window->width, window->height, 3, pixels, 3 * window->width);
	free(outfile);

	// Free resources
	stbi_image_free(pixels);

	return true; // return true if the screenshot was success
}
