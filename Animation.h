#pragma once

#include "Texture.h"
#include "TileSheet.h"
#include <stdio.h>
#include <malloc.h>

typedef struct {

	Texture** frames;
	int pointer, frames_lenght;
	double elapsedTime, currentTime, lastTime, FPS;

} Animation;

Animation* mk_Animation(const char* animation_path, int amount, int fps) {

	Animation* self = (Animation*)malloc(sizeof(Animation));

	self->frames = (Texture**)malloc(sizeof(Texture*) * amount);

	self->pointer = 0;
	self->frames_lenght = amount;

	self->elapsedTime = 0;
	self->currentTime = 0;
	self->lastTime = glfwGetTime();
	self->FPS = 1 / (double)fps;

	for (int i = 0; i < amount; i++)
	{
		char* name = (char*)malloc(sizeof(char) * strlen(animation_path) + 8);
		sprintf_s(name, strlen(animation_path) + 8, "%s%d.png", animation_path, i);
		self->frames[i] = mk_Texture(name, GL_FALSE);
		free(name);
	}

	return self;
}

void AnimationBind(Animation* self, int sampler)
{
	self->currentTime = glfwGetTime();
	self->elapsedTime += self->currentTime - self->lastTime;

	if (self->elapsedTime >= self->FPS)
	{
		self->elapsedTime = 0;
		self->pointer++;
	}

	if (self->pointer >= self->frames_lenght)
	{
		self->pointer = 0;
	}
	self->lastTime = self->currentTime;

	TextureBind(self->frames[self->pointer], 0);
}


