#pragma once

#include <malloc.h>
#include <stdio.h>

#include "Texture.h"

typedef struct {

	bool isSolid;
	int id;
	Texture* texture;

} Tile;

static int nor = 0;
Tile* tiles[16];

Tile* mk_Tile(const char* texture_str) 
{
	Tile* self = (Tile*)malloc(sizeof(Tile*));
	self->id = nor;
	
	self->texture = mk_Texture(texture_str, GL_FALSE);
	
	if (tiles[self->id] != NULL) {
		fprintf(stderr, "Tile at %d is already being used", self->id);
		exit(-1);
	}

	tiles[self->id] = self;
	nor++;

	return self;
}

Tile* TileSetSolid(Tile* self) 
{
	self->isSolid = true;
	return self;
}
