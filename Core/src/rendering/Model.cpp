#include "Model.h"
#include <stdio.h>
#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include "Texture.h"
#include "../GLFunc.h"

namespace GL {

	void GetSubStringByLastFoundChar(char *destStr, const char *str, char a);
	void ProcessNode(Model *model, struct aiNode *node, const struct aiScene *scene);
	Mesh ProcessMesh(Model *model, struct aiMesh *mesh, const struct aiScene *scene);
	std::vector<Texture> LoadMaterialTextures(Model *model, struct aiMaterial *mat, enum aiTextureType type, const char *typeName);

	void DrawModel(Model *model, GL::Shader *shader) {
		size_t meshes_size = model->meshes.size();
		for (unsigned int i = 0; i < meshes_size; i++) {
			DrawMesh(&model->meshes[i], shader);
		}
	}
	void DrawModelInstanced(Model *model, Shader *shader, int instance_count) {
		size_t meshes_size = model->meshes.size();
		for (unsigned int i = 0; i < meshes_size; i++) {
			DrawMeshInstanced(&model->meshes[i], shader, instance_count);
		}
	}
	void LoadModel(Model *model, const char *path) {
		const struct aiScene *scene = aiImportFile(path, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);

		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
			printf("ERROR::ASSIMP::%s", aiGetErrorString());
			return;
		}

		char directory[200] = { 0 };
		GetSubStringByLastFoundChar(directory, path, '/');
		model->directory = directory;

		ProcessNode(model, scene->mRootNode, scene);
	}
	void LoadModelFromMemory(Model *model, const char *data, int data_size) {
		const struct aiScene *scene = aiImportFileFromMemory(data, data_size, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace, 0);

		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
			printf("ERROR::ASSIMP::%s", aiGetErrorString());
			return;
		}

		ProcessNode(model, scene->mRootNode, scene);
	}
	void ProcessNode(Model *model, struct aiNode *node, const struct aiScene *scene)
	{
		// process all the node's meshes (if any)
		for (unsigned int i = 0; i < node->mNumMeshes; i++)
		{
			struct aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];

			model->meshes.push_back(ProcessMesh(model, mesh, scene));
		}
		// then do the same for each of its children
		for (unsigned int i = 0; i < node->mNumChildren; i++)
		{
			ProcessNode(model, node->mChildren[i], scene);
		}
	}
	Mesh ProcessMesh(Model *model, struct aiMesh *mesh, const struct aiScene *scene)
	{
		std::vector<Vertex> vertices;
		std::vector<unsigned int> indices;
		std::vector<Texture> textures;

		for (unsigned int i = 0; i < mesh->mNumVertices; i++)
		{
			// process vertex positions, normals and texture coordinates
			Vertex vertex;

			Vec3 position;
			position.x = mesh->mVertices[i].x;
			position.y = mesh->mVertices[i].y;
			position.z = mesh->mVertices[i].z;
			vertex.position = position;

			Vec3 normal;
			normal.x = mesh->mNormals[i].x;
			normal.y = mesh->mNormals[i].y;
			normal.z = mesh->mNormals[i].z;
			vertex.normal = normal;

			if (mesh->mTextureCoords[0]) { // does the mesh contain texture coordinates?
				Vec2 vec;
				vec.x = mesh->mTextureCoords[0][i].x;
				vec.y = mesh->mTextureCoords[0][i].y;
				vertex.texCoords = vec;
			}
			else {
				vertex.texCoords = Vec2{ 0.0f, 0.0f };
			}

			Vec3 tangent;
			tangent.x = mesh->mTangents[i].x;
			tangent.y = mesh->mTangents[i].y;
			tangent.z = mesh->mTangents[i].z;
			vertex.tangent = tangent;

			Vec3 bitangent;
			bitangent.x = mesh->mBitangents[i].x;
			bitangent.y = mesh->mBitangents[i].y;
			bitangent.z = mesh->mBitangents[i].z;
			vertex.bitangent = bitangent;
			
			vertices.push_back(vertex);
		}

		// process indices
		for (unsigned int i = 0; i < mesh->mNumFaces; i++) {
			struct aiFace face = mesh->mFaces[i];
			for (unsigned int j = 0; j < face.mNumIndices; j++) {
				indices.push_back(face.mIndices[j]);
			}
		}
		// process material
		if (mesh->mMaterialIndex >= 0)
		{
			struct aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
			
			std::vector<Texture> diffuseMaps = LoadMaterialTextures(model, material, aiTextureType_DIFFUSE, "texture_diffuse");

			std::vector<Texture> specularMaps = LoadMaterialTextures(model, material, aiTextureType_SPECULAR, "texture_specular");

			std::vector<Texture> normalMaps = LoadMaterialTextures(model, material, aiTextureType_NORMALS, "texture_normal");

			size_t diffuseMaps_size = diffuseMaps.size();
			for (unsigned int i = 0; i < diffuseMaps_size; i++) {
				textures.push_back(diffuseMaps[i]);
			}
			size_t specularMaps_size = specularMaps.size();
			for (unsigned int i = 0; i < specularMaps_size; i++) {
				textures.push_back(specularMaps[i]);
			}
			size_t normalMaps_size = normalMaps.size();
			for (unsigned int i = 0; i < normalMaps_size; i++) {
				textures.push_back(normalMaps[i]);
			}
		}
		return CreateMesh(vertices, indices, textures);
	}

	std::vector<Texture> LoadMaterialTextures(Model *model, struct aiMaterial *mat, enum aiTextureType type, const char *typeName) {

		std::vector<Texture> textures;
		size_t count = aiGetMaterialTextureCount(mat, type);

		for (unsigned int i = 0; i < count; i++) {
			struct aiString path;

			aiGetMaterialTexture(mat, type, i, &path, NULL, NULL, NULL, NULL, NULL, NULL);

			bool skip = false;
			size_t textures_loaded_size = model->textures_loaded.size();
			for (unsigned int j = 0; j < textures_loaded_size; j++) {
				if (std::strcmp(model->textures_loaded[j].path, path.data) == 0) {
					textures.push_back(model->textures_loaded[j]);
					skip = true;
					break;
				}
			}
			if (!skip) { // If texure haven't been loaded before
				char texPath[200] = { 0 };
				strcat(texPath, model->directory);
				strcat(texPath, "/");
				strcat(texPath, path.data);

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

				Texture texture = GL::CreateTexture(texPath, texture_format);
				texture.path = texPath;
				texture.type = typeName;

				textures.push_back(texture);
				model->textures_loaded.push_back(texture);
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