#include "Model.h"
#include "../GLFunc.h"
#include "assets/AssetManager.h"

#include "assimp/cimport.h"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

namespace CW {

	void GetSubStringByLastFoundChar(char *destStr, char *str, char a);

	Model::Model() {

	}

	void Model::Load(char *full_path, char *path) {
		ai_scene = aiImportFile(full_path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

		if (!ai_scene || ai_scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !ai_scene->mRootNode) {
			printf("ERROR::ASSIMP::%s\n", aiGetErrorString());
			return;
		}
		strcpy(this->path, path);
		GetSubStringByLastFoundChar(this->asset_path_dir, path, '/');
		GetSubStringByLastFoundChar(this->path_dir, full_path, '/');

		ProcessNode(ai_scene->mRootNode, ai_scene);
	}
	void Model::ProcessNode(aiNode *node, const aiScene *scene) {
		material_count = scene->mNumMaterials;

		for (int i = 0; i < scene->mNumMaterials; i++)
			material_indexes[i] = 0;

		// process all the node's meshes (if any)
		for (unsigned int i = 0; i < node->mNumMeshes; i++)
		{
			struct aiMesh *ai_mesh = scene->mMeshes[node->mMeshes[i]];

			ProcessMesh(ai_mesh, scene);
		}
		// then do the same for each of its children
		for (unsigned int i = 0; i < node->mNumChildren; i++)
		{
			ProcessNode(node->mChildren[i], scene);
		}
	}
	
	void Model::ProcessMesh(struct aiMesh *mesh, const struct aiScene *scene) {
		// process material
		unsigned int texture_count = 0;

		if (mesh->mMaterialIndex >= 0)
		{
			struct aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
			
			std::vector<TextureIndex> diffuseMaps = LoadMaterialTextures(material, aiTextureType_DIFFUSE, "texture_diffuse");

			std::vector<TextureIndex> specularMaps = LoadMaterialTextures(material, aiTextureType_SPECULAR, "texture_specular");

			std::vector<TextureIndex> normalMaps = LoadMaterialTextures(material, aiTextureType_NORMALS, "texture_normal");

			size_t diffuseMaps_size = diffuseMaps.size();
			size_t specularMaps_size = specularMaps.size();
			size_t normalMaps_size = normalMaps.size();
			texture_count = diffuseMaps_size + specularMaps_size + normalMaps_size;

			if (texture_count > 0) {
				if (material_indexes[mesh->mMaterialIndex - 1] == 0) { //Material has not been created
					Material mat = {};
					mat.albedo_color = vec3s{ 1.0, 1.0, 1.0 };
					mat.albedo = diffuseMaps[0];
					mat.normal_map = normalMaps[0];

					AssetManager::Get()->CreateAndLoadMaterialAsset((char *)material->GetName().C_Str(), mat);
					material_indexes[mesh->mMaterialIndex - 1] = AssetManager::Get()->GetMaterialIndex((char *)material->GetName().C_Str());
				} else {
					CW_ASSERT(false, "?");
					//material_indexes[mesh->mMaterialIndex - 1] = AssetManager::Get()->GetDefaultMaterialIndex();
				}
			} else {
				material_indexes[mesh->mMaterialIndex - 1] = AssetManager::Get()->GetDefaultMaterialIndex();
			}
			for (unsigned int i = 0; i < diffuseMaps_size; i++) {
				//material_indexes[i] = diffuseMaps[i];
			}
			for (unsigned int i = 0; i < specularMaps_size; i++) {
				//material_indexes[diffuseMaps_size + i] = specularMaps[i];
			}
			for (unsigned int i = 0; i < normalMaps_size; i++) {
				//material_indexes[diffuseMaps_size + specularMaps_size + i] = normalMaps[i];
			}
		}
	}
	
	std::vector<TextureIndex> Model::LoadMaterialTextures(struct aiMaterial *mat, enum aiTextureType type, const char *typeName) {

		std::vector<TextureIndex> materials;
		size_t count = aiGetMaterialTextureCount(mat, type);

		for (unsigned int i = 0; i < count; i++) {
			struct aiString path;

			aiGetMaterialTexture(mat, type, i, &path, NULL, NULL, NULL, NULL, NULL, NULL);
			bool skip = false;
			
			char tex_path[2000] = {0};
			strcpy(tex_path, asset_path_dir);
			strcat(tex_path, "/");
			strcat(tex_path, path.data);

			/*size_t textures_loaded_size = model->textures_loaded.size();
			for (unsigned int j = 0; j < textures_loaded_size; j++) {
				if (strcmp(model->textures_loaded[j].path, tex_path) == 0) {
					textures.push_back(model->textures_loaded[j]);
					skip = true;
					break;
				}
			}*/
			if (!skip) { // If texure haven't been loaded before
				Texture_Format texture_format;
				texture_format.texture_filtering = TEXTURE_LINEAR_MIPMAP;
				switch (type)
				{
					case aiTextureType_NORMALS:
						texture_format.color_format = GL_RGBA;
						break;
					default:
						texture_format.color_format = GL_SRGB_ALPHA;
						break;
				}

				CW::AssetManager::Get()->LoadTexture(tex_path);
				materials.push_back(AssetManager::Get()->GetTextureIndex(tex_path));

				/*Material material = {};
				material.albedo_color = vec3s{ 1.0, 1.0, 1.0 };
				material.albedo = AssetManager::Get()->GetTextureIndex(tex_path);

				char mat_path[256] = {};
				GetSubStringByLastFoundChar(mat_path, this->path, '.');
				strcat(mat_path, ".mat");

				AssetManager::Get()->CreateAndLoadMaterialAsset(mat_path, material);
				MaterialIndex i =  AssetManager::Get()->GetMaterialIndex(mat_path);
				materials.push_back(i);*/
				//model->textures_loaded.push_back(texture_data->texture);
			}

		}
		return materials;
	}

	void GetSubStringByLastFoundChar(char *destStr, char *str, char a) {
		int lastIndex = -1;
		size_t len = strlen(str);
		for (unsigned int i = 0; i < len; i++) {
			if (str[i] == a) {
				lastIndex = i;
			}
		}
		if (lastIndex == -1) lastIndex = (int) len;
		strncpy(destStr, str, lastIndex);
	}
}