#include "Mesh.h"
#include "../GLFunc.h"
#include <stddef.h>
#include <stdio.h>
#include <string.h>

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

        const char *name = textures[i].type;
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