#pragma once
#include "../GLFunc.h"

namespace CW {

	enum Texture_Filter {
		TEXTURE_LINEAR_MIPMAP,
		TEXTURE_NEAREST_NEIGHBOR
	};

	struct Texture_Format {
		GLint color_format;
		Texture_Filter texture_filtering;
	};

	struct Texture {
		unsigned int id;

		void Use(int textureSlot);
	};

	using TextureIndex = size_t;

	struct TextureData {
		Texture texture;
		unsigned char *data;
		int width, height, nr_channels;

		const char *type;
		char path[256];
		Texture_Format texture_format;
	};

	TextureData* CreateTexture(const char *path);
	TextureData* CreateTexture(const char* path, Texture_Format texture_format);
	TextureData* CreateBlankTexture(Texture_Format texture_format);
	Texture CreateTextureFromData(unsigned char *data, int width, int height, Texture_Format texture_format);

	TextureData* GetTextureData(const char *path);
	void FreeTextureData(TextureData *texture_data);

	void _LoadFace(const char *face, unsigned int i);

	Texture CreateCubemapTexture(const char *right, const char *left, const char *top, const char *bottom, const char *front, const char *back);
}