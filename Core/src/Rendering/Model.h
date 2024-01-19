#pragma once

#include "Mesh.h"
#include "Shader.h"
#include "Texture.h"

#include <stdio.h>

namespace CW {

	using ModelIndex = size_t;

	class Model {
		public:
			//void Draw(Shader *shader);
			//void DrawInstanced(Shader *shader, int instance_count);

			void Load(const char *path);
			void LoadFromMemory(const char *data, int data_size);
			
			Mesh mesh;
			char *directory;
			std::vector<Texture> textures_loaded;
	};
}