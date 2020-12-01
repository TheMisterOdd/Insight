#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "Insight.h"
#include "Animation.h"
#include "camera.h"
#include <math.h>
#include <cglm/cglm.h>

enum {
	MOVE,
	ROLL,
	ATTACK
};

typedef struct {
	/* physics related: */
	vec3 position, velocity, acceleration, last_pos;
	mat4 object;

	/* render */
	animation_t animations[3];
	size_t animation_index;

	double last_time;

} Player;

/*! Creates a new player */
INSIGHT_API void mk_player(Player* self, vec3 starting_pos);

/*! Updates the given players */
INSIGHT_API void player_update(Player* self, double dt);

/*! Get Keyboard input */
INSIGHT_API void player_input(Player* self, input_t* input);

/*! Get draw player */
INSIGHT_API void player_draw(Player* self, model_t model, camera_t cam, double dt);

/*! free memory of player */
INSIGHT_API void player_finalize(Player* self);

#ifdef PLAYER_IMPL

INSIGHT_API void mk_player(Player* self, vec3 starting_pos) {
	memset(self, 0, sizeof(Player));

	glm_vec3_copy(self->position, starting_pos);

	mk_animation(&self->animations[0], "resources/Player_Walk_x.png", 6, 12);
	mk_animation(&self->animations[1], "resources/Player_Walk_y1.png", 6, 12);
	mk_animation(&self->animations[2], "resources/Player_Walk_y2.png", 6, 12);

	glm_mat4_identity(self->object);
}

INSIGHT_API void player_update(Player* self, double dt) {
	glm_vec3_copy(self->position, self->last_pos);

	glm_vec3_add(
		self->position,
		(vec3) {
			self->velocity[0] * (float)dt, self->velocity[1] * (float)dt, 0
		},
		self->position
	);

	glm_vec3_sub(
		self->velocity,
		(vec3) {
			self->acceleration[0] * (float)dt, self->acceleration[1] * (float)dt, 0
		},
		self->velocity
	);

	glm_translate_make(self->object, self->position);

	ivec3 movement = {
		(int)(((self->position[0] - self->last_pos[0]) * 0.25F) / dt),
		(int)(((self->position[1] - self->last_pos[1]) * 0.25F) / dt)
	};

	if (movement[0] > 0 || movement[0] < 0) {
		if (movement[0] < 0)
			glm_rotate(self->object, glm_rad(180.0F), (vec3) { 0, 1, 0 });
		self->animation_index = 0;
		animation_update(&self->animations[self->animation_index]);
	}
	else if (movement[1] > 0 || movement[1] < 0) {
		if (movement[1] < 0)
			self->animation_index = 2;
		else
			self->animation_index = 1;
		animation_update(&self->animations[self->animation_index]);
	}
	else {
		self->animation_index = 0;
		animation_stop(&self->animations[self->animation_index]);
	}

	self->acceleration[0] = self->velocity[0] * 8;
	self->acceleration[1] = self->velocity[1] * 8;
}

INSIGHT_API void player_input(Player* self, input_t* input) {
	if (input_is_key_down(input, GLFW_KEY_W)) {
		self->velocity[1] = 10;
	}

	if (input_is_key_down(input, GLFW_KEY_S)) {
		self->velocity[1] = -10;
	}

	if (input_is_key_down(input, GLFW_KEY_A)) {
		self->velocity[0] = -10;
	}

	if (input_is_key_down(input, GLFW_KEY_D)) {
		self->velocity[0] = 10;
	}

	
}

INSIGHT_API void player_draw(Player* self, model_t model, camera_t cam, double dt) {
	animation_t* current = &self->animations[self->animation_index];
	tilesheet_bind(&current->tiles);

	mat4 mvp = GLM_MAT4_IDENTITY_INIT;

	glm_vec3_lerp(cam[1][3], (vec3) { -self->position[0], -self->position[1], 0 }, 3.5F * (float)dt, cam[1][3]);
	camera_get_matrix(cam, self->object, mvp);

	tilesheet_draw(&current->tiles, mvp, model, current->tile_index, 0);
	tilesheet_unbind();
}


INSIGHT_API void player_finalize(Player* self) {
	animation_finalize(&self->animations[0]);
	animation_finalize(&self->animations[1]);
	animation_finalize(&self->animations[2]);
}


#endif

#endif
