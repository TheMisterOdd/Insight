#ifndef _CAMERA_H_
#define _CAMERA_H_

#include <cglm/cglm.h>
#include "Insight.h"


typedef mat4 camera_t[2];

/*
	@breif - Initialize a camera with an orthographic projection
	@param[out] self The camera
	@param[in] width Screen width
	@param[in] height Screen height
*/
INSIGHT_API void camera(camera_t self, int width, int height);

/*
	@breif - Initialize a camera with an orthographic projection
	@param[in] self The camera
	@param[in] dest The destination vector, where the position will be saved
*/
INSIGHT_API void camera_get_pos(camera_t self, vec3 dest);

INSIGHT_API void camera_set_pos(camera_t self, vec3 pos);

INSIGHT_API void camera_update(camera_t self, int width, int height);

INSIGHT_API void camera_get_matrix(camera_t self, mat4 model, mat4 mvp);

#ifdef CAMERA_IMPL

INSIGHT_API void camera(camera_t self, int width, int height)
{
	glm_ortho(
		(float)-width / 125.0F, (float)width / 125.0F,
		(float)-height / 125.0F, (float)height / 125.0F,
		-1.0F, 1.0F,
		self[0]
	);

	glm_mat4_identity(self[1]);
}
INSIGHT_API void camera_get_pos(camera_t self, vec3 dest)
{
	dest[0] = self[1][3][0];
	dest[1] = self[1][3][1];
	dest[2] = self[1][3][2];
}
INSIGHT_API void camera_set_pos(camera_t self, vec3 pos)
{
	self[1][3][0] = pos[0];
	self[1][3][1] = pos[1];
	self[1][3][2] = pos[2];
}
INSIGHT_API void camera_update(camera_t self, int width, int height)
{
	glm_ortho(
		(float)-width / 125.0F, (float)width / 125.0F,
		(float)-height / 125.0F, (float)height / 125.0F,
		-1.0F, 1.0F,
		self[0]
	);
}
INSIGHT_API void camera_get_matrix(camera_t self, mat4 model, mat4 mvp)
{
	glm_mul(self[0], self[1], mvp);
	glm_mul(mvp, model, mvp);
}

#endif

#endif