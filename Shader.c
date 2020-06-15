#include "Shader.h"

#include <assert.h>

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
		fprintf(stderr, "Error: cannot load given file: '%s'\n", path);
		return NULL;
	}
	buffer[length] = '\0';

	return buffer;
}


Shader* NewShader(const char* vsPath, const char* fsPath)
{

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

	return &(Shader){self};
}

void ShaderTerminate(Shader* self)
{
	glDeleteShader(self->id);
	free(self);
}
