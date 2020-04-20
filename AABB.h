#pragma once

#include "Collision.h"
#include <cglm/cglm.h>
#include <stdlib.h>
#include <stdbool.h>

typedef struct {

	vec3 center, halfExtent;

} AABB;

static float ABS(float x)
{
	return (x < 0) ? -x : x;
}

char* glm_vec3_str(vec3 vec)
{
	char* string = (char*)malloc(sizeof(char) * 512);
	sprintf_s(string, 512, "[%.2f, %.2f, %.2f]", vec[0], vec[1], vec[2]);
	return string;
}

AABB* mk_AABB(vec3 center, vec3 halfExtent)
{
	AABB* self = (AABB*)malloc(sizeof(AABB));

	glm_vec3_copy(center, self->center);
	glm_vec3_copy(halfExtent, self->halfExtent);

	return self;
}

Collision* AABBGetCollision(AABB* self, AABB* box)
{
	vec3 distance = { box->center[0] - self->center[0], box->center[1] - self->center[1], 0 };
	distance[0] = ABS(distance[0]);
	distance[1] = ABS(distance[1]);

	glm_vec3_sub(
		distance,
		(vec3) { self->halfExtent[0] + box->halfExtent[0], self->halfExtent[1] + box->halfExtent[1], 0 },
		distance
	);

	printf("%s\n", glm_vec3_str(distance));
	return mk_Collision(distance, distance[0] < 0 && distance[1] < 0);
}

void AABBCorrectPosition(AABB* self, Collision* data, AABB* box)
{
	vec3 correction = { box->center[0] - self->center[0], box->center[1] - self->center[1], 0 };

	if (data->distance[0] > data->distance[1])
	{
		if (correction[0] > 0)
		{
			glm_vec3_add(self->center, (vec3) { data->distance[0], 0, 0 }, self->center);
		}
		else
		{
			glm_vec3_add(self->center, (vec3) { -data->distance[0], 0, 0 }, self->center);
		}
	}
	else
	{
		if (correction[1] > 0)
		{
			glm_vec3_add(self->center, (vec3) { 0, data->distance[1], 0 }, self->center);
		}
		else
		{
			glm_vec3_add(self->center, (vec3) { 0, -data->distance[1], 0 }, self->center);
		}
	}
}
