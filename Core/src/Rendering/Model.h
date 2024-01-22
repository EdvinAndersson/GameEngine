#pragma once

#include "Mesh.h"
#include "Shader.h"
#include "Texture.h"

#include <stdio.h>

namespace CW {

	using ModelIndex = size_t;

	class Model {
		public:
			Model();
			void Draw(Shader *shader);
			void DrawInstanced(Shader *shader, int instance_count);

			void Load(const char *path);
			
			MeshIndex mesh_index;
			char *directory;
			std::vector<Texture> textures_loaded;
	};
}