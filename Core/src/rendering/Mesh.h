#pragma once
#include "../GLMath.h"
#include "Texture.h"
#include "Shader.h"
#include <vector>

namespace CW {

	typedef struct {
		Vec3 position;
		Vec3 normal;
		Vec2 texCoords;
		Vec3 tangent;
		Vec3 bitangent;
	} Vertex;


	class Mesh {
		public:
			Mesh(Vertex *vertices, unsigned int *indices, Texture *textures, unsigned int vertex_count, unsigned int index_count, unsigned int texture_count);
			void DrawMesh(Shader *shader);
			void DrawMeshInstanced(Shader *shader, int instance_count);

			Vertex *vertices;
			unsigned int *indices;
			Texture *textures;

			unsigned int VAO, VBO, EBO;
			unsigned int vertex_count, index_count, texture_count;
		
		private:
			void MakeInstanced();
			void BindMeshTextures(Shader *shader);
	};

	typedef Mesh *Vec_Mesh;
}