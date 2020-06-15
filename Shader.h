#ifndef _SHADER_H_
#define _SHADER_H_

#include <glad/glad.h>

typedef struct 
{
	GLuint id;
} Shader;

Shader* NewShader(const char* vsPath, const char* fsPath);

void ShaderTerminate(Shader* self);

#endif

