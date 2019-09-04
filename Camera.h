#pragma once

// GL Includes
#include <glad/glad.h>
#include <math.h>
#include <cglm.h>


// Default camera values
const float YAW = -90.0f;
const float PITCH = 0.0f;
const float SPEED = 10.0f;
const float SENSITIVTY = 0.025f;
const float ZOOM = 45.0f;

// Defines several possible options for camera movement. Used as abstraction to stay away from window-system specific input methods
enum
{
	FORWARD,
	BACKWARD,
	LEFT,
	RIGHT,
	UP,
	DOWN
};

typedef struct 
{
	vec3 position;
	vec3 front;
	vec3 up;
	vec3 right;
	vec3 worldUp;

	// Eular Angles
	float yaw;
	float pitch;

	// Camera options
	float movementSpeed;
	float mouseSensitivity;
	float zoom;

} Camera;



void CameraUpdateVectors(Camera* self)
{
	// Calculate the new Front vector
	vec3 front = {
		(float)(cos(glm_rad(self->yaw)) * cos(glm_rad(self->pitch))),
		(float)(sin(glm_rad(self->pitch))),
		(float)(sin(glm_rad(self->yaw)) * cos(glm_rad(self->pitch)))
	};
	
	//glm_normalize_to(front, self->front);

	glm_cross(self->front, self->worldUp, front);
	glm_normalize_to(front, self->right);

	glm_cross(self->right, self->front, front);
	glm_normalize_to(front, self->up);
}

Camera* mk_Camera(vec3 position, vec3 up, float yaw, float pitch) 
{
	Camera* self = (Camera*)malloc(sizeof(Camera));

	glm_vec3_copy(position, self->position);
	glm_vec3_copy((vec3) { 0.0f, 0.0f, -1.0f }, self->front);
	glm_vec3_copy(up, self->worldUp);

	self->yaw = yaw;
	self->pitch = pitch;

	self->movementSpeed = SPEED;
	self->mouseSensitivity = SENSITIVTY;
	self->zoom = ZOOM;

	CameraUpdateVectors(self);

	return self;
}

void CameraGetViewMatrix(Camera* self, mat4 dest) 
{
	glm_lookat(
		self->position,
		(vec3) { self->position[0] + self->front[0], self->position[1] + self->front[1], self->position[2] + self->front[2] },
		self->up,
		dest
	);

}

void CameraProcessKeyboard(Camera* self, int direction, float deltaTime)
{
	float velocity = self->movementSpeed * deltaTime;

	if (direction == FORWARD)
	{
		glm_vec3_add(
			self->position,
			(vec3) {self->front[0] * velocity, self->front[1] * velocity, self->front[2] * velocity},
			self->position
		);
	}

	if (direction == BACKWARD)
	{
		glm_vec3_sub(
			self->position,
			(vec3) {self->front[0] * velocity, self->front[1] * velocity, self->front[2] * velocity},
			self->position
		);
	}

	if (direction == LEFT)
	{
		glm_vec3_sub(
			self->position,
			(vec3) {self->right[0] * velocity, self->right[1] * velocity, self->right[2] * velocity},
			self->position
		);
	}

	if (direction == RIGHT)
	{
		glm_vec3_add(
			self->position,
			(vec3) {self->right[0] * velocity, self->right[1] * velocity, self->right[2] * velocity},
			self->position
		);
	}

	if (direction == UP)
	{
		glm_vec3_add(
			self->position,
			(vec3) {self->up[0] * velocity, self->up[1] * velocity, self->up[2] * velocity},
			self->position
		);
	}

	if (direction == DOWN)
	{
		glm_vec3_sub(
			self->position,
			(vec3) {self->up[0] * velocity, self->up[1] * velocity, self->up[2] * velocity},
			self->position
		);
	}

}

void CameraProcessMouseMovement(Camera* self, float xOffset, float yOffset, float deltaTime, bool constrainPitch)
{
	
	xOffset *= self->mouseSensitivity ;
	yOffset *= self->mouseSensitivity ;

	self->yaw += xOffset;
	self->pitch += yOffset;

	// Make sure that when pitch is out of bounds, screen doesn't get flipped
	if (constrainPitch)
	{
		if (self->pitch > 89.0f)
		{
			self->pitch = 89.0f;
		}

		if (self->pitch < -89.0f)
		{
			self->pitch = -89.0f;
		}
	}

	// Update Front, Right and Up Vectors using the updated Eular angles
	CameraUpdateVectors(self);
}

void CameraProcessMouseScroll(Camera* self, float yOffset)
{
	if (self->zoom >= 1.0f && self->zoom <= 45.0f)
	{
		self->zoom -= yOffset;
	}

	if (self->zoom <= 1.0f)
	{
		self->zoom = 1.0f;
	}

	if (self->zoom >= 45.0f)
	{
		self->zoom = 45.0f;
	}
}