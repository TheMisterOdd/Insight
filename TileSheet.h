#pragma once

#include <cglm/cglm.h>
#include "Texture.h"
#include "Shaders.h"

typedef struct {

	Texture* texture;

	mat4 scale, translation;

	int amount;

} TileSheet;

TileSheet* mk_TileSheet(const char* filename, int amountOfTiles)
{
	TileSheet* self = (TileSheet*)malloc(sizeof(TileSheet));

	self->texture = mk_Texture(filename, GL_FALSE);

	glm_mat4_copy((mat4)GLM_MAT4_IDENTITY_INIT, self->scale);
	glm_mat4_copy((mat4)GLM_MAT4_IDENTITY_INIT, self->translation);

	glm_scale(self->scale, (vec3) { 1.f / amountOfTiles, 1.f / amountOfTiles, 0.f });

	self->amount = amountOfTiles;

	return self;
}

void TileSheetBind(TileSheet* self, Shader shader, int x, int y)
{
	vec3 translation = { (float)x, (float)y, 0 };
	glm_translate_to(self->scale, translation, self->translation);

	glUniformMatrix4fv(glGetUniformLocation(shader, "texModifier"), 1, GL_FALSE, (float*)self->translation);
	glUniform1i(glGetUniformLocation(shader, "Texture"), 0);

	TextureBind(self->texture, 0);
}

void TileSheetRender(TileSheet* self, Shader shader, int tile)
{
	int x = tile % self->amount;
	int y = tile / self->amount;

	TileSheetBind(self, shader, x, y);
}

void TileSheetTerminate(TileSheet* self)
{
	free(self->texture);
	free(self);
}
