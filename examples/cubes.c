#include "Window.h"
#include "Shaders.h"
#include "Model.h"
#include "Camera.h"
#include "Texture.h"

Camera* camera = NULL;

GLuint Lighting, Lamp;
Model* model;

mat4 projection = GLM_MAT4_IDENTITY_INIT;
mat4 object = GLM_MAT4_IDENTITY_INIT;
mat4 view = GLM_MAT4_IDENTITY_INIT;

float lastX = 0.0f, lastY = 0.0f;
bool firstMouse = true, GL_POLY = false;

void keyboard_manager(Window* window)
{
	if (InputIsKeyDown(window->input, GLFW_KEY_ESCAPE))
		glfwSetWindowShouldClose(window->window, GLFW_TRUE);

	if (InputIsKeyDown(window->input, GLFW_KEY_W))
	{
		CameraKeyInput(camera, FORWARD, deltaTime);
	}

	if (InputIsKeyDown(window->input, GLFW_KEY_S))
	{
		CameraKeyInput(camera, BACKWARD, deltaTime);
	}

	if (InputIsKeyDown(window->input, GLFW_KEY_A))
	{
		CameraKeyInput(camera, LEFT, deltaTime);
	}

	if (InputIsKeyDown(window->input, GLFW_KEY_D))
	{
		CameraKeyInput(camera, RIGHT, deltaTime);
	}

	if (InputIsKeyDown(window->input, GLFW_KEY_SPACE))
	{
		CameraKeyInput(camera, UP, deltaTime);
	}

	if (InputIsKeyDown(window->input, GLFW_KEY_LEFT_CONTROL))
	{
		CameraKeyInput(camera, DOWN, deltaTime);
	}

	if (InputIsKeyPressed(window->input, GLFW_KEY_F2))
	{
		TextureMakeScreenshot(window);
	}

	if (InputIsKeyPressed(window->input, GLFW_KEY_F3) && !GL_POLY)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		GL_POLY = true;
	}
	else if (InputIsKeyPressed(window->input, GLFW_KEY_F3) && GL_POLY)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		GL_POLY = false;
	}

	printf(
		"\rDisplay[%d, %d], frames: %.0f, %.1fms\t", window->width, window->height, 1 / deltaTime, deltaTime * 1000
	);

}
void var_init(Window* window)
{
	Lighting = NewShader("assests/shaders/lighting.vs", "assests/shaders/lighting.fs");
	Lamp = NewShader("assests/shaders/lamp.vs", "assests/shaders/lamp.fs");

	model = NewModel3D();

	camera = NewCamera(
		(vec3) {
		0.0f, 0.0f, 3.0f
	},
		(vec3) {
		0.0f, 1.0f, 0.0f
	},
			YAW,
			PITCH,
			SENSITIVTY
			);

	glm_perspective(camera->zoom, (float)window->width / (float)window->height, -1.f, 100.0f, projection);

}

int main(void)
{
	InsightInit();
	Window* window = NewWindow(1280, 720, "OpenGL", false);
	var_init(window);

	vec3 cubePositions[] =
	{
		{0.0f, 0.0f, 0.0f},
		{2.0f, 5.0f, -15.0f},
		{-1.5f, -2.2f, -2.5f},
		{-3.8f, -2.0f, -12.3f},
		{2.4f, -0.4f, -3.5f},
		{-1.7f, 3.0f, -7.5f},
		{1.3f, -2.0f, -2.5f},
		{1.5f, 2.0f, -2.5f},
		{1.5f, 0.2f, -1.5f},
		{-1.3f, 1.0f, -1.5f}
	};


	while (WindowIsRunning(window))
	{
		WindowPollEvents(window);
		// Math:	
		glm_perspective(camera->zoom, (float)window->width / (float)window->height, 0.1f, 100.0f, projection);
		CameraGetViewMatrix(camera, view);

		// OpenGL:
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

		{
			// Binding the first shader
			glm_translate_make(object, (vec3) { 0.f, 0.f, 0.f });
			glm_rotate(object, glfwGetTime(), (vec3) { 1.0f, 0.5f, 0.1f });

			ShaderBind(Lighting);
			glUniformMatrix4fv(glGetUniformLocation(Lighting, "Proj"), 1, GL_FALSE, (float*)projection);
			glUniformMatrix4fv(glGetUniformLocation(Lighting, "View"), 1, GL_FALSE, (float*)view);
			glUniformMatrix4fv(glGetUniformLocation(Lighting, "Object"), 1, GL_FALSE, (float*)object);

			glUniform4f(glGetUniformLocation(Lighting, "ObjectColor"), 1.0f, 0.25f, 0.31f, 1.0f);

			ModelDraw(model, 36, GL_TRIANGLES);
			glUseProgram(0);


			glm_translate_make(object, (vec3) { 1.2f, 1.0f, 2.0f });
			glm_scale(object, (vec3) { .5f, .5f, .5f });

			// Binding the second shader
			ShaderBind(Lamp);

			glUniformMatrix4fv(glGetUniformLocation(Lamp, "Proj"), 1, GL_FALSE, (float*)projection);
			glUniformMatrix4fv(glGetUniformLocation(Lamp, "View"), 1, GL_FALSE, (float*)view);
			glUniformMatrix4fv(glGetUniformLocation(Lamp, "Object"), 1, GL_FALSE, (float*)object);

			ModelDraw(model, 36, GL_TRIANGLES);
			glUseProgram(0);
		}

		// API:
		keyboard_manager(window);
		glfwSwapInterval(0);
	}

	// deleting memory
	{
		WindowTerminate(window);
		free(camera);
	}

	return 0;
}
