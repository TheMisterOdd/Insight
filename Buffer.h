#ifndef _BUFFER_H_
#define _BUFFER_H_

#include "Texture.h"
#include <glad/glad.h>


/*! Vertex array object */
struct vao_t
{
	GLuint id;
};

/*! Vertex buffer object */
struct vbo_t
{
	GLuint id;
};

/*! Index buffer object */
struct ibo_t
{
	GLuint id;
	GLsizeiptr count;
};

/*! Framebuffer object */
struct fbo_t
{
	struct texture_t* texture;
	GLuint fbo, rbo;

};

/*! Returns a pointer to a vertex array object in memory */
INSIGHT_API struct vao_t* vao_init();

/*! Binds the given vertex array object, in order to use it */
INSIGHT_API void vao_bind(struct vao_t* self);

/*! Unbinds the current binded vertex array object */
INSIGHT_API void vao_unbind();

/*! Deletes the memory of the given vertex array object */
INSIGHT_API void vao_finalize(struct vao_t* self);

/*! Returns a pointer to a vertex buffer object in memory
NOTE: The given array should have a three component value for position per vertex 
and a two component value for texture cordinates per vertex.
In case that you don't want this layout, you must set up your own model and buffer class.*/
INSIGHT_API struct vbo_t* vbo_init(const void* data, GLsizeiptr size);

/*! Binds the given vertex buffer object, in order to use it */
INSIGHT_API void vbo_bind(struct vbo_t* self);

/*! Unbinds the current binded vertex buffer object */
INSIGHT_API void vbo_unbind();

/*! Deletes the memory of the given vertex buffer object */
INSIGHT_API void vbo_finalize(struct vbo_t* self);


/*! Returns a pointer to a index buffer object in memory */
INSIGHT_API struct ibo_t* ibo_init(const void* data, GLsizeiptr count);

/*! Binds the given index buffer object, in order to use it */
INSIGHT_API void ibo_bind(struct ibo_t* self);

/*! Unbinds the current binded index buffer object */
INSIGHT_API void ibo_unbind();

/*! Deletes the memory of the given index buffer object */
INSIGHT_API void ibo_finalize(struct ibo_t* self);


/*! Returns a pointer to a framebuffer object in memory */
INSIGHT_API struct fbo_t* fbo_init(int width, int height);

/*! Binds the buffer for handeling it. */
INSIGHT_API void fbo_bind(struct fbo_t* self);

/*! Unbinds the current binded buffer. */
INSIGHT_API void fbo_unbind(int width, int height);

/*! Deletes the memory of the given framebuffer object */
INSIGHT_API void fbo_finalize(struct fbo_t* self);


/*
 * ==============================================================
 *
 *                          IMPLEMENTATION
 *
 * ===============================================================
 */
#ifdef INSIGHT_BUFFER_IMPL

#include <malloc.h>
#include <assert.h>
#include <stdio.h>

INSIGHT_API struct vao_t* vao_init()
{
	struct vao_t* self = (struct vao_t*)malloc(sizeof(struct vao_t));
	assert(self);

	glGenVertexArrays(1, &self->id);

	return self;
}

INSIGHT_API void vao_bind(struct vao_t* self)
{
	glBindVertexArray(self->id);
}

INSIGHT_API void vao_unbind()
{
	glBindVertexArray(0);
}

INSIGHT_API void vao_finalize(struct vao_t* self)
{
	glDeleteVertexArrays(1, &self->id);
	free(self);
}

INSIGHT_API struct vbo_t* vbo_init(const void* data, GLsizeiptr size)
{
	struct vbo_t* self = (struct vbo_t*)malloc(sizeof(struct vbo_t));
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

INSIGHT_API void vbo_bind(struct vbo_t* self)
{
	glBindBuffer(GL_ARRAY_BUFFER, self->id);
}

INSIGHT_API void vbo_unbind()
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

INSIGHT_API void vbo_finalize(struct vbo_t* self)
{
	if (self == NULL)
		return;

	glDeleteBuffers(1, &self->id);
}

INSIGHT_API struct ibo_t* ibo_init(const void* data, GLsizeiptr count)
{
	struct ibo_t* self = (struct ibo_t*)malloc(sizeof(struct ibo_t));
	assert(self);

	self->count = count;

	glGenBuffers(1, &self->id);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, self->id);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * self->count, data, GL_STATIC_DRAW);

	return self;
}

INSIGHT_API void ibo_bind(struct ibo_t* self)
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, self->id);
}

INSIGHT_API void ibo_unbind()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

INSIGHT_API void ibo_finalize(struct ibo_t* self)
{
	if (self == NULL)
		return;

	glDeleteBuffers(1, &self->id);
	self->count = 0;
	free(self);
}

INSIGHT_API struct fbo_t* fbo_init(int width, int height)
{
	struct fbo_t* self = (struct fbo_t*)malloc(sizeof(struct fbo_t));
	assert(self);

	self->texture = (struct texture_t*)malloc(sizeof(struct texture_t));
	assert(self->texture);

	self->texture->width = width;
	self->texture->height = height;

	glGenTextures(1, &self->texture->texture);
	glBindTexture(GL_TEXTURE_2D, self->texture->texture);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glBindTexture(GL_TEXTURE_2D, 0);

	glGenFramebuffers(1, &self->fbo);
	glBindFramebuffer(GL_FRAMEBUFFER, self->fbo);
	glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, self->texture->texture, 0);

	glGenRenderbuffers(1, &self->rbo);
	glBindRenderbuffer(GL_RENDERBUFFER, self->rbo);
	glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT, width, height);
	glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, self->rbo);

	if (glCheckFramebufferStatus(GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE)
	{
		fprintf(stderr, "ERROR::FRAMEBUFFER:: Framebuffer is not complete!");
		return NULL;
	}

	glBindRenderbuffer(GL_RENDERBUFFER, 0);
	glBindFramebuffer(GL_FRAMEBUFFER, 0);


	return self;
}

INSIGHT_API void fbo_bind(struct fbo_t* self)
{
	glBindFramebuffer(GL_FRAMEBUFFER, self->fbo);
	glViewport(0, 0, self->texture->width, self->texture->height);
}

INSIGHT_API void fbo_unbind(int width, int height)
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
	glViewport(0, 0, width, height);
}

INSIGHT_API void fbo_finalize(struct fbo_t* self)
{
	texture_finalize(self->texture);
	glDeleteRenderbuffers(1, &self->rbo);
	glDeleteFramebuffers(1, &self->fbo);
	free(self);
}

#endif /* INSIGHT_BUFFER_IMPL */

#endif /* !_BUFFER_H_ */


