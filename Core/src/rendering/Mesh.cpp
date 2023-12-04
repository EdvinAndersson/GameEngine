#include "Mesh.h"
#include "../GLFunc.h"
#include <stddef.h>
#include <stdio.h>
#include <string.h>

namespace GL {

    void BindMeshTextures(Mesh *mesh, Shader *shader);

    Mesh CreateMesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures) {
        Mesh mesh;
        mesh.vertices = vertices;
        mesh.indices = indices;
        mesh.textures = textures;

        glGenVertexArrays(1, &mesh.VAO);
        glGenBuffers(1, &mesh.VBO);
        glGenBuffers(1, &mesh.EBO);

        glBindVertexArray(mesh.VAO);
        glBindBuffer(GL_ARRAY_BUFFER, mesh.VBO);

        glBufferData(GL_ARRAY_BUFFER, mesh.vertices.size() * sizeof(Vertex), &mesh.vertices[0], GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh.EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh.indices.size() * sizeof(unsigned int), &mesh.indices[0], GL_STATIC_DRAW);

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

        return mesh;
    }
    void _MakeInstanced(Mesh *mesh) {
        glBindVertexArray(mesh->VAO);

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
    void DrawMesh(Mesh *mesh, Shader *shader) {
        BindMeshTextures(mesh, shader);

        glBindVertexArray(mesh->VAO);
        glDrawElements(GL_TRIANGLES, (GLsizei) mesh->indices.size(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }
    void DrawMeshInstanced(Mesh *mesh, Shader *shader, int instance_count) {
        BindMeshTextures(mesh, shader);

        glBindVertexArray(mesh->VAO);
        glDrawElementsInstanced(GL_TRIANGLES, (GLsizei)mesh->indices.size(), GL_UNSIGNED_INT, 0, instance_count);
        glBindVertexArray(0);
    }

    void BindMeshTextures(Mesh *mesh, Shader *shader) {
        unsigned int diffuseNr = 1;
        unsigned int specularNr = 1;
        unsigned int normalsNr = 1;

        for (unsigned int i = 0; i < mesh->textures.size(); i++)
        {
            char number[3] = { 0 };

            const char *name = mesh->textures[i].type;
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
            mesh->textures[i].Use(i);
        }
    }
}