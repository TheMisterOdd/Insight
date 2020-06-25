#ifndef _SHADER_H_
#define _SHADER_H_

#include "Core.h"
#include <glad/glad.h>

typedef struct 
{
	GLuint id;
} shader_t;

/*! Returns a pointer to a shader object in memory. */
INSIGHT_API shader_t* shader_init(const char* vsPath, const char* fsPath);

/*! Binds the given shader, so the user can use it. */
INSIGHT_API void shader_bind(shader_t* self);

/*! Unbinds the current binded shader, so it cannot be used anymore. */
INSIGHT_API void shader_unbind();

/*! Deletes the memory of the given shader. */
INSIGHT_API void shader_terminate(shader_t* self);

#endif /* !_SHADER_H_ */

/*
 * ==============================================================
 *
 *                          IMPLEMENTATION
 *
 * ===============================================================
 */
#ifdef INSIGHT_SHADER_IMPL
GLuint new_shader(GLenum type, const char* src)
{
	GLuint shader = glCreateShader(type);
	glShaderSource(shader, 1, &src, NULL);
	glCompileShader(shader);

	int status = 0;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &status);

	if (!status)
	{
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &status);

		char* err = (char*)malloc(sizeof(char) * status);
		glGetShaderInfoLog(shader, status, &status, err);

		printf((type == GL_VERTEX_SHADER) ? "\nVERTEX_SHADER_ERROR: \n%s\n" : "\nFRAGMENT_SHADER_ERROR: \n%s\n", err);
	}

	return shader;
}

char* read_file(const char* path)
{

	char* buffer = NULL;
	long length;
	FILE* f = NULL;
	fopen_s(&f, path, "rb"); /* was "rb" */
	assert(f != NULL);

	if (f)
	{
		fseek(f, 0, SEEK_END);
		length = ftell(f);
		fseek(f, 0, SEEK_SET);
		buffer = (char*)malloc((length + 1) * sizeof(char));
		if (buffer)
		{
			fread(buffer, sizeof(char), length, f);
		}

		fclose(f);
	}
	else
	{
		fprintf(stderr, "Error: cannot load given file: '%s'\n", path);
		return NULL;
	}
	buffer[length] = '\0';

	return buffer;
}


INSIGHT_API shader_t* shader_init(const char* vsPath, const char* fsPath)
{
	shader_t* self = (shader_t*)malloc(sizeof(shader_t));
	assert(self);

	self->id = glCreateProgram();

	GLuint vs = new_shader(GL_VERTEX_SHADER, read_file(vsPath));
	GLuint fs = new_shader(GL_FRAGMENT_SHADER, read_file(fsPath));

	glAttachShader(self->id, vs);
	glAttachShader(self->id, fs);
	glLinkProgram(self->id);
	glValidateProgram(self->id);

	glDetachShader(self->id, vs);
	glDetachShader(self->id, fs);
	glDeleteShader(vs);
	glDeleteShader(fs);

	return self;
}

INSIGHT_API void shader_bind(shader_t* self)
{
	glUseProgram(self->id);
}

INSIGHT_API void shader_unbind()
{
	glUseProgram(0);
}

INSIGHT_API void shader_terminate(shader_t* self)
{
	glDeleteProgram(self->id);
	free(self);
}

#endif /* INSIGHT_SHADER_IMPL */


