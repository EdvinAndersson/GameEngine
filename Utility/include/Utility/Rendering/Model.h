#pragma once

#include "Mesh.h"
#include "Shader.h"
#include "Texture.h"

#include <stdio.h>

class Model {
	public:
		void Draw(Shader *shader);
		void DrawInstanced(Shader *shader, int instance_count);

		void Load(const char *path);
		void LoadFromMemory(const char *data, int data_size);
		
		std::vector<Mesh> meshes;
		char *directory;
		std::vector<Texture> textures_loaded;
};