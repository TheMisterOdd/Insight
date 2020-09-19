#ifndef _SHADER_H_
#define _SHADER_H_

#include "Insight.h"
#include <glad/glad.h>

#define DEFAULT_VS\
	"#version 330 core\n"\
	"layout(location = 0) in vec3 m_Position;"\
	"layout(location = 1) in vec2 m_TexCoords;"\
	"out vec2 TexCoords;\n"\
	"uniform mat4 MVP;\n"\
	"void main() {\n"\
	"TexCoords = m_TexCoords;\n"\
	"gl_Position = MVP * vec4(m_Position, 1.0);\n"\
	"}\n"\

#define DEFAULT_FS\
	"#version 330 core\n"\
	"in vec2 TexCoords;\n"\
	"uniform sampler2D Texture;\n"\
	"void main() {\n"\
	"gl_FragColor = texture(Texture, TexCoords);\n"\
	"}\n"\


/*! Returns a pointer to a shader object in memory. */
INSIGHT_API unsigned int insight_shader(const char* vsSrc, const char* fsSrc);

/*! Returns a pointer to a shader object in memory. */
INSIGHT_API unsigned int insight_shader_from_path(const char* vsPath, const char* fsPath);

/*! Binds the given shader, so the user can use it. */
INSIGHT_API void insight_shader_bind(GLuint self);

/*! Unbinds the current binded shader, so it cannot be used anymore. */
INSIGHT_API void insight_shader_unbind();

/*! Deletes the memory of the given shader. */
INSIGHT_API void insight_shader_finalize(unsigned int self);


/*
 * ==============================================================
 *
 *                          IMPLEMENTATION
 *
 * ===============================================================
 */
#ifdef INSIGHT_SHADER_IMPL

#include <stdio.h>
#include <stdlib.h>

static unsigned int new_shader(GLenum type, const char* src)
{
	unsigned int shader = glCreateShader(type);
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

	long length;
	FILE* f = NULL;
	fopen_s(&f, path, "rb"); /* was "rb" */
	if (f == NULL) 
	{
		fprintf(stderr, "[Error]: Cannot allocate memory for new shader.\n");
		exit(-1);
	}

	fseek(f, 0, SEEK_END);
	length = ftell(f);
	fseek(f, 0, SEEK_SET);
	char* buffer = (char*)malloc((length + 1) * sizeof(char));
	if (buffer)
	{
		fread(buffer, sizeof(char), length, f);
	}

	fclose(f);

	buffer[length] = '\0';

	return buffer;
}


INSIGHT_API unsigned int insight_shader(const char* vsSrc, const char* fsSrc)
{
	unsigned int shader = glCreateProgram();

	unsigned int vs = new_shader(GL_VERTEX_SHADER, vsSrc);
	unsigned int fs = new_shader(GL_FRAGMENT_SHADER, fsSrc);

	glAttachShader(shader, vs);
	glAttachShader(shader, fs);
	glLinkProgram(shader);
	glValidateProgram(shader);

	glDetachShader(shader, vs);
	glDetachShader(shader, fs);
	glDeleteShader(vs);
	glDeleteShader(fs);

	return shader;
}

INSIGHT_API unsigned int insight_shader_from_path(const char* vsPath, const char* fsPath)
{
	return insight_shader(read_file(vsPath), read_file(fsPath));
}

INSIGHT_API void insight_shader_bind(GLuint self)
{
	glUseProgram(self);
}

INSIGHT_API void insight_shader_unbind()
{
	glUseProgram(0);
}

INSIGHT_API void insight_shader_finalize(unsigned int self)
{
	glDeleteProgram(self);
}

#endif /* INSIGHT_SHADER_IMPL */

#endif /* !_SHADER_H_ */

