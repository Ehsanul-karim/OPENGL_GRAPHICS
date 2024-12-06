//
//  cone.h
//  test
//
//  Created by Nazirul Hasan on 4/12/23.
//

#ifndef cone_h
#define cone_h

#include <glad/glad.h>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "shader.h"

# define PI 3.1416

using namespace std;

class Cone
{
public:
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    float shininess;

    // Constructor
    Cone(float radius = 1.0f, float height = 1.0f, int sectorCount = 18,
        glm::vec3 amb = glm::vec3(0.0f, 1.0f, 0.0f),
        glm::vec3 diff = glm::vec3(1.0f, 0.0f, 0.0f),
        glm::vec3 spec = glm::vec3(0.5f, 0.5f, 0.5f),
        float shiny = 32.0f) : verticesStride(24)
    {
        set(radius, height, sectorCount, amb, diff, spec, shiny);
        buildCoordinatesAndIndices();
        buildVertices();

        glGenVertexArrays(1, &coneVAO);
        glBindVertexArray(coneVAO);

        // Create VBO to copy vertex data
        unsigned int coneVBO;
        glGenBuffers(1, &coneVBO);
        glBindBuffer(GL_ARRAY_BUFFER, coneVBO);
        glBufferData(GL_ARRAY_BUFFER,
            this->getVertexSize(),
            this->getVertices(),
            GL_STATIC_DRAW);

        // Create EBO to copy index data
        unsigned int coneEBO;
        glGenBuffers(1, &coneEBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, coneEBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER,
            this->getIndexSize(),
            this->getIndices(),
            GL_STATIC_DRAW);

        // Activate and set attribute arrays
        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);

        int stride = this->getVerticesStride();
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)0);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void*)(sizeof(float) * 3));

        // Unbind VAO and buffers
        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

    ~Cone() {}

    void set(float radius, float height, int sectors, glm::vec3 amb, glm::vec3 diff, glm::vec3 spec, float shiny)
    {
        if (radius > 0) this->radius = radius;
        if (height > 0) this->height = height;
        this->sectorCount = max(sectors, 3); // Minimum of 3 sectors
        this->ambient = amb;
        this->diffuse = diff;
        this->specular = spec;
        this->shininess = shiny;
    }

    unsigned int getVertexCount() const { return (unsigned int)coordinates.size() / 3; }
    unsigned int getVertexSize() const { return (unsigned int)vertices.size() * sizeof(float); }
    int getVerticesStride() const { return verticesStride; }
    const float* getVertices() const { return vertices.data(); }

    unsigned int getIndexSize() const { return (unsigned int)indices.size() * sizeof(unsigned int); }
    const unsigned int* getIndices() const { return indices.data(); }
    unsigned int getIndexCount() const { return (unsigned int)indices.size(); }

    void drawCone(Shader& lightingShader, glm::mat4 model) const
    {
        lightingShader.use();
        lightingShader.setVec3("material.ambient", this->ambient);
        lightingShader.setVec3("material.diffuse", this->diffuse);
        lightingShader.setVec3("material.specular", this->specular);
        lightingShader.setFloat("material.shininess", this->shininess);
        lightingShader.setMat4("model", model);

        glBindVertexArray(coneVAO);
        glDrawElements(GL_TRIANGLES, this->getIndexCount(), GL_UNSIGNED_INT, (void*)0);
        glBindVertexArray(0);
    }

private:
    void buildCoordinatesAndIndices()
    {
        float x, z; // Vertex position
        float nx, ny, nz, lengthInv = 1.0f / radius; // Vertex normal
        float sectorStep = 2 * PI / sectorCount;

        // Base circle vertices and normals
        coordinates.push_back(0.0f); // Center of base
        coordinates.push_back(0.0f);
        coordinates.push_back(0.0f);
        normals.push_back(0.0f);
        normals.push_back(-1.0f);
        normals.push_back(0.0f);

        for (int i = 0; i < sectorCount; ++i)
        {
            float sectorAngle = i * sectorStep;
            x = radius * cosf(sectorAngle);
            z = radius * sinf(sectorAngle);
            coordinates.push_back(x);
            coordinates.push_back(0.0f);
            coordinates.push_back(z);
            normals.push_back(0.0f);
            normals.push_back(-1.0f);
            normals.push_back(0.0f);
        }

        // Apex vertex and normal
        coordinates.push_back(0.0f);
        coordinates.push_back(height);
        coordinates.push_back(0.0f);
        normals.push_back(0.0f);
        normals.push_back(1.0f);
        normals.push_back(0.0f);

        // Indices for base circle
        for (int i = 1; i < sectorCount; ++i)
        {
            indices.push_back(0);
            indices.push_back(i);
            indices.push_back(i + 1);
        }
        indices.push_back(0);
        indices.push_back(sectorCount);
        indices.push_back(1);

        // Indices for cone sides
        int apexIndex = sectorCount + 1;
        for (int i = 1; i < sectorCount; ++i)
        {
            indices.push_back(apexIndex);
            indices.push_back(i);
            indices.push_back(i + 1);
        }
        indices.push_back(apexIndex);
        indices.push_back(sectorCount);
        indices.push_back(1);
    }

    void buildVertices()
    {
        for (size_t i = 0; i < coordinates.size(); i += 3)
        {
            vertices.push_back(coordinates[i]);
            vertices.push_back(coordinates[i + 1]);
            vertices.push_back(coordinates[i + 2]);
            vertices.push_back(normals[i]);
            vertices.push_back(normals[i + 1]);
            vertices.push_back(normals[i + 2]);
        }
    }

    unsigned int coneVAO;
    float radius, height;
    int sectorCount;
    vector<float> vertices;
    vector<float> normals;
    vector<unsigned int> indices;
    vector<float> coordinates;
    int verticesStride; // 24 bytes
};

#endif /* cone_h */
