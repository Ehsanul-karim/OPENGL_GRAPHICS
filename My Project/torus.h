#pragma once
//
//  torus.h
//  OpenGL Torus Object
//

#ifndef TORUS_H
#define TORUS_H

#include <glad/glad.h>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "shader.h"

class Torus {
public:
    // Material properties
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;

    // Texture properties
    unsigned int diffuseMap;
    unsigned int specularMap;
    float shininess;

    // Torus parameters
    float outerRadius;
    float innerRadius;
    int segmentCount;
    int tubeCount;

    // Constructor with default values
    Torus(float outer = 1.0f, float inner = 0.4f, unsigned int dMap=0, unsigned int sMap = 0,  int segments = 32, int tubes = 16,
        glm::vec3 amb = glm::vec3(0.2f), glm::vec3 diff = glm::vec3(0.5f),
        glm::vec3 spec = glm::vec3(1.0f), float shiny = 32.0f)
        : outerRadius(outer), innerRadius(inner), segmentCount(segments), tubeCount(tubes),
        ambient(amb), diffuse(diff), specular(spec), shininess(shiny),
        diffuseMap(dMap), specularMap(sMap) {
        buildCoordinatesAndIndices();
        buildVertices();
        setUpVertexData();
    }

    ~Torus() {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &EBO);
    }

    void drawWithMaterial(Shader& shader, glm::mat4 model = glm::mat4(1.0f)) {
        shader.use();
        shader.setVec3("material.ambient", ambient);
        shader.setVec3("material.diffuse", diffuse);
        shader.setVec3("material.specular", specular);
        shader.setFloat("material.shininess", shininess);
        shader.setMat4("model", model);

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indices.size()), GL_UNSIGNED_INT, 0);
    }

    void drawWithTexture(Shader& shader, glm::mat4 model = glm::mat4(1.0f)) {
        shader.use();

        shader.setInt("material.diffuse", 0);
        shader.setInt("material.specular", 1);
        shader.setFloat("material.shininess", shininess);
        shader.setMat4("model", model);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, diffuseMap);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, specularMap);

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(indices.size()), GL_UNSIGNED_INT, 0);
    }

private:
    unsigned int VAO, VBO, EBO;
    std::vector<float> coordinates;
    std::vector<float> normals;
    std::vector<float> texCoords;
    std::vector<unsigned int> indices;
    std::vector<float> vertices;

    void buildCoordinatesAndIndices() {
        float segmentStep = 2 * 3.1416 / segmentCount;
        float tubeStep = 2 * 3.1416 / tubeCount;

        for (int i = 0; i <= segmentCount; ++i) {
            float segmentAngle = i * segmentStep;
            float cosSegment = cos(segmentAngle);
            float sinSegment = sin(segmentAngle);

            for (int j = 0; j <= tubeCount; ++j) {
                float tubeAngle = j * tubeStep;
                float cosTube = cos(tubeAngle);
                float sinTube = sin(tubeAngle);

                float x = (outerRadius + innerRadius * cosTube) * cosSegment;
                float y = (outerRadius + innerRadius * cosTube) * sinSegment;
                float z = innerRadius * sinTube;

                coordinates.push_back(x);
                coordinates.push_back(y);
                coordinates.push_back(z);

                normals.push_back(cosTube * cosSegment);
                normals.push_back(cosTube * sinSegment);
                normals.push_back(sinTube);

                texCoords.push_back((float)i / segmentCount); // U
                texCoords.push_back((float)j / tubeCount);    // V
            }
        }

        for (int i = 0; i < segmentCount; ++i) {
            for (int j = 0; j < tubeCount; ++j) {
                int k1 = i * (tubeCount + 1) + j;
                int k2 = k1 + tubeCount + 1;

                indices.push_back(k1);
                indices.push_back(k2);
                indices.push_back(k1 + 1);

                indices.push_back(k2);
                indices.push_back(k2 + 1);
                indices.push_back(k1 + 1);
            }
        }
    }

    void buildVertices() {
        vertices.clear();
        for (size_t i = 0; i < coordinates.size() / 3; ++i) {
            vertices.push_back(coordinates[i * 3]);
            vertices.push_back(coordinates[i * 3 + 1]);
            vertices.push_back(coordinates[i * 3 + 2]);

            vertices.push_back(normals[i * 3]);
            vertices.push_back(normals[i * 3 + 1]);
            vertices.push_back(normals[i * 3 + 2]);

            vertices.push_back(texCoords[i * 2]);
            vertices.push_back(texCoords[i * 2 + 1]);
        }
    }

    void setUpVertexData() {
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

        // Position attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        // Normal attribute
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        // Texture coordinate attribute
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
        glEnableVertexAttribArray(2);

        glBindVertexArray(0);
    }
};

#endif /* TORUS_H */
