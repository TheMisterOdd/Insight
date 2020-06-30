#ifndef _MODEL_H_
#define _MODEL_H_

#define INSIGHT_BUFFER_IMPL
#include "Buffer.h"

/*! Custom model */
struct model_t
{
	struct vao_t* vao;
	struct vbo_t* vbo;
	struct ibo_t* ibo;
};

/*! Returns a pointer to a model in memory with indices. */
INSIGHT_API struct model_t* model_init(const float* vertices, const GLuint* indices, GLsizeiptr vert_size, GLsizeiptr indices_count);

/*! Returns a pointer to a model in memory without indices. */
INSIGHT_API struct model_t* model_init_without_indices(const float* vertices, GLsizeiptr vert_size);

/*! Starts using the given model, so the user can start managing the model. */
INSIGHT_API void model_begin(struct model_t* self);

/*! Draw the model onto screen.
NOTE: 'model_begin' and 'model_end', should be called before and after this function, respectively. */
INSIGHT_API void model_draw(struct model_t* self);

INSIGHT_API void model_draw_without_indices(struct model_t* self, GLsizeiptr count);

/*! Stops using the current binded model, so the user cannot handle it anymore. */
INSIGHT_API void model_end();

/*! Deletes the memory of the given model. */
INSIGHT_API void model_finalize(struct model_t* self);


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


INSIGHT_API struct model_t* model_init(const float* vertices, const GLuint* indices, GLsizeiptr vert_size, GLsizeiptr indices_count)
{
	struct model_t* self = (struct model_t*)malloc(sizeof(struct model_t));
	assert(self);

	self->vao = vao_init();
	vao_bind(self->vao);

	self->vbo = vbo_init(vertices, vert_size);
	self->ibo = ibo_init(indices, indices_count);

	vao_unbind();
	ibo_unbind();

	return self;
}

INSIGHT_API struct model_t* model_init_without_indices(const float* vertices, GLsizeiptr vert_size)
{
	struct model_t* self = (struct model_t*)malloc(sizeof(struct model_t));
	assert(self);

	self->vao = vao_init();
	vao_bind(self->vao);

	self->vbo = vbo_init(vertices, vert_size);
	self->ibo = NULL;

	vao_unbind();

	return self;
}

INSIGHT_API void model_begin(struct model_t* self)
{
	vao_bind(self->vao);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
}

INSIGHT_API void model_draw(struct model_t* self)
{
	glDrawElements(GL_TRIANGLES, self->ibo->count, GL_UNSIGNED_INT, NULL);
}

INSIGHT_API void model_draw_without_indices(struct model_t* self, GLsizeiptr count)
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

INSIGHT_API void model_finalize(struct model_t* self)
{
	vao_finalize(self->vao);
	vbo_finalize(self->vbo);
	ibo_finalize(self->ibo);

	free(self);
}

#endif /* !INSIGHT_MODEL_IMPL */

#endif /* !_MODEL_H_ */

