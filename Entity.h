#pragma once

#include "Model.h"
#include "Window.h"
#include "World.h"
#include "Animation.h"
#include "AABB.h"

typedef struct {

	Model model;
	//AABB* bounding_box;

	int health, scale;

	mat4 projection;
	vec3 position;

} Entity;


Entity* mk_Entity(int health, int scale, vec3 start_pos) {

	Entity* self = (Entity*)malloc(sizeof(Entity));

	self->model = mk_Model(); // new Model

	self->health = health;
	self->scale = scale;

	glm_mat4_copy((mat4)GLM_MAT4_IDENTITY_INIT, self->projection);
	glm_vec3_copy(start_pos, self->position);

	//self->bounding_box = mk_AABB(self->position, (vec3) { self->scale, self->scale, 0 });
	return self;
}


void EntityUpdate(Entity* self, Player* follow, float deltaTime, Window* window, Camera* camera, World* world) {

    static float lastTime = 0.0f;

	vec3 movement = { self->position[0], self->position[1], 0 };
	CameraGetTranslatedProjection(camera, self->projection, self->scale);
	glm_translate(self->projection, self->position);

	if (follow != NULL)
	{
		vec3 distance;
		glm_vec3_sub(follow->position, self->position, distance);

        float d = sqrt(distance[0] * distance[0] + distance[1] * distance[1]);
        //glm_vec3_add(position, distance, position);

        if (d <= 15)
		{
            self->position[0] += distance[0] * 0.75f * deltaTime;
            self->position[1] += distance[1] * 0.75f * deltaTime;

            if (glfwGetTime() - lastTime > 1.0f)
            {
                if (floor(d) == 0)
                {
                    follow->health--;
                    follow->position[0] += 2;

                    AnimationBind(DAMAGE, 1);
                }

                lastTime = glfwGetTime();
            }
		}

	}

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

void EntityRender(Entity* self, Player* follow, Shader shader, Camera* camera, float deltaTime, Window* window, World* world) {

	EntityUpdate(self, follow, deltaTime, window, camera, world); // Update The Player

	ShaderBind(shader); // Bind Shader

	glUniformMatrix4fv(glGetUniformLocation(shader, "target"), 1, GL_FALSE, (float*)self->projection);
	glUniform1i(glGetUniformLocation(shader, "Texture"), 0);

	ModelDraw(self->model); // Draw The model

	glBindTexture(GL_TEXTURE_2D, 0); // Unbinding the Texture
	glUseProgram(0);				 // Unbinding the Shader
}
