#ifndef _MODEL_H_
#define _MODEL_H_

#include "Buffer.h"

/*! Custom model */
typedef struct 
{
	vao_t* vao;
	vbo_t* vbo;
	ibo_t* ibo;
} model_t;

/*! Returns a pointer to a model in memory. */
model_t* model_init();

/*! Starts using the given model, so the user can start managing the model. */
void model_begin(model_t* self);

/*! Draw the model onto screen.
NOTE: 'model_begin' and 'model_end', should be called before and after this function, respectively. */
void model_draw(model_t* self);

/*! Stops using the current binded model, so the user cannot handle it anymore. */
void model_end();

/*! Deletes the memory of the given model. */
void model_terminate(model_t* self);

#endif // !_MODEL_H_

