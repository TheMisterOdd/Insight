#ifndef _MODEL_H_
#define _MODEL_H_

#include "Buffer.h"

typedef struct 
{
	VAO* vao;
	VBO* vbo;
	IBO* ibo;
} Model;

Model* NewModel();

void ModelBegin(Model* self);

void ModelDraw(Model* self);

void ModelEnd();

#endif // !_MODEL_H_

