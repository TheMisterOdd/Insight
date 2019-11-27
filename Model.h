#pragma once

#include <stdlib.h>
#include <glad/glad.h>

typedef unsigned int Model;

#define MODEL_SIZE 1.f

Model NewModel() 
{
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

	unsigned int VBO, VAO;
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

	// Position attribute
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)0);
	glEnableVertexAttribArray(0);
	// TexCoord attribute
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void *)(3 * sizeof(float)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0); // Unbind VAO

	return VAO;
}

void ModelDraw(Model self)
{	
	glBindVertexArray(self);
	glDrawArrays(GL_TRIANGLES, 0, 36);
	glBindVertexArray(0);
}

Model NewModelSkyBox()
{
	float vertices[] =
	{
		// Positions
		-1.f,  1.f, -1.f,
		-1.f, -1.f, -1.f,
		 1.f, -1.f, -1.f,
		 1.f, -1.f, -1.f,
		 1.f,  1.f, -1.f,
		-1.f,  1.f, -1.f,
		    	 	 
		-1.f, -1.f,  1.f,
		-1.f, -1.f, -1.f,
		-1.f,  1.f, -1.f,
		-1.f,  1.f, -1.f,
		-1.f,  1.f,  1.f,
		-1.f, -1.f,  1.f,
		    	 	 
		 1.f, -1.f, -1.f,
		 1.f, -1.f,  1.f,
		 1.f,  1.f,  1.f,
		 1.f,  1.f,  1.f,
		 1.f,  1.f, -1.f,
		 1.f, -1.f, -1.f,
		    	 	 
		-1.f, -1.f,  1.f,
		-1.f,  1.f,  1.f,
		 1.f,  1.f,  1.f,
		 1.f,  1.f,  1.f,
		 1.f, -1.f,  1.f,
		-1.f, -1.f,  1.f,
		    	 	 
		-1.f,  1.f, -1.f,
		 1.f,  1.f, -1.f,
		 1.f,  1.f,  1.f,
		 1.f,  1.f,  1.f,
		-1.f,  1.f,  1.f,
		-1.f,  1.f, -1.f,
		    	 	 
		-1.f, -1.f, -1.f,
		-1.f, -1.f,  1.f,
		 1.f, -1.f, -1.f,
		 1.f, -1.f, -1.f,
		-1.f, -1.f,  1.f,
		 1.f, -1.f,  1.f
	};

	unsigned int VAO, VBO;

	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	glBindVertexArray(VAO);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), &vertices, GL_STATIC_DRAW);

	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid *)0);
	glBindVertexArray(0);
	
	return VAO;
}