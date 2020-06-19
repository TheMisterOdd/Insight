#ifndef _BUFFER_H_
#define _BUFFER_H_

#include "Texture.h"
#include <glad/glad.h>

/*! Vertex array object */
typedef struct
{
	GLuint id;
} vao_t;

/*! Vertex buffer object */
typedef struct
{
	GLuint id;
} vbo_t;

/*! Index buffer object */
typedef struct
{
	GLuint id;
	GLsizeiptr count;
} ibo_t;

/*! Framebuffer object */
typedef struct 
{
	texture_t* texture;
	GLuint fbo, rbo;

} fbo_t;

/*! Returns a pointer to a vertex array object in memory */
INSIGHT_API vao_t* vao_init();

/*! Binds the given vertex array object, in order to use it */
INSIGHT_API void vao_bind(vao_t* self);

/*! Unbinds the current binded vertex array object */
INSIGHT_API void vao_unbind();

/*! Deletes the memory of the given vertex array object */
INSIGHT_API void vao_terminate(vao_t* self);

/*! Returns a pointer to a vertex buffer object in memory
NOTE: The given array should have a three component value for position per vertex 
and a two component value for texture cordinates per vertex.
In case that you don't want this layout, you must set up your own model and buffer class.*/
INSIGHT_API vbo_t* vbo_init(const void* data, GLsizeiptr size);

/*! Binds the given vertex buffer object, in order to use it */
INSIGHT_API void vbo_bind(vbo_t* self);

/*! Unbinds the current binded vertex buffer object */
INSIGHT_API void vbo_unbind();

/*! Deletes the memory of the given vertex buffer object */
INSIGHT_API void vbo_terminate(vbo_t* self);


/*! Returns a pointer to a index buffer object in memory */
INSIGHT_API ibo_t* ibo_init(const void* data, GLsizeiptr count);

/*! Binds the given index buffer object, in order to use it */
INSIGHT_API void ibo_bind(ibo_t* self);

/*! Unbinds the current binded index buffer object */
INSIGHT_API void ibo_unbind();

/*! Deletes the memory of the given index buffer object */
INSIGHT_API void ibo_terminate(ibo_t* self);


/*! Returns a pointer to a framebuffer object in memory */
INSIGHT_API fbo_t* fbo_init(int width, int height);

/*! Binds the buffer for handeling it. */
INSIGHT_API void fbo_bind(fbo_t* self);

/*! Unbinds the current binded buffer. */
INSIGHT_API void fbo_unbind(int width, int height);

/*! Deletes the memory of the given framebuffer object */
INSIGHT_API void fbo_terminate(fbo_t* self);

#endif // !_BUFFER_H_


