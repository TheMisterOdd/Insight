#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <time.h>

#ifndef STB_IMAGE_IMPLEMENTATION

	#define STB_IMAGE_IMPLEMENTATION
	#include <stb_image.h>

#endif 

#ifndef STB_IMAGE_WRITE_IMPLEMENTATION
	#define STB_IMAGE_WRITE_IMPLEMENTATION
	#include <stb_image_writer.h>
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

	if (img == NULL)
	{
		fprintf(stderr, "Error: %s: %s\n", path, strerror(errno)); 
		exit(-1);
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

void TextureBind(Texture* self, unsigned int sampler)
{
	glActiveTexture(GL_TEXTURE0 + sampler);
	glBindTexture(GL_TEXTURE_2D, self->texture);
}

void Screenshot(Window* window) 
{
	printf("\n[Time: %.1f] Screenshot Taken\n", glfwGetTime());

	int n = 3 * window->width * window->height;
	unsigned char* pixels = (unsigned char*)malloc(sizeof(unsigned char) * n);

	glPixelStorei(GL_PACK_ALIGNMENT, 1);

	glReadPixels(0, 0, window->width, window->height, GL_RGB, GL_UNSIGNED_BYTE, pixels);
	if (GL_NO_ERROR != glGetError()) fputs("Error: Unable to read pixels.", stderr);

	stbi_write_png("screenshot.png", window->width, window->height, 3, pixels, 3 * window->width);

	// Free resources
	free(pixels);
}
