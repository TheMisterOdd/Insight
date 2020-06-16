#include "Model.h"

#include <malloc.h>
#include <assert.h>

model_t* model_init()
{
	model_t* self = (model_t*)malloc(sizeof(model_t));
	assert(self);

	float vertices[] =
	{	// Positions:				TexCoords:
		-0.5f, -0.5f, +0.0f,		+0.0f, +0.0f,
		+0.5f, -0.5f, +0.0f,        +1.0f, +0.0f,
		+0.5f, +0.5f, +0.0f,		+1.0f, +1.0f,
		-0.5f, +0.5f, +0.0f,		+0.0f, +1.0f
	};


	unsigned char indices[] =
	{
		0, 1, 2,
		2, 3, 0
	};

	self->vao = vao_init();
	vao_bind(self->vao);

	self->vbo = vbo_init(vertices, sizeof(float) * 32);
	self->ibo = ibo_init(indices, 6);

	vao_unbind();
	ibo_unbind();

	return self;
}

void model_begin(model_t* self)
{
	vao_bind(self->vao);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
}

void model_draw(model_t* self)
{
	glDrawElements(GL_TRIANGLES, self->ibo->count, GL_UNSIGNED_BYTE, NULL);
}

void model_end()
{
	vao_unbind();
}

void model_terminate(model_t* self)
{
	vao_terminate(self->vao);
	vbo_terminate(self->vbo);
	ibo_terminate(self->ibo);

	free(self);
}


