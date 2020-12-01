#ifndef _TILESHEET_H_
#define _TILESHEET_H_

#include "texture.h"
#include "shader.h"
#include "model.h"
#include <cglm/types.h>


typedef struct
{
	texture_t texture;

	unsigned int shader;
	int mvp_loc, texmod_loc;

	mat4 scale;

	unsigned int x_amount, y_amount;

} tilesheet_t;

INSIGHT_API void mk_tilesheet(tilesheet_t* self, const char* path, int x_amount, int y_amount);

INSIGHT_API void tilesheet_bind(tilesheet_t* self);

INSIGHT_API void tilesheet_draw(tilesheet_t* self, mat4 proj, model_t m, int x, int y);

INSIGHT_API void tilesheet_draw_at(tilesheet_t* self, mat4 proj, model_t m, int id);

INSIGHT_API void tilesheet_unbind();

INSIGHT_API void tilesheet_finalize(tilesheet_t* self);


#endif
