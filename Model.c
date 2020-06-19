#include "Model.h"

#include <malloc.h>
#include <assert.h>


model_t* model_init(const float* vertices, const GLuint* indices, GLsizeiptr vert_size, GLsizeiptr indices_count)
{
	model_t* self = (model_t*)malloc(sizeof(model_t));
	assert(self);

	self->vao = vao_init();
	vao_bind(self->vao);

	self->vbo = vbo_init(vertices, vert_size);
	self->ibo = ibo_init(indices, indices_count);

	vao_unbind();
	ibo_unbind();

	return self;
}

model_t* model_init_without_indices(const float* vertices, GLsizeiptr vert_size)
{
	model_t* self = (model_t*)malloc(sizeof(model_t));
	assert(self);

	self->vao = vao_init();
	vao_bind(self->vao);

	self->vbo = vbo_init(vertices, vert_size);
	self->ibo = NULL;

	vao_unbind();

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
	glDrawElements(GL_TRIANGLES, self->ibo->count, GL_UNSIGNED_INT, NULL);
}

void model_draw_without_indices(model_t* self, GLsizeiptr count)
{
	glDrawArrays(GL_TRIANGLES, 0, count);
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


