#ifndef _MAIN_H_
#define _MAIN_H_

#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

#include <glad/glad.h>

/* engine includes */
#include "window.h"
#include "Texture.h"
#include "Model.h"
#include "Shader.h"

/* game includes */
#define PLAYER_IMPL
#define WORLD_IMPL
#include "player.h"
#include "world.h"

#include <stdio.h>
static bool window_has_resized = false;
void viewport(GLFWwindow* window, int fbW, int fbH) {
	window_has_resized = true;
	glViewport(0, 0, fbW, fbH);
}

int main(void)
{
	/* checks if can init glfw */
	if (!glfwInit())
		return EXIT_FAILURE;

	_window_resize_callback = viewport;
	struct window* wnd = mk_window(1280, 720, "OpenGL", 0);
	window_set_cursor(wnd, "resources/cursor.png");

	glEnable(GL_CULL_FACE);
	glCullFace(GL_BACK);

	const float vertices[] = {
		/* Positions:				Textures: */
		-1.0F, -1.0F, +0.0f,		+0.0F, +0.0F,
		+1.0F, -1.0F, +0.0f,		+1.0F, +0.0F,
		+1.0F, +1.0F, +0.0f,		+1.0F, +1.0F,
		-1.0F, +1.0F, +0.0f,		+0.0F, +1.0F
	};

	const unsigned int indices[] = {
		0, 1, 2,
		2, 3, 0
	};

	model_t m;
	mk_model(m, vertices, sizeof(float) * 20, indices, sizeof(unsigned int) * 6);

	camera_t cam;
	ortho(cam, wnd->width, wnd->height);

	Player p;
	mk_player(&p, (vec3) { 0, 0, 0 });

	texture_t tex0;
	mk_texture2d(tex0, "resources/GrassBackground.png");
	unsigned int shader = mk_shader(DEFAULT_VS, DEFAULT_FS);

	/* CREA EL MUNDO */
	world_t w;
	world(&w, 16, 16, 1);
	world_set_tile(&w, 0, 0, tex0);

	double last = glfwGetTime();
	while (window_is_running(wnd)) {
		/* fps, timing... */
		double dt = wnd->deltaTime;

		double current = glfwGetTime();
		if (current - last > 0.25) {
			printf("\r%.0f FPS", 1.0 / dt);
			last = current;
		}

		/* if window is resized: */
		if (window_has_resized) {
			ortho_update(cam, wnd->width, wnd->height);
			window_has_resized = false;
		}

		/* don't need to clear any buffer right now */
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		/* world */
		world_render(&w, m, wnd, cam, shader);
		/* end world */

		player_input(&p, wnd->input);
		player_update(&p, dt);
		player_draw(&p, m, cam, dt);

		if (input_is_key_pressed(wnd->input, GLFW_KEY_F3)) {
			screenshot();
		}

	}

	/* finalize (memory deallocation) */
	{
		/* game */
		player_finalize(&p);
		world_finalize(&w);

		/* engine */
		model_finalize(m);
		shader_finalize(shader);
		WindowFinalize(wnd);
		glfwTerminate();
	}

	return 0;
}

#endif
