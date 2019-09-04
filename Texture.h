#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#ifndef STB_IMAGE_IMPLEMENTATION

	#define STB_IMAGE_IMPLEMENTATION
	#include <stb_image.h>

#endif 

typedef struct  
{
	int width, height;
	unsigned int texture;

}Texture;

typedef unsigned char* Image;

Texture* mk_Texture(const char * path, bool antialias)
{
	
	Texture* self = (Texture*)malloc(sizeof(Texture));

	glGenTextures(1, &self->texture);
	Image img = stbi_load(path, &self->width, &self->height, NULL, STBI_rgb_alpha);

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
