#ifndef _ANIMATION_H_
#define _ANIMATION_H_

#include "Tilesheet.h"

typedef struct
{
	/* render */
	tilesheet_t tiles;
	unsigned int tile_index, x_amount;

	/* time */
	double elapsed_time, last_time, fps;
} animation_t;

/* animations only function on X axes by the time being */
void mk_animation(animation_t* self, const char* path, unsigned int x_amount, int fps);

void animation_update(animation_t* self);

void animation_stop(animation_t* self);

void animation_finalize(animation_t* self);


#endif