#include "Buffer.h"

#include <malloc.h>
#include <assert.h>
#include <stdio.h>

vao_t* vao_init()
{
	vao_t* self = (vao_t*)malloc(sizeof(vao_t));
	assert(self);

	glGenVertexArrays(1, &self->id);

	return self;
}

void vao_bind(vao_t* self)
{
	glBindVertexArray(self->id);
}

void vao_unbind()
{
	glBindVertexArray(0);
}

void vao_terminate(vao_t* self)
{

	glDeleteVertexArrays(1, &self->id);
	free(self);
}

vbo_t* vbo_init(const void* data, GLsizeiptr size)
{
	vbo_t* self = (vbo_t*)malloc(sizeof(vbo_t));
	assert(self);

	glGenBuffers(1, &self->id);
	glBindBuffer(GL_ARRAY_BUFFER, self->id);
	glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (void*)0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (void*)(sizeof(float) * 3));


	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	vbo_unbind();

	return self;
}

void vbo_bind(vbo_t* self)
{
	glBindBuffer(GL_ARRAY_BUFFER, self->id);
}

void vbo_unbind()
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void vbo_terminate(vbo_t* self)
{
	if (self == NULL)
		return;

	glDeleteBuffers(1, &self->id);
}

ibo_t* ibo_init(const void* data, GLsizeiptr count)
{
	ibo_t* self = (ibo_t*)malloc(sizeof(ibo_t));
	assert(self);

	self->count = count;

	glGenBuffers(1, &self->id);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, self->id);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * self->count, data, GL_STATIC_DRAW);

	return self;
}

void ibo_bind(ibo_t* self)
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, self->id);
}

void ibo_unbind()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void ibo_terminate(ibo_t* self)
{
	if (self == NULL)
		return;

	glDeleteBuffers(1, &self->id);
	self->count = 0;
	free(self);
}

fbo_t* fbo_init(int width, int height)
{
	fbo_t* self = (fbo_t*)malloc(sizeof(fbo_t));
	assert(self);

	self->texture = (texture_t*)malloc(sizeof(texture_t));
	assert(self->texture);

	self->texture->width = width;
	self->texture->height = height;

	glGenTextures(1, &self->texture->texture);
	glBindTexture(GL_TEXTURE_2D, self->texture->texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glBindTexture(GL_TEXTURE_2D, 0);

	glGenFramebuffers(1, &self->fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, self->fbo);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, self->texture->texture, 0);

	glGenRenderbuffers(1, &self->rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, self->rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH24_STENCIL8, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, self->rbo);

	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		fprintf(stderr, "ERROR::FRAMEBUFFER:: Framebuffer is not complete!");
		return NULL;
	}

	return self;
}

void fbo_set_into(fbo_t* self, void(*draw_into_func)(void))
{
	glBindFramebuffer(GL_FRAMEBUFFER, self->fbo);
	draw_into_func();
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void fbo_terminate(fbo_t* self)
{
	texture_terminate(self->texture);
	glDeleteRenderbuffers(1, &self->rbo);
	glDeleteFramebuffers(1, &self->fbo);
	free(self);
}
