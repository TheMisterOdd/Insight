#ifndef _MODEL_H_
#define _MODEL_H_

#include <stdint.h>
#include "Insight.h"

/*
	Custom model declaration.
	Is an array of 4 elements:
	-	1st is the vbo id
	-	2nd is the vao id
	-	3nd is the ibo id
	-	4th is the indices count
*/
typedef uint32_t Insight_Model[4];

/*! Returns a pointer to a model in memory with indices. */
INSIGHT_API void insight_model(Insight_Model self, const float* const vertices, const uint32_t* const indices, uintptr_t vert_size, uintptr_t indices_count);

/*! Starts using the given model, so the user can start managing the model. */
INSIGHT_API void insight_model_begin(Insight_Model self);

/*! Draw the model onto screen.
NOTE: 'model_begin' and 'model_end', should be called before and after this function, respectively. */
INSIGHT_API void insight_model_draw(Insight_Model self, GLenum type);

/*! Stops using the current binded model, so the user cannot handle it anymore. */
INSIGHT_API void insight_model_end();

/*! Deletes the memory of the given model. */
INSIGHT_API void insight_model_finalize(Insight_Model self);


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

INSIGHT_API void insight_model(Insight_Model self, const float* const vertices, const uint32_t* const indices, uintptr_t vert_size, uintptr_t indices_count)
{
	/* allocate the vao on the first element of the array */
	glGenVertexArrays(1, &self[0]);
	glBindVertexArray(self[0]);

	glGenBuffers(1, &self[1]);
	glBindBuffer(GL_ARRAY_BUFFER, self[1]);
	glBufferData(GL_ARRAY_BUFFER, vert_size, vertices, GL_STATIC_DRAW);

	glGenBuffers(1, &self[2]);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, self[2]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint32_t) * indices_count, indices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (void*)0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(float) * 5, (void*)(sizeof(float) * 3));

	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	self[3] = (uint32_t)indices_count;
}

INSIGHT_API void insight_model_begin(Insight_Model self)
{
	glBindVertexArray(self[0]);

	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
}

INSIGHT_API void insight_model_draw(Insight_Model self, GLenum type)
{
	glDrawElements(type, self[3], GL_UNSIGNED_INT, NULL);
}

INSIGHT_API void insight_model_end()
{
	glDisableVertexAttribArray(0);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(2);

	glBindVertexArray(0);
}

INSIGHT_API void insight_model_finalize(Insight_Model self)
{
	glDeleteVertexArrays(1, &self[0]);
	glDeleteBuffers(1, &self[1]);
	glDeleteBuffers(1, &self[2]);
}

#endif /* !INSIGHT_MODEL_IMPL */

#endif /* !_MODEL_H_ */

