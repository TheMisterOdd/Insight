#pragma once

#include <malloc.h>

#include <GLFW/glfw3.h>

#include "Renderer.h"
#include "Window.h"
#include "AABB.h"

typedef struct {

	int* tiles;

	int width, height, scale;
	int viewX, viewY;

	mat4 projection;

} World;


World* mk_World(Window* window, int width, int height, int scale) {

	World* self = (World*)malloc(sizeof(World));

	self->tiles = (int*)malloc(sizeof(int) * (width * height));

	for (int i = 0; i < (width * height) + 1; i++)
	{
		self->tiles[i] = 1;
	}

	// Save all the world size paramatres
	self->width = width;
	self->height = height;
	self->scale = scale;

	glm_mat4_copy((mat4)GLM_MAT4_IDENTITY_INIT, self->projection);

	return self;
}

static void WorldCalculateView(World* self, Window* window)
{
	self->viewX = (int)(window->width) / (self->scale * 100) + 3;
	self->viewY = (int)(window->height) / (self->scale * 100) + 2;
}

void WorldCorrectCamera(World* self, Camera* camera, Window* window)
{

	int w = -self->width * self->scale * 2;
	int h = self->height * self->scale * 2;

	if (camera->position[0] > -(window->width / 100.f) + self->scale)
		camera->position[0] = -(window->width / 100.f) + self->scale;

	if (camera->position[0] < w + (window->width / 100.f) + self->scale)
		camera->position[0] = w + (window->width / 100.f) + self->scale;

	if (camera->position[1] < (window->height / 100.f) - self->scale)
		camera->position[1] = (window->height / 100.f) - self->scale;

	if (camera->position[1] > h - (window->height / 100.f) - self->scale)
		camera->position[1] = h - (window->height / 100.f) - self->scale;

}

/*void WorldSetTileAt(World* self, Tile* tile, int x, int y)
{
	self->tiles[x + y * self->width] = tile->id;
	if (tile->isSolid)
	{
		self->bounding_boxes[x + y * self->width] = mk_AABB((vec3) { x * 2.f, -y * 2.f, 0 }, (vec3) { 1.f, 1.f, 0 });
	}
	else
	{
		self->bounding_boxes[x + y * self->width] = NULL;
	}
}
*/

Tile* WorldGetTileAt(World* self, int x, int y)
{
	return tiles[self->tiles[x + y * self->width]];
}

/*AABB* WorldGetTileBoundingBoxes(World* self, int x, int y)
{
	return self->bounding_boxes[x + y * self->width];
}*/

void WorldRender(World* self, Window* window, Renderer* renderer)
{
	WorldCorrectCamera(self, renderer->camera, window);
	WorldCalculateView(self, window);

	// calculate the cuantity of tiles to render
	int posX = ((int)(renderer->camera->position[0] + window->width / 100.f) / (self->scale * 2));
	int posY = ((int)(renderer->camera->position[1] - window->height / 100.f) / (self->scale * 2));

	for (int i = 0; i < self->viewX; ++i)
	{
	    for (int j = 0; j < self->viewY; ++j)
        {
           Tile* T = WorldGetTileAt(self, i-posX, j+posY);

            // Check if there is an exception
            if (T)
                RendererRenderTile(renderer, T, i - posX , -j-posY , self->projection);

        }
	}

}

void WorldTerminate(World* self)
{
	free(self->tiles);
//	free(self->bounding_boxes);

	free(self);
}




