#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <glad/glad.h>

#define ShaderBind(x) glUseProgram(x);

GLuint new_shader(GLenum type, const char* src)
{
	GLuint shader = glCreateShader(type);
	glShaderSource(shader, 1, &src, NULL);
	glCompileShader(shader);

	//Error Handeling
	int errState = 0;

	glGetShaderiv(shader, GL_COMPILE_STATUS, &errState);
	if (!errState)
	{
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &errState);
		char* err = (char*)malloc(sizeof(char) * errState);
		glGetShaderInfoLog(shader, errState, &errState, err);

		printf((type == GL_VERTEX_SHADER) ? "\nVERTEX_SHADER_ERROR: %s\n" : "\nFRAGMENT_SHADER_ERROR: %s\n", err);
	}

	free(src);
	return shader;
}

static char* read_file(const char* path)
{

	char* buffer = NULL;
	long length;
	FILE* f = NULL;
	fopen_s(&f, path, "rb"); //was "rb"
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
		char* err = (char*)malloc(sizeof(char) * 128);
		strerror_s(err, 128, errno);

		printf("Error: %s: %s\n", path, err);
		free(err);

		return NULL;
	}
	buffer[length] = '\0';

	return buffer;
}

GLuint NewShader(const char* vsPath, const char* fsPath)
{
	printf("Loading new shader...\n");
	GLuint self = glCreateProgram();
	GLuint vs = new_shader(GL_VERTEX_SHADER, read_file(vsPath));
	GLuint fs = new_shader(GL_FRAGMENT_SHADER, read_file(fsPath));

	glAttachShader(self, vs);
	glAttachShader(self, fs);

	glLinkProgram(self);
	glValidateProgram(self);

	glDetachShader(self, vs);
	glDetachShader(self, fs);
	glDeleteShader(vs);
	glDeleteShader(fs);
	

	return self;
}

void ShaderTerminate(GLuint shader)
{
	glDeleteProgram(shader);
}

