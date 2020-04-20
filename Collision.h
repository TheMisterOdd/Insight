#pragma once

#include <cglm/cglm.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct
{
	vec3 distance;
	bool isIntersecting;

} Collision;

Collision* mk_Collision(vec3 distance, bool intersects)
{
	Collision* self = (Collision*)malloc(sizeof(Collision));

	glm_vec3_copy(distance, self->distance);
	self->isIntersecting = intersects;

	return self;
}
