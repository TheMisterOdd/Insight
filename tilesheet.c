#include "tilesheet.h"
#include <cglm/cglm.h>
#include <glad/glad.h>
#include <string.h>

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


void mk_tilesheet(tilesheet_t* self, const char* path, int x_amount, int y_amount)
{
	memset(self, 0, sizeof(tilesheet_t));
	mk_texture2d(self->texture, path);
	self->shader = mk_shader(TILESHEET_VS, TILESHEET_FS);

	glm_scale_make(self->scale, (vec3) { 1.0F / (float)(x_amount), 1.0F / (float)(y_amount), 0 });

	self->mvp_loc = glGetUniformLocation(self->shader, "MVP");
	self->texmod_loc = glGetUniformLocation(self->shader, "TexMod");

	self->x_amount = x_amount;
	self->y_amount = y_amount;
}

void tilesheet_bind(tilesheet_t* self)
{
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glEnable(GL_BLEND);
	shader_bind(self->shader);
	texture_bind(self->texture, 0);
}

void tilesheet_draw(tilesheet_t* self, mat4 proj, model_t m, int x, int y)
{
	mat4 translation = GLM_MAT4_IDENTITY_INIT;
	glm_translate_to(self->scale, (vec3) { (float)x, (float)y, 0 }, translation);

	glUniformMatrix4fv(self->mvp_loc, 1, GL_FALSE, proj[0]);
	glUniformMatrix4fv(self->texmod_loc, 1, GL_FALSE, translation[0]);

	model_begin(m);
	model_draw(m, GL_TRIANGLES);
	model_end();
}

INSIGHT_API void tilesheet_draw_at(tilesheet_t* self, mat4 proj, model_t m, int id) {
	int x = id / self->x_amount;
	int y = id % self->x_amount;
	tilesheet_draw(self, proj, m, x, y);
}

INSIGHT_API void tilesheet_unbind()
{
	texture_unbind();
	shader_unbind();
	glDisable(GL_BLEND);
}

INSIGHT_API void tilesheet_finalize(tilesheet_t* self)
{
	texture_finalize(self->texture);
	shader_finalize(self->shader);
}