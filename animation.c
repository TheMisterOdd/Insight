#include "animation.h"

#include <GLFW/glfw3.h>

void mk_animation(animation_t* self, const char* path, unsigned int x_amount, int fps) {
	mk_tilesheet(&self->tiles, path, x_amount, 1);

	self->tile_index = 0;
	self->x_amount = x_amount;

	self->fps = 1.0 / fps;
	self->last_time = glfwGetTime();
	self->elapsed_time = 0.0;
}

/* needs to bind and draw manually the tilesheet */
void animation_update(animation_t* self) {
	double current_time = glfwGetTime();
	self->elapsed_time += current_time - self->last_time;

	if (self->elapsed_time >= self->fps) {
		self->elapsed_time = 0.0;
		self->tile_index++;
	}

	if (self->tile_index >= self->x_amount) self->tile_index = 0;

	self->last_time = current_time;
}

void animation_stop(animation_t* self) {
	self->tile_index = 0;
}

void animation_finalize(animation_t* self) {
	tilesheet_finalize(&self->tiles);
}