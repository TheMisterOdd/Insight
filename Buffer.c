#include "Buffer.h"

#include <malloc.h>
#include <assert.h>

VAO* NewVAO()
{
	VAO* self = (VAO*)malloc(sizeof(VAO));
	assert(self);

	glGenVertexArrays(1, &self->id);

	return self;
}

void VAOBind(VAO* self)
{
	glBindVertexArray(self->id);
}

void VAOUnbind()
{
	glBindVertexArray(0);
}

void VAOTerminate(VAO* self)
{
	glDeleteVertexArrays(1, &self->id);
	free(self);
}

VBO* NewVBO(const void* data, GLsizeiptr size)
{
	VBO* self = (VBO*)malloc(sizeof(VBO));
	assert(self);

	glGenBuffers(1, &self->id);
	glBindBuffer(GL_ARRAY_BUFFER, self->id);
	glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)(sizeof(float) * 3));

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)(sizeof(float) * 6));

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);
	VBOUnbind();

	return self;
}

void VBOBind(VBO* self)
{
	glBindBuffer(GL_ARRAY_BUFFER, self->id);
}

void VBOUnbind()
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void VBOTerminate(VBO* self)
{
	glDeleteBuffers(1, &self->id);
}

IBO* NewIBO(const void* data, GLsizeiptr count)
{
	IBO* self = (IBO*)malloc(sizeof(IBO));
	assert(self);

	self->count = count;

	glGenBuffers(1, &self->id);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, self->id);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned char) * self->count, data, GL_STATIC_DRAW);

	IBOUnbind();
	return self;
}

void IBOBind(IBO* self)
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, self->id);
}

void IBOUnbind()
{
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void IBOTerminate(IBO* self)
{
	glDeleteBuffers(1, &self->id);
	self->count = 0;
	free(self);
}
