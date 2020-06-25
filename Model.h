#ifndef _MODEL_H_
#define _MODEL_H_

#define INSIGHT_BUFFER_IMPL
#include "Buffer.h"

/*! Custom model */
typedef struct 
{
	vao_t* vao;
	vbo_t* vbo;
	ibo_t* ibo;
} model_t;

/*! Returns a pointer to a model in memory with indices. */
INSIGHT_API model_t* model_init(const float* vertices, const GLuint* indices, GLsizeiptr vert_size, GLsizeiptr indices_count);

/*! Returns a pointer to a model in memory without indices. */
INSIGHT_API model_t* model_init_without_indices(const float* vertices, GLsizeiptr vert_size);

/*! Starts using the given model, so the user can start managing the model. */
INSIGHT_API void model_begin(model_t* self);

/*! Draw the model onto screen.
NOTE: 'model_begin' and 'model_end', should be called before and after this function, respectively. */
INSIGHT_API void model_draw(model_t* self);

INSIGHT_API void model_draw_without_indices(model_t* self, GLsizeiptr count);

/*! Stops using the current binded model, so the user cannot handle it anymore. */
INSIGHT_API void model_end();

/*! Deletes the memory of the given model. */
INSIGHT_API void model_terminate(model_t* self);

#endif // !_MODEL_H_

/*
 * ==============================================================
 *
 *                          IMPLEMENTATION
 *
 * ===============================================================
 */
#ifdef INSIGHT_MODEL_IMPL

#include <malloc.h>
#include <assert.h>


INSIGHT_API model_t* model_init(const float* vertices, const GLuint* indices, GLsizeiptr vert_size, GLsizeiptr indices_count)
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

INSIGHT_API model_t* model_init_without_indices(const float* vertices, GLsizeiptr vert_size)
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

INSIGHT_API void model_begin(model_t* self)
{
	vao_bind(self->vao);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
}

INSIGHT_API void model_draw(model_t* self)
{
	glDrawElements(GL_TRIANGLES, self->ibo->count, GL_UNSIGNED_INT, NULL);
}

INSIGHT_API void model_draw_without_indices(model_t* self, GLsizeiptr count)
{
	glDrawArrays(GL_TRIANGLES, 0, count);
}

INSIGHT_API void model_end()
{
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);

	vao_unbind();
}

INSIGHT_API void model_terminate(model_t* self)
{
	vao_terminate(self->vao);
	vbo_terminate(self->vbo);
	ibo_terminate(self->ibo);

	free(self);
}

#endif // INSIGHT_MODEL_IMPL

