#pragma once

#include "Window.h"
#include "Shaders.h"
#include "Model.h"
#include "Texture.h"
#include "Camera.h"

#include <glad/glad.h>
#include <cglm.h>

static Camera *Game_Camera = NULL;
static Texture *Game_CubemapTexture = NULL;
static Model *Game_Model = NULL;

static GLuint Game_Lighting, Game_Lamp, Game_SkyBox;
static mat4 Game_Projection = GLM_MAT4_IDENTITY_INIT, Game_Object = GLM_MAT4_IDENTITY_INIT, Game_View = GLM_MAT4_IDENTITY_INIT;


const vec3 Game_LightPos = { 1.2f, 1.0f, 2.0f };

void Game_MouseMgr(GLFWwindow *window, double xPos, double yPos)
{
	static float lastX = 0.0f, lastY = 0.0f;
	static int firstMouse = 1;

	if (firstMouse)
	{
		lastX = (float)xPos;
		lastY = (float)yPos;
		firstMouse = 0;
	}

	float xOffset = (float)xPos - lastX;
	float yOffset = lastY - (float)yPos;  // Reversed since y-coordinates go from bottom to left

	lastX = (float)xPos;
	lastY = (float)yPos;

	CameraMouseInput(Game_Camera, xOffset, yOffset, deltaTime, false);
}

