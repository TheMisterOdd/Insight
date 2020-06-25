#ifndef _MAIN_H_
#define _MAIN_H_


#include "Insight.h"

#define INSIGHT_MODEL_IMPL
#include "Model.h"

#define INSIGHT_TEXTURE_IMPL
#include "Texture.h"

#define INSIGHT_SHADER_IMPL
#include "Shader.h"

#include <stdio.h>
#include <cglm/cglm.h>
#include <assert.h>

struct objects 
{
	model_t* model, * plane;
	shader_t* cube_shader, * screen_shader;
	texture_t* texture;

	fbo_t* fbo;

	mat4 projection, view, object;
	mat4 screen_translation;

	int proj_loc, view_loc, obj_loc, plane_obj_loc;
};

void init(struct objects* objects, window_t* wnd)
{
	/* --- Plane set up --- */
	float vertices[] =
	{	// Positions:		   TexCoords:
		-1.0f, 1.0f, -1.0f,    0, 0,
		-1.0f, 1.0f,  1.0f,    0, 1,
		 1.0f, 1.0f,  1.0f,    1, 1,
		 1.0f, 1.0f, -1.0f,    1, 0,

		// Left
		-1.0f,  1.0f,  1.0,    0, 0,
		-1.0f, -1.0f,  1.0f,   1, 0,
		-1.0f, -1.0f, -1.0f,   1, 1,
		-1.0f,  1.0f, -1.0f,   0, 1,

		// Right
		 1.0f,  1.0f,  1.0f,    1, 1,
		 1.0f, -1.0f,  1.0f,    0, 1,
		 1.0f, -1.0f, -1.0f,    0, 0,
		 1.0f,  1.0f, -1.0f,    1, 0,

		// Front
		 1.0f,  1.0f, 1.0f,    1, 1,
		 1.0f, -1.0f, 1.0f,    1, 0,
		-1.0f, -1.0f, 1.0f,    0, 0,
		-1.0f,  1.0f, 1.0f,    0, 1,

		// Back
		 1.0f,  1.0f, -1.0f,    0, 0,
		 1.0f, -1.0f, -1.0f,    0, 1,
		-1.0f, -1.0f, -1.0f,    1, 1,
		-1.0f,  1.0f, -1.0f,    1, 0,

		// Bottom
		-1.0f, -1.0f, -1.0f,    1, 1,
		-1.0f, -1.0f,  1.0f,    1, 0,
		 1.0f, -1.0f,  1.0f,    0, 0,
		 1.0f, -1.0f, -1.0f,    0, 1,
	};

	const GLuint indices[] =
	{	
		// Top
		0, 1, 2,
		0, 2, 3,

		// Left
		5, 4, 6,
		6, 4, 7,

		// Right
		8, 9, 10,
		8, 10, 11,

		// Front
		13, 12, 14,
		15, 14, 12,

		// Back
		16, 17, 18,
		16, 18, 19,

		// Bottom
		21, 20, 22,
		22, 20, 23
	};

	float plane_vert[] =
	{	// Positions:				Color:
		-1.0f, -1.0f, +0.0f,		+0.0f, +0.0f,
		+1.0f, -1.0f, +0.0f,		+1.0f, +0.0f,
		+1.0f, +1.0f, +0.0f,		+1.0f, +1.0f,
		-1.0f, +1.0f, +0.0f,		+0.0f, +1.0f
	};

	GLuint plane_ind[] =
	{
		0, 1, 2,
		2, 3, 0
	};

	objects->model = model_init(vertices, indices, sizeof(vertices), 36);
	objects->plane   = model_init(plane_vert, plane_ind, sizeof(plane_vert), 6);
	/* --- End of plane set up --- */


	/* --- Shader set up --- */
	objects->screen_shader  = shader_init("resources/shader/plane.vert.glsl", "resources/shader/plane.frag.glsl");
	objects->cube_shader = shader_init("resources/shader/cube.vert.glsl", "resources/shader/cube.frag.glsl");
	/* --- End of shader set up --- */


	/* --- Texture set up --- */
	objects->texture = texture_init("resources/luigi.png");
	/* --- End texture set up --- */


	/* --- Matrix set up --- */
	glm_perspective(45.0f, (float)wnd->width / (float)wnd->height, 0.1f, 1000.f, objects->projection);
	glm_lookat((vec3) { 0.0f, 0.0f, 5.0f }, (vec3) { 0.0f, 0.0f, 0.0f }, (vec3) { 0.0f, 1.0f, 0.0f }, objects->view);
	glm_translate_make(objects->screen_translation, (vec3) { 0.0f, 0.0f, 0.0f });

	objects->proj_loc = glGetUniformLocation(objects->cube_shader->id, "projection");
	objects->view_loc = glGetUniformLocation(objects->cube_shader->id, "view");
	objects->obj_loc = glGetUniformLocation(objects->cube_shader->id, "object");

	/* --- End set up --- */

	/* --- Framebuffer object creation --- */
	objects->plane_obj_loc = glGetUniformLocation(objects->screen_shader->id, "object");
	objects->fbo = fbo_init(wnd->vidMode->width, wnd->vidMode->height);

	/* --- End of framebuffer object creation --- */


}

