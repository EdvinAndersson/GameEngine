#pragma once
#include "Mesh.h"
#include "Shader.h"

namespace CW {

	typedef struct {
		std::vector<Mesh> meshes;
		char *directory;
		std::vector<Texture> textures_loaded;
	} Model;

	void DrawModel(Model *model, Shader *shader);
	void DrawModelInstanced(Model *model, Shader *shader, int instance_count);

	void LoadModel(Model *model, const char *path);
	void LoadModelFromMemory(Model *model, const char *data, int data_size);
}