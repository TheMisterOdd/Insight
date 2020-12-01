#ifndef _CAMERA_H_
#define _CAMERA_H_

#include "insight.h"
#include <cglm/cglm.h>

typedef mat4 camera_t[2];

/*
	@breif - Initialize a camera with an orthographic projection
	@param[out] self The camera
	@param[in] width Screen width
	@param[in] height Screen height
*/
INSIGHT_API void ortho(camera_t self, int width, int height);

/*
	@breif - Initialize a camera with an orthographic projection
	@param[in] self The camera
	@param[in] dest The destination vector, where the position will be saved
*/
INSIGHT_API void camera_get_pos(camera_t self, vec3 dest);

INSIGHT_API void camera_set_pos(camera_t self, vec3 pos);

INSIGHT_API void ortho_update(camera_t self, int width, int height);

INSIGHT_API void camera_get_matrix(camera_t self, mat4 model, mat4 mvp);

#endif