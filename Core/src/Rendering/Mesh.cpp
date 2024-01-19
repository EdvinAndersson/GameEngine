#include "Mesh.h"
#include "../GLFunc.h"
#include <stddef.h>
#include <stdio.h>
#include <string.h>

namespace CW {

    Mesh::Mesh(Vertex *_vertices, unsigned int *_indices, Texture *_textures, unsigned int vertex_count, unsigned int index_count, unsigned int texture_count)
        :vertices(_vertices), indices(_indices), textures(_textures), vertex_count(vertex_count), index_count(index_count), texture_count(texture_count) {

        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);

        glBufferData(GL_ARRAY_BUFFER, vertex_count * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_count * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

        // vertex positions
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)0);
        // vertex texture coords
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, texCoords));
        // vertex normals
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, normal));
        // vertex tangent
        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void *)offsetof(Vertex, tangent));

        glBindVertexArray(0);
    }
    Mesh::~Mesh() {
        //delete[] vertices;
        //delete[] indices;
        //delete[] textures;
    }
    void Mesh::LoadMesh(const aiScene *scene) {
        ProcessNode(this, scene->mRootNode, scene);
    }
    void Mesh::ProcessNode(Mesh *m, aiNode *node, const aiScene *scene) {
		// process all the node's meshes (if any)
		for (unsigned int i = 0; i < node->mNumMeshes; i++)
		{
			struct aiMesh *ai_mesh = scene->mMeshes[node->mMeshes[i]];

			m->submeshes.push_back(CreateMesh(ai_mesh, scene));
		}
		// then do the same for each of its children
		for (unsigned int i = 0; i < node->mNumChildren; i++)
		{
			ProcessNode(m, node->mChildren[i], scene);
		}
	}
    Mesh *Mesh::CreateMesh(struct aiMesh *ai_mesh, const struct aiScene *scene) {
		Vertex *vertices = new Vertex[ai_mesh->mNumVertices];

		unsigned int index_count = 0;
		for (unsigned int i = 0; i < ai_mesh->mNumFaces; i++) {
			index_count += ai_mesh->mFaces[i].mNumIndices;
		}
		unsigned int* indices = new unsigned int[index_count];
		
		for (unsigned int i = 0; i < ai_mesh->mNumVertices; i++)
		{
			// process vertex positions, normals and texture coordinates
			Vertex vertex;

			Vec3 position;
			position.x = ai_mesh->mVertices[i].x;
			position.y = ai_mesh->mVertices[i].y;
			position.z = ai_mesh->mVertices[i].z;
			vertex.position = position;

			Vec3 normal;
			normal.x = ai_mesh->mNormals[i].x;
			normal.y = ai_mesh->mNormals[i].y;
			normal.z = ai_mesh->mNormals[i].z;
			vertex.normal = normal;

			if (ai_mesh->mTextureCoords[0]) { // does the mesh contain texture coordinates?
				Vec2 vec;
				vec.x = ai_mesh->mTextureCoords[0][i].x;
				vec.y = ai_mesh->mTextureCoords[0][i].y;
				vertex.texCoords = vec;
			}
			else {
				vertex.texCoords = Vec2{ 0.0f, 0.0f };
			}

			Vec3 tangent;
			tangent.x = ai_mesh->mTangents[i].x;
			tangent.y = ai_mesh->mTangents[i].y;
			tangent.z = ai_mesh->mTangents[i].z;
			vertex.tangent = tangent;

			Vec3 bitangent;
			bitangent.x = ai_mesh->mBitangents[i].x;
			bitangent.y = ai_mesh->mBitangents[i].y;
			bitangent.z = ai_mesh->mBitangents[i].z;
			vertex.bitangent = bitangent;
			
			vertices[i] = vertex;
		}

		// process indices
		unsigned int index = 0;
		for (unsigned int i = 0; i < ai_mesh->mNumFaces; i++) {
			struct aiFace face = ai_mesh->mFaces[i];
			for (unsigned int j = 0; j < face.mNumIndices; j++) {
				indices[index++] = face.mIndices[j];
			}
		}

        return new Mesh(vertices, indices, textures, vertex_count, index_count, texture_count);
	}
    void Mesh::MakeInstanced() {
        glBindVertexArray(VAO);

        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(Vec4), (void *)0);

        glEnableVertexAttribArray(4);
        glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(Vec4), (void *)(1 * sizeof(Vec4)));

        glEnableVertexAttribArray(5);
        glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(Vec4), (void *)(2 * sizeof(Vec4)));

        glEnableVertexAttribArray(6);
        glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(Vec4), (void *)(3 * sizeof(Vec4)));

        glVertexAttribDivisor(3, 1);
        glVertexAttribDivisor(4, 1);
        glVertexAttribDivisor(5, 1);
        glVertexAttribDivisor(6, 1);

        glBindVertexArray(0);
    }
    void Mesh::DrawMesh(Shader *shader) {
        BindMeshTextures(shader);

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, (GLsizei) index_count, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }
    void Mesh::DrawMeshInstanced(Shader *shader, int instance_count) {
        BindMeshTextures(shader);

        glBindVertexArray(VAO);
        glDrawElementsInstanced(GL_TRIANGLES, (GLsizei) index_count, GL_UNSIGNED_INT, 0, instance_count);
        glBindVertexArray(0);
    }

    void Mesh::BindMeshTextures(Shader *shader) {
        unsigned int diffuseNr = 1;
        unsigned int specularNr = 1;
        unsigned int normalsNr = 1;

        for (unsigned int i = 0; i < texture_count; i++)
        {
            char number[3] = { 0 };

            const char *name = "texture_diffuse";//textures[i].type;
            if (strcmp(name, "texture_diffuse"))
                sprintf(number, "%i", diffuseNr++);
            else if (strcmp(name, "texture_specular"))
                sprintf(number, "%i", specularNr++);
            else if (strcmp(name, "texture_normal"))
                sprintf(number, "%i", normalsNr++);

            char result[60] = {};
            strcat(result, "material.");
            strcat(result, name);
            strcat(result, number);

            shader->SetInt(result, i);
            textures[i].Use(i);
        }
    }
}