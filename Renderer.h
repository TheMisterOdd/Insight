#pragma once

#include "Model.h"
#include "Tile.h"
#include "Texture.h"
#include "Shaders.h"
#include "Camera.h"

#include <cglm/cglm.h>

typedef struct {

	Model model;
	Shader shader;
	Camera* camera;

} Renderer;

Tile* null_tile = NULL, *bg = NULL;

Renderer* mk_Renderer(Shader shader, Camera* camera) {

	Renderer* self = (Renderer*)malloc(sizeof(Renderer));

	null_tile = mk_Tile("resources/textures/test.png");
	bg = mk_Tile("resources/textures/Sand.png");

	self->model = mk_Model();
	self->shader = shader;
	self->camera = camera;

	return self;
}

void RendererRenderTile(Renderer* self, Tile* tile, int x, int y, mat4 world) {

	ShaderBind(self->shader);
	if (tile != NULL)
		TextureBind(tile->texture, 0);

	mat4 tile_pos, projection;
	CameraGetTranslatedProjection(self->camera, projection, 0);

	glm_translate_make(tile_pos, (vec3) { x * 2.f, y * 2.f, 0 });

	glm_mat4_mul(projection, world, projection);
	glm_mat4_mul(projection, tile_pos, projection);

	glUniformMatrix4fv(glGetUniformLocation(self->shader, "target"), 1, GL_FALSE, (float*)projection);
	glUniform1i(glGetUniformLocation(self->shader, "Texture"), 0);

	ModelDraw(self->model);

	glBindTexture(GL_TEXTURE_2D, 0);
	glUseProgram(0);
}

void RendererTerminate(Renderer* self)
{
	free(self->camera);
	free(self);
}
