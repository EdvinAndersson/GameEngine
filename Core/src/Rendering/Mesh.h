#pragma once
#include "cglm/struct.h"
#include "Texture.h"
#include "Shader.h"
#include <vector>

#include "assimp/cimport.h"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

namespace CW {

	typedef struct {
		vec3s position;
		vec3s normal;
		vec2s texCoords;
		vec3s tangent;
		vec3s bitangent;
	} Vertex;

	using MeshIndex = size_t;

	class Mesh {
		public:
			Mesh();
			~Mesh();

			void DrawMesh(Shader *shader);
			void DrawMeshInstanced(Shader *shader, int instance_count);

			void Load(char *path);
			void LoadMesh(const aiScene *scene);
			void LoadMeshFromData(const char *data, int data_size);

			void ProcessNode(Mesh *mesh, aiNode *node, const aiScene *scene);
			Mesh* CreateSubMesh(struct aiMesh *ai_mesh);
			void SubmitMeshData(Vertex *vertices, unsigned int *indices, unsigned int vertex_count, unsigned int index_count);

			Vertex *vertices = 0;
			unsigned int *indices = 0;
			Texture *textures = 0;

			unsigned int VAO = 0, VBO = 0, EBO = 0;
			unsigned int vertex_count = 0, index_count = 0, texture_count = 0;

			std::vector<Mesh*> submeshes;
		private:
			void DrawSubmesh(Shader *shader);
			void MakeInstanced();
			void BindMeshTextures(Shader *shader);
	};

	typedef Mesh *Vec_Mesh;
}