void update(struct objects* objects, window_t* wnd)
{
	/* --- Cube shader handeling --- */
	shader_bind(objects->cube_shader);

	glm_rotate_make(objects->object, (float)glfwGetTime(), (vec3) { 1.0f, 0.0f, 1.0f });

	glUniformMatrix4fv(objects->proj_loc, 1, GL_FALSE, (float*)objects->projection);
	glUniformMatrix4fv(objects->view_loc, 1, GL_FALSE, (float*)objects->view);
	glUniformMatrix4fv(objects->obj_loc, 1, GL_FALSE, (float*)objects->object);
	/* --- End --- */


	/* --- Plane shader handeling --- */
	shader_bind(objects->screen_shader);
	glUniformMatrix4fv(objects->plane_obj_loc, 1, GL_FALSE, (float*)objects->screen_translation);
	shader_unbind();
	/* --- End --- */

}

 
void has_resized(struct objects* objects, int width, int height)
{
	/* --- Update the projection if the window is resized --- */
	glm_perspective(45.0f, (float)width / (float)height, 0.1f, 1000.f, objects->projection);
	
}

void draw(struct objects* objects, window_t* wnd)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	/* Draw into custom Framebuffer */
	{glClearColor(1.0f, 0.0f, 0.0f, 1.0f);
	fbo_bind(objects->fbo);

	glClear(GL_COLOR_BUFFER_BIT | GL_STENCIL_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	
	shader_bind(objects->cube_shader);
	texture_bind(objects->texture, 0);

	model_begin(objects->model);
	model_draw(objects->model);
	model_end();

	texture_unbind();
	shader_unbind();

	fbo_unbind(wnd->width, wnd->height);}

	/* End */
	/* Draw onto screen */
	shader_bind(objects->screen_shader);
	texture_bind(objects->fbo->texture, 0);

	model_begin(objects->plane);
	model_draw(objects->plane);
	model_end();

	texture_unbind();
	shader_unbind();
	/* End */
}

void input(struct objects* objects, window_t* wnd)
{
	if (input_is_key_pressed(wnd->input, GLFW_KEY_ESCAPE))
	{
		glfwSetWindowShouldClose(wnd->wnd_hndl, GLFW_TRUE);
	}

	if (input_is_key_pressed(wnd->input, GLFW_KEY_F12))
	{
		if (texture_make_screenshot()) 
		{
			printf("Screenshot has been taken!\n"); 
		}
	}
}

void terminate(struct objects* objects)
{
	model_terminate(objects->model);
	model_terminate(objects->plane);

	shader_terminate(objects->cube_shader);
	shader_terminate(objects->screen_shader);

	texture_terminate(objects->texture);

	fbo_terminate(objects->fbo);
}


int main(void)
{
	struct objects a;

	insight_init_ptr        = (void*)init;
	insight_update_ptr      = (void*)update;
	insight_has_resized_ptr = (void*)has_resized;
	insight_draw_ptr        = (void*)draw;
	insight_input_ptr       = (void*)input;
	insight_terminate_ptr   = (void*)terminate;
	
	insight_engine(&a, "Insight", 0);
	


	return 0;
}

#endif
