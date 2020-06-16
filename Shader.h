#ifndef _SHADER_H_
#define _SHADER_H_

#include <glad/glad.h>

typedef struct 
{
	GLuint id;
} shader_t;

/*! Returns a pointer to a shader object in memory. */
shader_t* shader_init(const char* vsPath, const char* fsPath);

/*! Binds the given shader, so the user can use it. */
void shader_bind(shader_t* self);

/*! Unbinds the current binded shader, so it cannot be used anymore. */
void shader_unbind();

/*! Deletes the memory of the given shader. */
void shader_terminate(shader_t* self);

#endif

