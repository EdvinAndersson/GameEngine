#pragma once
#include "../GLFunc.h"
#include "cglm/struct.h"

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
		char path[256], asset_path_dir[256];
		Texture_Format texture_format;
	};

	TextureData* CreateTexture(const char *path, char *asset_path_dir);
	TextureData* CreateTexture(const char* path, char *asset_path_dir, Texture_Format texture_format);
	TextureData* CreateBlankTexture(Texture_Format texture_format, unsigned int color);
	Texture CreateTextureFromData(unsigned char *data, int width, int height, Texture_Format texture_format);

	TextureData* GetTextureData(const char *path);
	void FreeTextureData(TextureData *texture_data);

	void _LoadFace(const char *face, unsigned int i);

	TextureData* CreateCubemapTexture(TextureIndex right, TextureIndex left, TextureIndex top, TextureIndex bottom, TextureIndex front, TextureIndex back);
	Texture CreateCubemapTextureOld(const char *right, const char *left, const char *top, const char *bottom, const char *front, const char *back);
}