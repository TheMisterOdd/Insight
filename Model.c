#include "Model.h"

#include <malloc.h>
#include <assert.h>

Model* NewModel()
{
	Model* self = (Model*)malloc(sizeof(Model));
	assert(self);

	float vertices[] =
	{	// Positions:				Color:
		-0.5f, -0.5f, +0.0f,		+1.0f, +1.0f, +1.0f,		+0.0f, +0.0f,
		+0.5f, -0.5f, +0.0f,		+1.0f, +1.0f, +1.0f,		+1.0f, +0.0f,
		+0.5f, +0.5f, +0.0f,		+1.0f, +1.0f, +1.0f,		+1.0f, +1.0f,
		-0.5f, +0.5f, +0.0f,		+1.0f, +1.0f, +1.0f,		+0.0f, +1.0f
	};


	unsigned char indices[] =
	{
		0, 1, 2,
		2, 3, 0
	};

	self->vao = NewVAO();
	VAOBind(self->vao);

	self->vbo = NewVBO(vertices, sizeof(float) * 32);
	self->ibo = NewIBO(indices, 6);

	VAOUnbind();

	return self;
}

void ModelBegin(Model* self)
{
	VAOBind(self->vao);
	IBOBind(self->ibo);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
}

void ModelDraw(Model* self)
{
	glDrawElements(GL_TRIANGLES, self->ibo->count, GL_UNSIGNED_BYTE, NULL);
}

void ModelEnd()
{
	IBOUnbind();
	VAOUnbind();
}
