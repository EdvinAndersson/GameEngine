#include "Mesh.h"
#include "../GLFunc.h"
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include "assets/AssetManager.h"

namespace CW {

    Mesh::Mesh() {
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);

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
        delete[] vertices;
        delete[] indices;

        for (Mesh *m : submeshes) {
            delete m;
        }
        submeshes.clear();
    }
    void Mesh::Load(const aiScene *scene) {
		ProcessNode(this, scene->mRootNode, scene);
    }
    void Mesh::LoadMeshFromData(const char *data, int data_size) {
        const struct aiScene *scene = aiImportFileFromMemory(data, data_size, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace, 0);

		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
			printf("ERROR::ASSIMP::%s\n", aiGetErrorString());
			return;
		}

        ProcessNode(this, scene->mRootNode, scene);
    }
    void Mesh::ProcessNode(Mesh *m, aiNode *node, const aiScene *scene) {
		for (unsigned int i = 0; i < node->mNumMeshes; i++)
		{
			struct aiMesh *ai_mesh = scene->mMeshes[node->mMeshes[i]];
			m->submeshes.push_back(CreateSubMesh(ai_mesh));
		}

		for (unsigned int i = 0; i < node->mNumChildren; i++)
			ProcessNode(m, node->mChildren[i], scene);
	}
    Mesh *Mesh::CreateSubMesh(struct aiMesh *ai_mesh) {
		Vertex *sub_vertices = new Vertex[ai_mesh->mNumVertices];

		unsigned int sub_index_count = 0;
		for (unsigned int i = 0; i < ai_mesh->mNumFaces; i++) {
			sub_index_count += ai_mesh->mFaces[i].mNumIndices;
		}
		unsigned int* sub_indices = new unsigned int[sub_index_count];
		
		for (unsigned int i = 0; i < ai_mesh->mNumVertices; i++)
		{
			Vertex vertex;
            
			vec3s position;
			position.x = ai_mesh->mVertices[i].x;
			position.y = ai_mesh->mVertices[i].y;
			position.z = ai_mesh->mVertices[i].z;
			vertex.position = position;

			vec3s normal;
			normal.x = ai_mesh->mNormals[i].x;
			normal.y = ai_mesh->mNormals[i].y;
			normal.z = ai_mesh->mNormals[i].z;
			vertex.normal = normal;

			if (ai_mesh->mTextureCoords[0]) { // does the mesh contain texture coordinates?
				vec2s vec;
				vec.x = ai_mesh->mTextureCoords[0][i].x;
				vec.y = ai_mesh->mTextureCoords[0][i].y;
				vertex.texCoords = vec;
			}
			else {
				vertex.texCoords = vec2s { 0.0f, 0.0f };
			}

			vec3s tangent;
			tangent.x = ai_mesh->mTangents[i].x;
			tangent.y = ai_mesh->mTangents[i].y;
			tangent.z = ai_mesh->mTangents[i].z;
			vertex.tangent = tangent;
			
			sub_vertices[i] = vertex;
		}

		// process indices
		unsigned int index = 0;
		for (unsigned int i = 0; i < ai_mesh->mNumFaces; i++) {
			struct aiFace face = ai_mesh->mFaces[i];
			for (unsigned int j = 0; j < face.mNumIndices; j++) {
				sub_indices[index++] = face.mIndices[j];
			}
		}

        Mesh *submesh = new Mesh();
        submesh->SubmitMeshData(sub_vertices, sub_indices, ai_mesh->mNumVertices, sub_index_count);
        submesh->material_used = ai_mesh->mMaterialIndex == 0 ? 0 : ai_mesh->mMaterialIndex - 1;
        return submesh;
	}

    void Mesh::SubmitMeshData(Vertex *vertices, unsigned int *indices, unsigned int vertex_count, unsigned int index_count) {
        this->vertices = vertices;
        this->indices = indices;
        this->vertex_count = vertex_count;
        this->index_count = index_count;

        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);

        glBufferData(GL_ARRAY_BUFFER, vertex_count * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, index_count * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

        glBindVertexArray(0);
    }

    void Mesh::MakeInstanced() {
        glBindVertexArray(VAO);

        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(vec4s), (void *)0);

        glEnableVertexAttribArray(4);
        glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(vec4s), (void *)(1 * sizeof(vec4s)));

        glEnableVertexAttribArray(5);
        glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(vec4s), (void *)(2 * sizeof(vec4s)));

        glEnableVertexAttribArray(6);
        glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(vec4s), (void *)(3 * sizeof(vec4s)));

        glVertexAttribDivisor(3, 1);
        glVertexAttribDivisor(4, 1);
        glVertexAttribDivisor(5, 1);
        glVertexAttribDivisor(6, 1);

        glBindVertexArray(0);
    }
    void Mesh::DrawMesh(Shader *shader) {
        for (Mesh* submesh : submeshes) {
            submesh->DrawSubmesh(shader);
        }
    }
    void Mesh::DrawMesh(Shader *shader, MaterialIndex* material_indexes, int material_count) {
        for (Mesh* submesh : submeshes) {
            MaterialIndex material_index = material_indexes[submesh->material_used];
            if (submesh->material_used >= material_count)
                material_index = AssetManager::Get()->GetDefaultMaterialIndex();
            Material* mat = AssetManager::Get()->GetMaterial(material_index);

            submesh->DrawSubmesh(shader, mat);
        }
    }
    void Mesh::DrawSubmesh(Shader *shader, Material *material) {
        shader->SetV4("objectColor", material->albedo_color.r, material->albedo_color.g, material->albedo_color.b, 1.0f);

        shader->SetInt("material.texture_diffuse1", 0);
        Texture albedo = AssetManager::Get()->GetTexture(material->albedo);
        albedo.Use(0);

        if(material->normal_map != 0) {
            shader->SetInt("material.texture_normal1", 1);
            Texture normal = AssetManager::Get()->GetTexture(material->normal_map);
            normal.Use(1);
        }
        if(material->specular_map != 0) {
            shader->SetInt("material.texture_specular1", 2);
            Texture specular = AssetManager::Get()->GetTexture(material->specular_map);
            specular.Use(2);
        }

        DrawSubmesh(shader);
    }
    void Mesh::DrawSubmesh(Shader *shader) {
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, (GLsizei) index_count, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }
    void Mesh::DrawMeshInstanced(Shader *shader, int instance_count) {
        //BindMeshTextures(shader);

        glBindVertexArray(VAO);
        glDrawElementsInstanced(GL_TRIANGLES, (GLsizei) index_count, GL_UNSIGNED_INT, 0, instance_count);
        glBindVertexArray(0);
    }
}