void Game_KbMgr(Window *window)
{
	if (InputIsKeyDown(window->input, GLFW_KEY_ESCAPE))
	{
		glfwSetCursorPosCallback(window->window, NULL);
		glfwSetInputMode(window->window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

	} else if (InputIsKeyReleased(window->input, GLFW_KEY_ESCAPE))
	{
		glfwSetCursorPosCallback(window->window, Game_MouseMgr);
		glfwSetInputMode(window->window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	}

	if (InputIsKeyDown(window->input, GLFW_KEY_W))
	{
		CameraKeyInput(Game_Camera, FORWARD, deltaTime);
	}

	if (InputIsKeyDown(window->input, GLFW_KEY_S))
	{
		CameraKeyInput(Game_Camera, BACKWARD, deltaTime);
	}

	if (InputIsKeyDown(window->input, GLFW_KEY_A))
	{
		CameraKeyInput(Game_Camera, LEFT, deltaTime);
	}

	if (InputIsKeyDown(window->input, GLFW_KEY_D))
	{
		CameraKeyInput(Game_Camera, RIGHT, deltaTime);
	}

	if (InputIsKeyDown(window->input, GLFW_KEY_SPACE))
	{
		CameraKeyInput(Game_Camera, UP, deltaTime);
	}

	if (InputIsKeyDown(window->input, GLFW_KEY_LEFT_CONTROL))
	{
		CameraKeyInput(Game_Camera, DOWN, deltaTime);
	}

	if (InputIsKeyPressed(window->input, GLFW_KEY_F2))
	{
		TextureMakeScreenshot();
	}

}

void Game_Init(Window *window)
{
	WindowSetCursor(window, "assests/cursor.png");
	WindowSetIcon(window, "assests/icon.png");

	glfwSetCursorPosCallback(window->window, Game_MouseMgr);
	glfwSetInputMode(window->window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	Game_Lighting = NewShader("assests/shaders/lighting.vs", "assests/shaders/lighting.fs");
	Game_Lamp = NewShader("assests/shaders/lamp.vs", "assests/shaders/lamp.fs");
	Game_SkyBox = NewShader("assests/shaders/skybox.vs", "assests/shaders/skybox.fs");

	Game_Model = NewModel3D();

	Game_Camera = NewCamera((vec3) { 0.0f, 0.0f, 3.0f}, (vec3) { 0.0f, 1.0f, 0.0f }, YAW, PITCH, SENSITIVTY);

	char* faces[] = {
		"assests/skybox/cloudtop_ft.tga",
		"assests/skybox/cloudtop_bk.tga",
		"assests/skybox/cloudtop_up.tga", // UP
		"assests/skybox/cloudtop_dn.tga", // DOWN
		"assests/skybox/cloudtop_rt.tga",
		"assests/skybox/cloudtop_lf.tga"
	};

	Game_CubemapTexture = NewTextureSkyBox(faces, 6);
}

void Game_Terminate() 
{
	ShaderTerminate(Game_Lighting);
	ShaderTerminate(Game_Lamp);
	ShaderTerminate(Game_SkyBox);
	ModelTerminate(Game_Model);
	free(Game_Camera);
	TextureTerminate(Game_CubemapTexture);
}

void Game_Loop(Window *window)
{
	glm_perspective(Game_Camera->zoom, (float)window->width / (float)window->height, .1f, 1000.0f, Game_Projection);
	CameraGetViewMatrix(Game_Camera, Game_View);

	/// LIGHT SHADER
	glm_scale_make(Game_Object, (vec3) { .5f, .5f, .5f });

	ShaderBind(Game_Lighting)

	glUniform3f(glGetUniformLocation(Game_Lighting, "light.position"), Game_LightPos[0], Game_LightPos[1], Game_LightPos[2]);
	glUniform3f(glGetUniformLocation(Game_Lighting, "viewPos"), Game_Camera->position[0], Game_Camera->position[1], Game_Camera->position[2]);

	vec3 lightColor;
	lightColor[0] = (float)sin(glfwGetTime() * 2.0f);
	lightColor[1] = (float)sin(glfwGetTime() * 0.7f);
	lightColor[2] = (float)sin(glfwGetTime() * 1.3f);

	vec3 diffuseColor = { lightColor[0] * 0.5f, lightColor[1] * 0.5f, lightColor[2] * 0.5f };
	vec3 ambientColor = { diffuseColor[0] * 0.5f, diffuseColor[1] * 0.5f, diffuseColor[2] * 0.5f };
	glUniform3f(glGetUniformLocation(Game_Lighting, "light.diffuse"), diffuseColor[0], diffuseColor[1], diffuseColor[2]);
	glUniform3f(glGetUniformLocation(Game_Lighting, "light.ambient"), ambientColor[0], ambientColor[1], ambientColor[2]);
	glUniform3f(glGetUniformLocation(Game_Lighting, "light.specular"), 1.0f, 1.0f, 1.0f);

	// Set material properties
	glUniform3f(glGetUniformLocation(Game_Lighting, "material.ambient"), 1.0f, 0.5f, 0.31f);
	glUniform3f(glGetUniformLocation(Game_Lighting, "material.diffuse"), 1.0f, 0.5f, 0.31f);
	glUniform3f(glGetUniformLocation(Game_Lighting, "material.specular"), 0.5f, 0.5f, 0.5f); // Specular doesn't have full effect on this object's material
	glUniform1f(glGetUniformLocation(Game_Lighting, "material.shininess"), 32.0f);

	glUniformMatrix4fv(glGetUniformLocation(Game_Lighting, "Proj"), 1, GL_FALSE, (float*)Game_Projection);
	glUniformMatrix4fv(glGetUniformLocation(Game_Lighting, "View"), 1, GL_FALSE, (float*)Game_View);
	glUniformMatrix4fv(glGetUniformLocation(Game_Lighting, "Object"), 1, GL_FALSE, (float*)Game_Object);
	ModelDraw(Game_Model, 36, GL_TRIANGLES);
	glUseProgram(0);

	/// LAMP SHADER
	glm_translate_make(Game_Object, (vec3) { Game_LightPos[0], Game_LightPos[1], Game_LightPos[2] });
	glm_scale(Game_Object, (vec3) { .2f, .2f, .2f });

	ShaderBind(Game_Lamp);
	glUniformMatrix4fv(glGetUniformLocation(Game_Lamp, "Proj"), 1, GL_FALSE, (float*)Game_Projection);
	glUniformMatrix4fv(glGetUniformLocation(Game_Lamp, "View"), 1, GL_FALSE, (float*)Game_View);
	glUniformMatrix4fv(glGetUniformLocation(Game_Lamp, "Object"), 1, GL_FALSE, (float*)Game_Object);

	ModelDraw(Game_Model, 36, GL_TRIANGLES);
	glUseProgram(0);

	/// SKYBOX
	ShaderBind(Game_SkyBox);
	glDepthFunc(GL_LEQUAL);  // Change depth function so depth test passes when values are equal to depth buffer's content

	// Remove translation in the view Matrix for the skybox
	Game_View[3][0] = 0;
	Game_View[3][1] = 0;
	Game_View[3][2] = 0;

	glUniformMatrix4fv(glGetUniformLocation(Game_SkyBox, "view"), 1, GL_FALSE, (float*)Game_View);
	glUniformMatrix4fv(glGetUniformLocation(Game_SkyBox, "projection"), 1, GL_FALSE, (float*)Game_Projection);

	// skybox cube
	glBindTexture(GL_TEXTURE_CUBE_MAP, Game_CubemapTexture->texture);
	ModelDraw(Game_Model, 36, GL_TRIANGLES);
	glDepthFunc(GL_LESS); // Set depth function back to default

	glUseProgram(0);

	Game_KbMgr(window); 
}
