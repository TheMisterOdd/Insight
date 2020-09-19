#ifndef _TILE_H_
#define _TILE_H_

#include "Input.h"
#include "Shader.h"
#include "Texture.h"
#include <cglm/cglm.h>

typedef struct 
{
	Insight_Texture texture;

	GLuint shader;
	mat4 translation, scale;

	uint32_t x_amount, y_amount;
	int mvp_loc, texmod_loc;

} Insight_Tilesheet;

INSIGHT_API void insight_tilesheet(Insight_Tilesheet* self, const char* path, int x_amount, int y_amount);

INSIGHT_API void insight_tilesheet_bind(Insight_Tilesheet* self);

INSIGHT_API void insight_tilesheet_draw(Insight_Tilesheet* self, mat4* proj, Insight_Model m, int x, int y);

INSIGHT_API void insight_tilesheet_draw_at(Insight_Tilesheet* self, mat4* proj, Insight_Model m, int id);

INSIGHT_API void insight_tilesheet_unbind();

INSIGHT_API void insight_tilesheet_finalize(Insight_Tilesheet* self);

#ifdef TILESHEET_IMPL

#define TILESHEET_VS\
	"#version 330 core\n"\
	"layout(location = 0) in vec3 m_Position;"\
	"layout(location = 1) in vec2 m_TexCoords;"\
	"out vec2 TexCoords;\n"\
	"uniform mat4 MVP;\n"\
	"uniform mat4 TexMod;\n"\
	"void main() {\n"\
	"TexCoords = (TexMod * vec4(m_TexCoords, 0, 1)).xy;\n"\
	"gl_Position = MVP * vec4(m_Position, 1.0);\n"\
	"}\n"\

#define TILESHEET_FS\
	"#version 330 core\n"\
	"in vec2 TexCoords;\n"\
	"uniform sampler2D Texture;\n"\
	"void main() {\n"\
	"gl_FragColor = texture2D(Texture, TexCoords);\n"\
	"}\n"\


INSIGHT_API void insight_tilesheet(Insight_Tilesheet* self, const char* path, int x_amount, int y_amount)
{
	insight_texture2d(self->texture, path);
	self->shader = insight_shader(TILESHEET_VS, TILESHEET_FS);

	glm_mat4_identity(self->translation);
	glm_scale_make(self->scale, (vec3) { 1.0F / (float)(x_amount), 1.0F / (float)(y_amount), 0 });

	self->mvp_loc = glGetUniformLocation(self->shader, "MVP");
	self->texmod_loc = glGetUniformLocation(self->shader, "TexMod");

	self->x_amount = x_amount;
	self->y_amount = y_amount;
}

INSIGHT_API void insight_tilesheet_bind(Insight_Tilesheet* self)
{
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	insight_shader_bind(self->shader);
	insight_texture_bind(self->texture, 0);
}

INSIGHT_API void insight_tilesheet_draw(Insight_Tilesheet* self, mat4* proj, Insight_Model m, int x, int y)
{
	glm_translate_to(self->scale, (vec3) { (float)x, (float)y, 0 }, self->translation);

	glUniformMatrix4fv(self->mvp_loc, 1, GL_FALSE, (float*)*proj);
	glUniformMatrix4fv(self->texmod_loc, 1, GL_FALSE, (float*)self->translation);

	insight_model_begin(m);
	insight_model_draw(m, GL_TRIANGLES);
	insight_model_end();
}

INSIGHT_API void insight_tilesheet_draw_at(Insight_Tilesheet* self, mat4* proj, Insight_Model m, int id) {
	int x = id / self->x_amount;
	int y = id % self->x_amount;
	insight_tilesheet_draw(self, proj, m, x, y);
}

INSIGHT_API void insight_tilesheet_unbind()
{
	insight_texture_unbind();
	insight_shader_unbind();
	glDisable(GL_BLEND);
}

INSIGHT_API void insight_tilesheet_finalize(Insight_Tilesheet* self)
{
	insight_texture_finalize(self->texture);
	insight_shader_finalize(self->shader);
}

#endif


#endif