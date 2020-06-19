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
vao_t* vao_init();

/*! Binds the given vertex array object, in order to use it */
void vao_bind(vao_t* self);

/*! Unbinds the current binded vertex array object */
void vao_unbind();

/*! Deletes the memory of the given vertex array object */
void vao_terminate(vao_t* self);

/*! Returns a pointer to a vertex buffer object in memory
NOTE: The given array should have a three component value for position per vertex 
and a two component value for texture cordinates per vertex.
In case that you don't want this layout, you must set up your own model and buffer class.*/
vbo_t* vbo_init(const void* data, GLsizeiptr size);

/*! Binds the given vertex buffer object, in order to use it */
void vbo_bind(vbo_t* self);

/*! Unbinds the current binded vertex buffer object */
void vbo_unbind();

/*! Deletes the memory of the given vertex buffer object */
void vbo_terminate(vbo_t* self);


/*! Returns a pointer to a index buffer object in memory */
ibo_t* ibo_init(const void* data, GLsizeiptr count);

/*! Binds the given index buffer object, in order to use it */
void ibo_bind(ibo_t* self);

/*! Unbinds the current binded index buffer object */
void ibo_unbind();

/*! Deletes the memory of the given index buffer object */
void ibo_terminate(ibo_t* self);


/*! Returns a pointer to a framebuffer object in memory */
fbo_t* fbo_init(int width, int height);

/*! Binds the buffer and use the given function for drawing inside the buffer */
void fbo_set_into(fbo_t* self, void (*draw_into_func)(void));

/*! Deletes the memory of the given framebuffer object */
void fbo_terminate(fbo_t* self);

#endif // !_BUFFER_H_


