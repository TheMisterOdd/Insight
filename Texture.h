#ifndef _TEXTURE_H_
#define _TEXTURE_H_

#include <glad/glad.h>
#include <stdbool.h>
#include <time.h>

#include "Insight.h"

/*! Texture type. */
typedef uint32_t Insight_Texture[4];

/*! Returns a pointer to a texture object in memory. */
INSIGHT_API void insight_texture2d(Insight_Texture self, const char* path);

/*! Returns a pointer to a texture object in memory. */
INSIGHT_API void insight_texture2d_from_bytes(Insight_Texture self, unsigned char* raw_data, int width, int height);

/*! Returns a pointer to a texture object for a skybox in memory. */
INSIGHT_API void insight_skybox(Insight_Texture self, char** faces_files, unsigned int lenght);

/*! Uses the given texture, for handeling it. */
INSIGHT_API void insight_texture_bind(Insight_Texture self, unsigned int sampler);

/*! Stop using the current binded texture. */
INSIGHT_API void insight_texture_unbind();

/*! Copies the current texture into another */
INSIGHT_API void insight_texture_copy(Insight_Texture dest, Insight_Texture src);

/*! Delete the memory of the given texture. */
INSIGHT_API void insight_texture_finalize(Insight_Texture self);

/*! Makes a screenshot an writes it into a png file. */
INSIGHT_API int insight_screenshot();

/*! Sets window cursor */
INSIGHT_API GLFWcursor* insight_set_cursor(Insight_Window* self, const char* path);

/*! Sets window icon */
INSIGHT_API void insight_set_window_icon(Insight_Window* self, const char* path);


/*
 * ==============================================================
 *
 *                          IMPLEMENTATION
 *
 * ===============================================================
 */
#ifdef INSIGHT_TEXTURE_IMPL

#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION

#include <stb_image.h>
#include <stb_image_write.h>

INSIGHT_API void insight_texture2d(Insight_Texture self, const char* path)
{
#ifdef _DEBUG
	printf("Loading new texture '%s'...\n", path);
#endif

	glGenTextures(1, &self[3]);

	stbi_set_flip_vertically_on_load(1);
	unsigned char* img = stbi_load(path, (int*)&self[0], (int*)&self[1], (int*)&self[2], STBI_rgb_alpha);
	stbi_set_flip_vertically_on_load(0);
	
	if (!img)
	{
		fprintf(stderr, "[Error]: Cannot load given file '%s'\n", path);
		return;
	}

	glBindTexture(GL_TEXTURE_2D, self[3]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, (int)self[0], (int)self[1], 0, GL_RGBA, GL_UNSIGNED_BYTE, img);
	glGenerateMipmap(GL_TEXTURE_2D);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glBindTexture(GL_TEXTURE_2D, 0);
	stbi_image_free(img);
}

INSIGHT_API void insight_texture2d_from_bytes(Insight_Texture self, unsigned char* raw_data, int width, int height)
{
	self[0] = width;
	self[1] = height;
	glGenTextures(1, &self[3]);

	if (!raw_data)
	{
		fprintf(stderr, "[Error]: Cannot load given texture\n");
		return;
	}

	glBindTexture(GL_TEXTURE_2D, self[3]);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, (int)self[0], (int)self[1], 0, GL_RGBA, GL_UNSIGNED_BYTE, raw_data);
	glGenerateMipmap(GL_TEXTURE_2D);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

	glBindTexture(GL_TEXTURE_2D, 0);
}

INSIGHT_API void insight_skybox(Insight_Texture self, char** faces_files, unsigned int lenght)
{
	printf("Loading new skybox texture...\n");
	unsigned char* img = NULL;

	glGenTextures(1, &self[3]);
	glBindTexture(GL_TEXTURE_CUBE_MAP, self[3]);

	unsigned int i;
	for (i = 0; i < lenght; i++)
	{
		img = stbi_load(faces_files[i], (int*)&self[0], (int*)&self[1], (int*)&self[2], STBI_rgb);
		if (!img)
		{
			fprintf(stderr, "[Error]: Cannot load given file '%s'\n", faces_files[i]);
			return;
		}

		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, (int)self[0], (int)self[1], 0, GL_RGB, GL_UNSIGNED_BYTE, img);
		stbi_image_free(img);
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
	glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

INSIGHT_API void insight_texture_bind(Insight_Texture self, unsigned int sampler)
{
	glActiveTexture(GL_TEXTURE0 + sampler);
	glBindTexture(GL_TEXTURE_2D, self[3]);
}

INSIGHT_API void insight_texture_unbind()
{
	glBindTexture(GL_TEXTURE_2D, 0);
}

INSIGHT_API void insight_texture_copy(Insight_Texture dest, Insight_Texture src)
{
	dest[0] = src[0];
	dest[1] = src[1];
	dest[2] = src[2];
	dest[3] = src[3];
}

INSIGHT_API void insight_texture_finalize(Insight_Texture self)
{
	glDeleteTextures(1, &self[3]);
}

INSIGHT_API int insight_screenshot()
{
	time_t t = time(NULL);
	struct tm tm;
	localtime_s(&tm, &t);

	int viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);

	int x = viewport[0];
	int y = viewport[1];
	int width = viewport[2];
	int height = viewport[3];

	unsigned char* data = (unsigned char*)malloc((size_t)(width * height * 3)); /* 3 components (R, G, B) */

	if (!data)
		return 0;

	glPixelStorei(GL_PACK_ALIGNMENT, 1);
	glReadPixels(x, y, width, height, GL_RGB, GL_UNSIGNED_BYTE, data);

	char* _Buffer = (char*)malloc(sizeof(char) * 512);
	sprintf_s(_Buffer, 512, "%d-%d-%d_%d.%d.%02d.png", tm.tm_year + 1900, tm.tm_mon + 1, tm.tm_mday, tm.tm_hour, tm.tm_min, tm.tm_sec);

	stbi_flip_vertically_on_write(1);
	int saved = stbi_write_png(_Buffer, width, height, 3, data, 0);
	stbi_flip_vertically_on_write(0);

	free(data);
	free(_Buffer);

	return saved;
}

INSIGHT_API GLFWcursor* insight_set_cursor(Insight_Window* self, const char* path)
{
	GLFWimage* image = (GLFWimage*)malloc(sizeof(GLFWimage));
	image[0].pixels = stbi_load(path, &image[0].width, &image[0].height, NULL, STBI_rgb_alpha);
	GLFWcursor* cursor = glfwCreateCursor(image, 0, 0);

	glfwSetCursor(self->wnd_hndl, cursor);
	free(image);

	return cursor;
}

INSIGHT_API void insight_set_window_icon(Insight_Window* self, const char* path)
{
	GLFWimage* image = (GLFWimage*)malloc(sizeof(GLFWimage));
	image[0].pixels = stbi_load(path, &image[0].width, &image[0].height, NULL, STBI_rgb_alpha);
	glfwSetWindowIcon(self->wnd_hndl, 1, image);

	free(image);
}



#endif /* INSIGHT_TEXTURE_IMPL */

#endif /* !_TEXTURE_H_ */


