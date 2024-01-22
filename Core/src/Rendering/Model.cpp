#include "Model.h"
#include "../GLFunc.h"

#include "assimp/cimport.h"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

namespace CW {

	void GetSubStringByLastFoundChar(char *destStr, const char *str, char a);
	std::vector<Texture> LoadMaterialTextures(Model *model, struct aiMaterial *mat, enum aiTextureType type, const char *typeName);

	Model::Model() {

	}

	void Model::Load(const char *path) {
		const aiScene *scene = aiImportFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
			printf("ERROR::ASSIMP::%s", aiGetErrorString());
			return;
		}

		char directory[200] = { 0 };
		GetSubStringByLastFoundChar(directory, path, '/');
		this->directory = directory;

		//TODO: Load mesh
		//Mesh *mesh = new Mesh();
		//mesh->LoadMesh(scene);
	}
	/*
	Mesh ProcessMesh(Model *model, struct aiMesh *mesh, const struct aiScene *scene)
	{
		// process material
		unsigned int texture_count = 0;

		if (mesh->mMaterialIndex >= 0)
		{
			struct aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
			
			std::vector<Texture> diffuseMaps = LoadMaterialTextures(model, material, aiTextureType_DIFFUSE, "texture_diffuse");

			std::vector<Texture> specularMaps = LoadMaterialTextures(model, material, aiTextureType_SPECULAR, "texture_specular");

			std::vector<Texture> normalMaps = LoadMaterialTextures(model, material, aiTextureType_NORMALS, "texture_normal");

			size_t diffuseMaps_size = diffuseMaps.size();
			size_t specularMaps_size = specularMaps.size();
			size_t normalMaps_size = normalMaps.size();
			texture_count = diffuseMaps_size + specularMaps_size + normalMaps_size;
			textures = new Texture[texture_count];

			for (unsigned int i = 0; i < diffuseMaps_size; i++) {
				textures[i] = diffuseMaps[i];
			}
			for (unsigned int i = 0; i < specularMaps_size; i++) {
				textures[diffuseMaps_size + i] = specularMaps[i];
			}
			for (unsigned int i = 0; i < normalMaps_size; i++) {
				textures[diffuseMaps_size + specularMaps_size + i] = normalMaps[i];
			}
		}
	}
	*/
	std::vector<Texture> LoadMaterialTextures(Model *model, struct aiMaterial *mat, enum aiTextureType type, const char *typeName) {

		std::vector<Texture> textures;
		size_t count = aiGetMaterialTextureCount(mat, type);

		for (unsigned int i = 0; i < count; i++) {
			struct aiString path;

			aiGetMaterialTexture(mat, type, i, &path, NULL, NULL, NULL, NULL, NULL, NULL);
			bool skip = false;
			
			char tex_path[2000] = {0};
			strcpy(tex_path, model->directory);
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

				TextureData *texture_data = CreateTexture(tex_path, texture_format);
				strcpy(texture_data->path, tex_path);
				texture_data->type = typeName;
				textures.push_back(texture_data->texture);
				model->textures_loaded.push_back(texture_data->texture);
			}

		}
		return textures;
	}

	void GetSubStringByLastFoundChar(char *destStr, const char *str, char a) {
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