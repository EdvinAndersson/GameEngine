#pragma once

#include "Mesh.h"
#include "Shader.h"
#include "Material.h"

#include <stdio.h>

namespace CW {

	#define MAX_MATERIALS 8
	using ModelIndex = size_t;

	class Model {
		public:
			Model();
			void Draw(Shader *shader);
			void DrawInstanced(Shader *shader, int instance_count);

			void Load(char *full_path, char *path);
			void ProcessNode(aiNode *node, const aiScene *scene);
			void ProcessMesh(struct aiMesh *mesh, const struct aiScene *scene);
			inline const aiScene* GetAiScene() { return ai_scene; }

			MeshIndex mesh_index;
			MaterialIndex material_indexes[MAX_MATERIALS];
			unsigned int material_count = 0;

		private:
			std::vector<TextureIndex> Model::LoadMaterialTextures(struct aiMaterial *mat, enum aiTextureType type, const char *typeName);

			char path[512] = {}, path_dir[512] = {}, asset_path_dir[512] = {};
			const aiScene *ai_scene;
	};
}