#include "Buffer.h"

#include <malloc.h>
#include <assert.h>

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
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (void*)(sizeof(float) * 3));


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
	glDeleteBuffers(1, &self->id);
}

ibo_t* ibo_init(const void* data, GLsizeiptr count)
{
	ibo_t* self = (ibo_t*)malloc(sizeof(ibo_t));
	assert(self);

	self->count = count;

	glGenBuffers(1, &self->id);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, self->id);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned char) * self->count, data, GL_STATIC_DRAW);

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
	glDeleteBuffers(1, &self->id);
	self->count = 0;
	free(self);
}
