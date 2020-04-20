#pragma once

#include "Model.h"
#include "Window.h"
#include "World.h"
#include "Animation.h"
#include "AABB.h"

static float glm_vec3_squared_len(vec3 a)
{
	return (a[0] * a[0]) + (a[1] * a[1]) + (a[2] * a[2]);
}

static float glm_vec3_len(vec3 a)
{
	return (float)sqrt(glm_vec3_squared_len(a));
}

typedef struct {

	Model model;
	//AABB* bounding_box;

	int health, scale;

	mat4 projection;
	vec3 position;

} Player;

Animation *WALKING, *IDLE, *DAMAGE;
void PlayerInitAnimations()
{
	WALKING = mk_Animation("resources/textures/animations/walking/player_", 2, 1);
	IDLE    = mk_Animation("resources/textures/animations/walking/player_", 2, 1);
	DAMAGE  = mk_Animation("resources/textures/animations/damage/damage_", 2, 1);
}

void PlayerUnloadAnimations()
{
	free(WALKING);
	free(IDLE);
}

Player* mk_Player(int health, int scale, vec3 start_pos) {

	Player* self = (Player*)malloc(sizeof(Player));

	self->model = mk_Model(); // make a new Model

	self->health = health;
	self->scale = scale;

	glm_mat4_copy((mat4)GLM_MAT4_IDENTITY_INIT, self->projection);
	glm_vec3_copy(start_pos, self->position);

	//self->bounding_box = mk_AABB(self->position, (vec3) { self->scale, self->scale, 0 });

	return self;
}

void PlayerUpdate(Player* self, float deltaTime, Window* window, Camera* camera, World* world) {

	vec3 movement = { self->position[0], self->position[1], 0 }, target;
	CameraGetTranslatedProjection(camera, self->projection, self->scale);
	glm_translate(self->projection, self->position);

	const float step = 2.0f;

	if (InputIsKeyPressed(window->input, GLFW_KEY_W))
	{
		glm_vec3_add(self->position, (vec3) { 0, step, 0 }, self->position);
	}

	if (InputIsKeyPressed(window->input, GLFW_KEY_S))
	{
		glm_vec3_add(self->position, (vec3) { 0, -step, 0 }, self->position);
	}

	if (InputIsKeyPressed(window->input, GLFW_KEY_A))
	{
		glm_vec3_add(self->position, (vec3) { -step, 0, 0 }, self->position);
		glm_rotate_y(self->projection, glm_rad(180), self->projection);
	}

	if (InputIsKeyPressed(window->input, GLFW_KEY_D))
	{
		glm_vec3_add(self->position, (vec3) { step, 0, 0 }, self->position);
	}

	{
		/*AABB* boxes[25];
		for (int i = 0; i < 5; i++)
		{
			for (int j = 0; j < 5; j++)
			{
				boxes[i + j * 5] =
					WorldGetTileBoundingBoxes(
						world,
						(int)(((self->position[0] / 2) + 0.5f) - (5 / 2)) + i,
						(int)(((-self->position[1] / 2) + 0.5f) - (5 / 2)) + j
					);
			}
		}

		AABB* box = NULL;
		for (int i = 0; i < 25; i++)
		{
			if (boxes[i] != NULL && boxes[i]->center != NULL && boxes[i]->halfExtent != NULL)
			{
				if (box == NULL)
				{
					box = boxes[i];
				}

				vec3 lenght1 = { 0, 0, 0 };
				glm_vec3_sub(box->center, self->position, lenght1);

				vec3 lenght2 = { 0, 0, 0 };
				glm_vec3_sub(boxes[i]->center, self->position, lenght2);

				if (glm_vec3_squared_len(lenght1) > glm_vec3_squared_len(lenght2))
				{
					box = boxes[i];
				}

			}
		}

		if (box != NULL)
		{
			Collision* data = AABBGetCollision(self->bounding_box, box);
			if (data->isIntersecting)
			{
				AABBCorrectPosition(self->bounding_box, data, box);
				glm_vec3_copy((vec3) { self->bounding_box->center[0], 0, 0 }, self->position);
			}
		}*/

	}

	glm_vec3_lerp(
		camera->position,
		(vec3) { self->position[0] * -world->scale, self->position[1] * -world->scale, 0 },
		3.f * deltaTime,
		target
	);
	CameraSetPosition(camera, target); // Update camera

	glm_vec3_sub(self->position, movement, movement);
	if (movement[0] == 0 && movement[1] == 0)
	{
		AnimationBind(IDLE, 0); // use the 'idle' animation
	}
	else
	{
		AnimationBind(WALKING, 0); // if the player is moving, use the 'walk' animation
	}
}

void PlayerRender(Player* self, Shader shader, Camera* camera, float deltaTime, Window* window, World* world) {

	PlayerUpdate(self, deltaTime, window, camera, world); // Update The Player

	ShaderBind(shader); // Bind Shader

	glUniformMatrix4fv(glGetUniformLocation(shader, "target"), 1, GL_FALSE, (float*)self->projection);
	glUniform1i(glGetUniformLocation(shader, "Texture"), 0);

	ModelDraw(self->model); // Draw The model

	glBindTexture(GL_TEXTURE_2D, 0); // Unbinding the Texture
	glUseProgram(0);				 // Unbinding the Shader
}
