#ifndef _BUFFER_H_
#define _BUFFER_H_

#include <glad/glad.h>

typedef struct
{
	GLuint id;
} VAO;

typedef struct
{
	GLuint id;
} VBO;

typedef struct
{
	GLuint id;
	GLsizeiptr count;
} IBO;

/* VAO */
VAO* NewVAO();

void VAOBind(VAO* self);

void VAOUnbind();

void VAOTerminate(VAO* self);

/* VBO */
VBO* NewVBO(const void* data, GLsizeiptr size);

void VBOBind(VBO* self);

void VBOUnbind();

void VBOTerminate(VBO* self);

/* IBO */
IBO* NewIBO(const void* data, GLsizeiptr count);

void IBOBind(IBO* self);

void IBOUnbind();

void IBOTerminate(IBO* self);


#endif // !_BUFFER_H_


