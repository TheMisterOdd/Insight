#pragma once

#include <stdlib.h>
#include <glad/glad.h>

typedef struct
{
	unsigned int Data, SupplyData;
} Model;

#define MODEL_SIZE 1.0f

Model* NewModel3D()
{
	Model* self = (Model*)malloc(sizeof(Model));
	assert(self != NULL);

	printf("Loading new model...\n");
	float vertices[] =
	{
		-MODEL_SIZE, -MODEL_SIZE, -MODEL_SIZE,  0.0f,  0.0f, -1.0f,
		 MODEL_SIZE, -MODEL_SIZE, -MODEL_SIZE,  0.0f,  0.0f, -1.0f,
		 MODEL_SIZE,  MODEL_SIZE, -MODEL_SIZE,  0.0f,  0.0f, -1.0f,
		 MODEL_SIZE,  MODEL_SIZE, -MODEL_SIZE,  0.0f,  0.0f, -1.0f,
		-MODEL_SIZE,  MODEL_SIZE, -MODEL_SIZE,  0.0f,  0.0f, -1.0f,
		-MODEL_SIZE, -MODEL_SIZE, -MODEL_SIZE,  0.0f,  0.0f, -1.0f,

		-MODEL_SIZE, -MODEL_SIZE,  MODEL_SIZE,  0.0f,  0.0f,  1.0f,
		 MODEL_SIZE, -MODEL_SIZE,  MODEL_SIZE,  0.0f,  0.0f,  1.0f,
		 MODEL_SIZE,  MODEL_SIZE,  MODEL_SIZE,  0.0f,  0.0f,  1.0f,
		 MODEL_SIZE,  MODEL_SIZE,  MODEL_SIZE,  0.0f,  0.0f,  1.0f,
		-MODEL_SIZE,  MODEL_SIZE,  MODEL_SIZE,  0.0f,  0.0f,  1.0f,
		-MODEL_SIZE, -MODEL_SIZE,  MODEL_SIZE,  0.0f,  0.0f,  1.0f,

		-MODEL_SIZE,  MODEL_SIZE,  MODEL_SIZE, -1.0f,  0.0f,  0.0f,
		-MODEL_SIZE,  MODEL_SIZE, -MODEL_SIZE, -1.0f,  0.0f,  0.0f,
		-MODEL_SIZE, -MODEL_SIZE, -MODEL_SIZE, -1.0f,  0.0f,  0.0f,
		-MODEL_SIZE, -MODEL_SIZE, -MODEL_SIZE, -1.0f,  0.0f,  0.0f,
		-MODEL_SIZE, -MODEL_SIZE,  MODEL_SIZE, -1.0f,  0.0f,  0.0f,
		-MODEL_SIZE,  MODEL_SIZE,  MODEL_SIZE, -1.0f,  0.0f,  0.0f,

		 MODEL_SIZE,  MODEL_SIZE,  MODEL_SIZE,  1.0f,  0.0f,  0.0f,
		 MODEL_SIZE,  MODEL_SIZE, -MODEL_SIZE,  1.0f,  0.0f,  0.0f,
		 MODEL_SIZE, -MODEL_SIZE, -MODEL_SIZE,  1.0f,  0.0f,  0.0f,
		 MODEL_SIZE, -MODEL_SIZE, -MODEL_SIZE,  1.0f,  0.0f,  0.0f,
		 MODEL_SIZE, -MODEL_SIZE,  MODEL_SIZE,  1.0f,  0.0f,  0.0f,
		 MODEL_SIZE,  MODEL_SIZE,  MODEL_SIZE,  1.0f,  0.0f,  0.0f,

		-MODEL_SIZE, -MODEL_SIZE, -MODEL_SIZE,  0.0f, -1.0f,  0.0f,
		 MODEL_SIZE, -MODEL_SIZE, -MODEL_SIZE,  0.0f, -1.0f,  0.0f,
		 MODEL_SIZE, -MODEL_SIZE,  MODEL_SIZE,  0.0f, -1.0f,  0.0f,
		 MODEL_SIZE, -MODEL_SIZE,  MODEL_SIZE,  0.0f, -1.0f,  0.0f,
		-MODEL_SIZE, -MODEL_SIZE,  MODEL_SIZE,  0.0f, -1.0f,  0.0f,
		-MODEL_SIZE, -MODEL_SIZE, -MODEL_SIZE,  0.0f, -1.0f,  0.0f,

		-MODEL_SIZE,  MODEL_SIZE, -MODEL_SIZE,  0.0f,  1.0f,  0.0f,
		 MODEL_SIZE,  MODEL_SIZE, -MODEL_SIZE,  0.0f,  1.0f,  0.0f,
		 MODEL_SIZE,  MODEL_SIZE,  MODEL_SIZE,  0.0f,  1.0f,  0.0f,
		 MODEL_SIZE,  MODEL_SIZE,  MODEL_SIZE,  0.0f,  1.0f,  0.0f,
		-MODEL_SIZE,  MODEL_SIZE,  MODEL_SIZE,  0.0f,  1.0f,  0.0f,
		-MODEL_SIZE,  MODEL_SIZE, -MODEL_SIZE,  0.0f,  1.0f,  0.0f
	};

	glGenVertexArrays(1, &self->Data);
	glGenBuffers(1, &self->SupplyData);

	glBindVertexArray(self->Data);

	glBindBuffer(GL_ARRAY_BUFFER, self->SupplyData);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
	glEnableVertexAttribArray(0);
	// TexCoord attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0); // Unbind VAO

	return self;
}

Model* NewModel2D()
{
	printf("Loading new model...\n");
	Model* self = (Model*)malloc(sizeof(Model));

	if (!self)
		return NULL;

	float vertices[] =
	{
		// Positions			// Color				// TexCoords
		1.0f,  1.0f, 0.0f,		1.0f, 1.0f, 1.0f,		1.0f,  1.0f,	// 0
		1.0f, -1.0f, 0.0f,		1.0f, 1.0f, 1.0f,		1.0f,  0.0f,	// 1
	   -1.0f, -1.0f, 0.0f,		1.0f, 1.0f, 1.0f,		0.0f,  0.0f,	// 2	
	   -1.0f,  1.0f, 0.0f,		1.0f, 1.0f, 1.0f,		0.0f,  1.0f		// 3

	};
	unsigned int indices[] =
	{
		0, 1, 3,
		1, 2, 3
	};

	glGenBuffers(1, &self->Data);
	glGenBuffers(1, &self->SupplyData);

	glBindBuffer(GL_ARRAY_BUFFER, self->Data);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, self->SupplyData);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)0);

	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)(3 * sizeof(float)));

	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(float) * 8, (void*)(6 * sizeof(float)));

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	return self;
}

void ModelDraw(Model* self, int count, GLenum types)
{
	glBindVertexArray(self->Data);
	glDrawArrays(types, 0, count);
	glBindVertexArray(0);
}

void ModelDrawWithEBOs(Model* self, int count, GLenum type)
{
	glBindBuffer(GL_ARRAY_BUFFER, self->Data);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, self->SupplyData);
	glDrawElements(type, count, GL_UNSIGNED_INT, NULL);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void ModelTerminate(Model* self)
{
	glDeleteBuffers(1, &self->Data);
	glDeleteBuffers(1, &self->SupplyData);
	free(self);
}
