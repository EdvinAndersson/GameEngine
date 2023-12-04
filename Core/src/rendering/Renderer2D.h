#pragma once

#include "../Window.h"
#include "../GLFunc.h"
#include "../GLMath.h"
#include "Shader.h"
#include "Texture.h"

namespace GL {

	struct R2D_Texture {
		unsigned int handle;
		float use_bilinear = 0;
		int width = 0, height = 0;
	};

	void R2D_Init(Window *window);
	void R2D_Clear(Vec4 color);

	void R2D_RenderRect(Vec2 position, Vec2 size, Vec4 color);
	void R2D_RenderRect(Vec2 position, Vec2 size, Vec4 color, float rotation, Vec2 pivot);

	void R2D_RenderSprite(Vec2 position, Vec2 size, R2D_Texture texture);
	void R2D_RenderSprite(Vec2 position, Vec2 size, R2D_Texture texture, Vec4 color);
	void R2D_RenderSprite(Vec2 position, Vec2 size, R2D_Texture texture, Vec4 color, float rotation, Vec2 pivot);

	void R2D_Flush();
	void R2D__BeginBatch();
	void R2D__EndBatch();

	void R2D__CreateTexture3D();
	R2D_Texture R2D_AddTexture(const char *path, GL::Texture_Filter texture_filter);

	void R2D_UseShader(Shader *shader);

	void R2D__ResizeCallback(int screen_width, int screen_height);
}