#pragma once

#include <cglm/cglm.h>
typedef struct
{
	vec3 position;
	mat4 projection;

}Camera;

void CameraSetProjection(Camera* self, int width, int height)
{
	glm_ortho((float)-width/2.f, (float)width/2.f, (float)-height/2.f, (float)height/2.f, -1.f, 1.f, self->projection);
}

Camera* mk_Camera(int width, int height)
{
	Camera* self = (Camera*)malloc(sizeof(Camera));

	glm_vec3_copy((vec3) { 0, 0, 0 }, self->position);

	CameraSetProjection(self, width, height);

	return self;
}

void CameraSetPosition(Camera* self, vec3 position)
{
	glm_vec3_copy(position, self->position);
}

void CameraAddPosition(Camera* self, vec3 addition)
{
	glm_vec3_add(self->position, addition, self->position);
}

void CameraGetProjection(Camera* self, mat4 get, float scale) {

	glm_scale_to(self->projection, (vec3) { 50.f + scale, 50.f + scale, 0 }, get);
}

void CameraGetTranslatedProjection(Camera* self, mat4 get, int scale)
{
	glm_scale_to(self->projection, (vec3) { 50.f + scale, 50.f + scale, 0 }, get);
	glm_translate(get, self->position);
}



