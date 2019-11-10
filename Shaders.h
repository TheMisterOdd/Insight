#pragma once

#include <stdio.h>
#include <stdlib.h>
#include <glad/glad.h>

typedef unsigned int Shader;
#define ShaderBind(x) glUseProgram(x);

static unsigned int new_shader(unsigned int type, char* src)
{
	unsigned int shader = glCreateShader(type);
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

		fprintf(stderr, (type == GL_VERTEX_SHADER) ? "\nVERTEX_SHADER_ERROR: %s\n" : "\nFRAGMENT_SHADER_ERROR: %s\n", err);
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

		fprintf(stderr, "Error: %s: %s\n", path, err);
		free(err);

		return NULL;
	}
	buffer[length] = '\0';

	return buffer;
}

Shader mk_Shader(const char* vsPath, const char* fsPath)
{
	Shader self;
	self = glCreateProgram();
	unsigned int vs = new_shader(GL_VERTEX_SHADER, read_file(vsPath));
	unsigned int fs = new_shader(GL_FRAGMENT_SHADER, read_file(fsPath));

	glAttachShader(self, vs);
	glAttachShader(self, fs);

	glLinkProgram(self);
	glValidateProgram(self);

	glDeleteShader(vs);
	glDeleteShader(fs);

	return self;
}


