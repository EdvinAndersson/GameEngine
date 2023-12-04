#pragma once
#include "../GLMath.h"
#include "Texture.h"
#include "Shader.h"
#include <vector>

namespace GL {

	typedef struct {
		Vec3 position;
		Vec3 normal;
		Vec2 texCoords;
		Vec3 tangent;
		Vec3 bitangent;
	} Vertex;

	typedef struct {
		std::vector<Vertex> vertices;
		std::vector<unsigned int> indices;
		std::vector<Texture> textures;

		unsigned int VAO, VBO, EBO;
	} Mesh;

	typedef Mesh *Vec_Mesh;

	Mesh CreateMesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);
	void _MakeInstanced(Mesh *mesh);
	void DrawMesh(Mesh *mesh, Shader *shader);
	void DrawMeshInstanced(Mesh *mesh, Shader *shader, int instance_count);
}