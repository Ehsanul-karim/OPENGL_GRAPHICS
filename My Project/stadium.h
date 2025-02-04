#ifndef stadium_h
#define stadium_h

#include <glad/glad.h>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "shader.h"

# define PI 3.1416

using namespace std;

class Stadium
{
public:
    glm::vec3 ambient;
    glm::vec3 diffuse;
    glm::vec3 specular;
    float shininess;

    Stadium(float xlen = 1.0f, float ylen = 1.0f, float zlen = 1.0f,
        glm::vec3 amb = glm::vec3(1.0f, 0.0f, 0.0f),
        glm::vec3 diff = glm::vec3(1.0f, 0.0f, 0.0f),
        glm::vec3 spec = glm::vec3(0.5f, 0.5f, 0.5f),
        float shiny = 32.0f)
    {
        set(xlen, ylen, zlen, amb, diff, spec, shiny);
        buildCoordinatesAndIndices();

        glGenVertexArrays(1, &stadiumVAO);
        glBindVertexArray(stadiumVAO);

        unsigned int stadiumVBO;
        glGenBuffers(1, &stadiumVBO);
        glBindBuffer(GL_ARRAY_BUFFER, stadiumVBO);
        glBufferData(GL_ARRAY_BUFFER, this->getVertexSize(), this->getVertices(), GL_STATIC_DRAW);

        unsigned int stadiumEBO;
        glGenBuffers(1, &stadiumEBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, stadiumEBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->getIndexSize(), this->getIndices(), GL_STATIC_DRAW);

        int stride = this->getVerticesStride();
        glVertexAttribPointer(0, 3, GL_FLOAT, false, stride, (void*)(sizeof(float) * 0));
        glVertexAttribPointer(1, 3, GL_FLOAT, false, stride, (void*)(sizeof(float) * 3));
        glVertexAttribPointer(2, 2, GL_FLOAT, false, stride, (void*)(sizeof(float) * 6));

        glEnableVertexAttribArray(0);
        glEnableVertexAttribArray(1);
        glEnableVertexAttribArray(2);

        glBindVertexArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }
    ~Stadium() {}

    // getters/setters

    void set(float xlen, float ylen, float zlen,
        glm::vec3 amb, glm::vec3 diff, glm::vec3 spec, float shiny)
    {
        this->xlen = xlen;
        this->ylen = ylen;
        this->zlen = zlen;

        this->ambient = amb;
        this->diffuse = diff;
        this->specular = spec;
        this->shininess = shiny;
    }

    // for interleaved vertices
    unsigned int getVertexCount() const
    {
        return (unsigned int)coordinates.size() / 3;     // # of vertices
    }

    unsigned int getVertexSize() const
    {
        return (unsigned int)vertices.size() * sizeof(float);  // # of bytes
    }

    int getVerticesStride() const
    {
        return 8 * sizeof(float);
    }
    const float* getVertices() const
    {
        return vertices.data();
    }

    unsigned int getIndexSize() const
    {
        return (unsigned int)indices.size() * sizeof(unsigned int);
    }

    const unsigned int* getIndices() const
    {
        return indices.data();
    }

    unsigned int getIndexCount() const
    {
        return (unsigned int)indices.size();
    }

    // draw in VertexArray mode
    void drawStadium(Shader& lightingShader, glm::mat4 model) const      // draw surface
    {
        lightingShader.use();

        lightingShader.setVec3("material.ambient", this->ambient);
        lightingShader.setVec3("material.diffuse", this->diffuse);
        lightingShader.setVec3("material.specular", this->specular);
        lightingShader.setFloat("material.shininess", this->shininess);

        // lightingShader.setMat4("model", model);

        // draw a sphere with VAO
        glBindVertexArray(stadiumVAO);
        glDrawElements(GL_TRIANGLES,                    // primitive type
            this->getIndexCount(),          // # of indices
            GL_UNSIGNED_INT,                 // data type
            (void*)0);                       // offset to indices

        // unbind VAO
        glBindVertexArray(0);
    }

private:
    // member functions

    void updateArrays(float x, float y, float z, float nx, float ny, float nz, float u = 0.0f, float v = 0.0f)
    {
        //coordinates.push_back(x), coordinates.push_back(y), coordinates.push_back(z);
        //normals.push_back(nx), normals.push_back(ny), normals.push_back(nz);
        //textureCoordinates.push_back(u), textureCoordinates.push_back(v);

        vertices.push_back(x), vertices.push_back(y), vertices.push_back(z);
        vertices.push_back(nx), vertices.push_back(ny), vertices.push_back(nz);
        vertices.push_back(u), vertices.push_back(v);
    }

    void buildCoordinatesAndIndices()
    {
        float xstep = 0.01;
        int xcounts = xlen / xstep;

        // rectangular part

        // front-top
        for (int xcount = 0; xcount <= xcounts; ++xcount)
        {
            float x = -(xlen / 2) + xcount * xstep;
            float y = (ylen / 2);
            float z = (zlen / 2);

            float u = (ylen / 2 + xcount * xstep) / (xlen + ylen);
            float v = 1.0f;

            updateArrays(x, y, z, 0.0f, 0.0f, 1.0f, u, v);
        }
        // front-bottom
        for (int xcount = 0; xcount <= xcounts; ++xcount)
        {
            float x = -(xlen / 2) + xcount * xstep;
            float y = -(ylen / 2);
            float z = (zlen / 2);

            float u = (ylen / 2 + xcount * xstep) / (xlen + ylen);
            float v = 0.0f;

            updateArrays(x, y, z, 0.0f, 0.0f, 1.0f, u, v);

            
        }
        // front
        int k1 = 0 * (xcounts + 1), k2 = 1 * (xcounts + 1);
        for (int xcount = 0; xcount < xcounts; ++xcount, ++k1, ++k2)
        {
            indices.push_back(k1), indices.push_back(k2), indices.push_back(k2 + 1);
            indices.push_back(k1), indices.push_back(k2 + 1), indices.push_back(k1 + 1);
        }

        // back-top
        for (int xcount = 0; xcount <= xcounts; ++xcount)
        {
            float x = -(xlen / 2) + xcount * xstep;
            float y = (ylen / 2);
            float z = -(zlen / 2);

            float u = (ylen / 2 + xcount * xstep) / (xlen + ylen);
            float v = 1.0f;

            updateArrays(x, y, z, 0.0f, 0.0f, -1.0f, u, v);
        }
        // back-bottom
        for (int xcount = 0; xcount <= xcounts; ++xcount)
        {
            float x = -(xlen / 2) + xcount * xstep;
            float y = -(ylen / 2);
            float z = -(zlen / 2);

            float u = (ylen / 2 + xcount * xstep) / (xlen + ylen);
            float v = 0.0f;

            updateArrays(x, y, z, 0.0f, 0.0f, -1.0f, u, v);
        }
        // back
        k1 = 2 * (xcounts + 1), k2 = 3 * (xcounts + 1);
        for (int xcount = 0; xcount < xcounts; ++xcount, ++k1, ++k2)
        {
            indices.push_back(k1), indices.push_back(k2), indices.push_back(k2 + 1);
            indices.push_back(k1), indices.push_back(k2 + 1), indices.push_back(k1 + 1);
        }

        // top-back
        for (int xcount = 0; xcount <= xcounts; ++xcount)
        {
            float x = -(xlen / 2) + xcount * xstep;
            float y = (ylen / 2);
            float z = -(zlen / 2);

            float u = (ylen / 2 + xcount * xstep) / (xlen + ylen);
            float v = 1.0f;

            updateArrays(x, y, z, 0.0f, 1.0f, 0.0f, u, v);
        }
        // top-front
        for (int xcount = 0; xcount <= xcounts; ++xcount)
        {
            float x = -(xlen / 2) + xcount * xstep;
            float y = (ylen / 2);
            float z = (zlen / 2);

            float u = (ylen / 2 + xcount * xstep) / (xlen + ylen);
            float v = 1.0f;

            updateArrays(x, y, z, 0.0f, 1.0f, 0.0f, u, v);
        }
        // top
        k1 = 4 * (xcounts + 1), k2 = 5 * (xcounts + 1);
        for (int xcount = 0; xcount < xcounts; ++xcount, ++k1, ++k2)
        {
            indices.push_back(k1), indices.push_back(k2), indices.push_back(k2 + 1);
            indices.push_back(k1), indices.push_back(k2 + 1), indices.push_back(k1 + 1);
        }

        // bottom-back
        for (int xcount = 0; xcount <= xcounts; ++xcount)
        {
            float x = -(xlen / 2) + xcount * xstep;
            float y = -(ylen / 2);
            float z = -(zlen / 2);

            float u = (ylen / 2 + xcount * xstep) / (xlen + ylen);
            float v = 1.0f;

            updateArrays(x, y, z, 0.0f, -1.0f, 0.0f, u, v);
        }
        // bottom-front
        for (int xcount = 0; xcount <= xcounts; ++xcount)
        {
            float x = -(xlen / 2) + xcount * xstep;
            float y = -(ylen / 2);
            float z = (zlen / 2);

            float u = (ylen / 2 + xcount * xstep) / (xlen + ylen);
            float v = 1.0f;

            updateArrays(x, y, z, 0.0f, -1.0f, 0.0f, u, v);
        }
        // bottom
        k1 = 6 * (xcounts + 1), k2 = 7 * (xcounts + 1);
        for (int xcount = 0; xcount < xcounts; ++xcount, ++k1, ++k2)
        {
            indices.push_back(k1), indices.push_back(k2), indices.push_back(k2 + 1);
            indices.push_back(k1), indices.push_back(k2 + 1), indices.push_back(k1 + 1);
        }

        float angularStep = PI / xcounts;
        float radius = (ylen / 2);

        // curvy part

        // back-left
        for (int xcount = 0; xcount <= xcounts; ++xcount)
        {
            float angle = -(PI / 2) + xcount * angularStep;

            float nx = -cosf(angle);
            float ny = sinf(angle);
            float nz = 0.0f;

            float x = -(xlen / 2) + nx * radius;
            float y = ny * radius;
            float z = -(zlen / 2);

            float u = (ylen / 2) / (xlen + ylen) - cosf(angle) * radius / (xlen + ylen);
            float v = (1 + sinf(angle)) / 2;

            updateArrays(x, y, z, nx, ny, nz, u, v);
        }

        // front-left
        for (int xcount = 0; xcount <= xcounts; ++xcount)
        {
            float angle = -(PI / 2) + xcount * angularStep;

            float nx = -cosf(angle);
            float ny = sinf(angle);
            float nz = 0.0f;

            float x = -(xlen / 2) + nx * radius;
            float y = ny * radius;
            float z = (zlen / 2);

            float u = (ylen / 2) / (xlen + ylen) - cosf(angle) * radius / (xlen + ylen);
            float v = (1 + sinf(angle)) / 2;

            updateArrays(x, y, z, nx, ny, nz, u, v);
        }

        // left
        k1 = 8 * (xcounts + 1), k2 = 9 * (xcounts + 1);
        for (int xcount = 0; xcount < xcounts; ++xcount, ++k1, ++k2)
        {
            indices.push_back(k1), indices.push_back(k2), indices.push_back(k2 + 1);
            indices.push_back(k1), indices.push_back(k2 + 1), indices.push_back(k1 + 1);
        }

        // back-right
        for (int xcount = 0; xcount <= xcounts; ++xcount)
        {
            float angle = -(PI / 2) + xcount * angularStep;

            float nx = cosf(angle);
            float ny = sinf(angle);
            float nz = 0.0f;

            float x = (xlen / 2) + nx * radius;
            float y = ny * radius;
            float z = -(zlen / 2);

            float u = (ylen / 2 + xlen) / (xlen + ylen) + cosf(angle) * radius / (xlen + ylen);
            float v = (1 + sinf(angle)) / 2;

            updateArrays(x, y, z, nx, ny, nz, u, v);
        }

        // front-right
        for (int xcount = 0; xcount <= xcounts; ++xcount)
        {
            float angle = -(PI / 2) + xcount * angularStep;

            float nx = cosf(angle);
            float ny = sinf(angle);
            float nz = 0.0f;

            float x = (xlen / 2) + nx * radius;
            float y = ny * radius;
            float z = (zlen / 2);

            float u = (ylen / 2 + xlen) / (xlen + ylen) + cosf(angle) * radius / (xlen + ylen);
            float v = (1 + sinf(angle)) / 2;

            updateArrays(x, y, z, nx, ny, nz, u, v);
        }

        // right
        k1 = 10 * (xcounts + 1), k2 = 11 * (xcounts + 1);
        for (int xcount = 0; xcount < xcounts; ++xcount, ++k1, ++k2)
        {
            indices.push_back(k1), indices.push_back(k2), indices.push_back(k2 + 1);
            indices.push_back(k1), indices.push_back(k2 + 1), indices.push_back(k1 + 1);
        }

        // circular part

        // front-left
        for (int xcount = 0; xcount <= xcounts; ++xcount)
        {
            float angle = -(PI / 2) + xcount * angularStep;
            float x = -(xlen / 2) - cosf(angle) * radius;
            float y = sinf(angle) * radius;
            float z = (zlen / 2);

            float u = (ylen / 2) / (xlen + ylen) - cosf(angle) * radius / (xlen + ylen);
            float v = (1 + sinf(angle)) / 2;

            updateArrays(x, y, z, 0.0f, 0.0f, 1.0f, u, v);
        }

        // back-left
        for (int xcount = 0; xcount <= xcounts; ++xcount)
        {
            float angle = -(PI / 2) + xcount * angularStep;
            float x = -(xlen / 2) - cosf(angle) * radius;
            float y = sinf(angle) * radius;
            float z = -(zlen / 2);

            float u = (ylen / 2) / (xlen + ylen) - cosf(angle) * radius / (xlen + ylen);
            float v = (1 + sinf(angle)) / 2;

            updateArrays(x, y, z, 0.0f, 0.0f, -1.0f, u, v);
        }

        // front-right
        for (int xcount = 0; xcount <= xcounts; ++xcount)
        {
            float angle = -(PI / 2) + xcount * angularStep;
            float x = (xlen / 2) + cosf(angle) * radius;
            float y = sinf(angle) * radius;
            float z = (zlen / 2);

            float u = (ylen / 2 + xlen) / (xlen + ylen) + cosf(angle) * radius / (xlen + ylen);
            float v = (1 + sinf(angle)) / 2;

            updateArrays(x, y, z, 0.0f, 0.0f, 1.0f, u, v);
        }

        // back-right
        for (int xcount = 0; xcount <= xcounts; ++xcount)
        {
            float angle = -(PI / 2) + xcount * angularStep;
            float x = (xlen / 2) + cosf(angle) * radius;
            float y = sinf(angle) * radius;
            float z = -(zlen / 2);

            float u = (ylen / 2 + xlen) / (xlen + ylen) + cosf(angle) * radius / (xlen + ylen);
            float v = (1 + sinf(angle)) / 2;

            updateArrays(x, y, z, 0.0f, 0.0f, -1.0f, u, v);
        }

        updateArrays(-(xlen / 2), 0.0f, (zlen / 2), 0.0f, 0.0f, 1.0f, (ylen / 2) / (xlen + ylen), 0.5f); // front-left
        updateArrays(-(xlen / 2), 0.0f, -(zlen / 2), 0.0f, 0.0f, -1.0f, (ylen / 2) / (xlen + ylen), 0.5f); // back-left
        updateArrays((xlen / 2), 0.0f, (zlen / 2), 0.0f, 0.0f, 1.0f, (ylen / 2 + xlen) / (xlen + ylen) + 0.0f, 0.5f); // front-right
        updateArrays((xlen / 2), 0.0f, -(zlen / 2), 0.0f, 0.0f, -1.0f, (ylen / 2 + xlen) / (xlen + ylen) + 0.0f, 0.5f); // back-right

        int center_front_left = 16 * (xcounts + 1);
        int center_back_left = center_front_left + 1;
        int center_front_right = center_back_left + 1;
        int center_back_right = center_front_right + 1;

        // front-left
        k1 = 12 * (xcounts + 1);
        for (int xcount = 0; xcount < xcounts; ++xcount, ++k1)
        {
            indices.push_back(center_front_left), indices.push_back(k1), indices.push_back(k1 + 1);
        }
        // back-left
        k1 = 13 * (xcounts + 1);
        for (int xcount = 0; xcount < xcounts; ++xcount, ++k1)
        {
            indices.push_back(center_back_left), indices.push_back(k1), indices.push_back(k1 + 1);
        }
        // front-right
        k1 = 14 * (xcounts + 1);
        for (int xcount = 0; xcount < xcounts; ++xcount, ++k1)
        {
            indices.push_back(center_front_right), indices.push_back(k1), indices.push_back(k1 + 1);
        }
        // back-right
        k1 = 15 * (xcounts + 1);
        for (int xcount = 0; xcount < xcounts; ++xcount, ++k1)
        {
            indices.push_back(center_back_right), indices.push_back(k1), indices.push_back(k1 + 1);
        }
    }

    unsigned int stadiumVAO;
    
    float xlen, ylen, zlen;

    vector<float> vertices;
    vector<float> normals;
    vector<unsigned int> indices;
    vector<float> coordinates;

    vector<float> textureCoordinates;

    int verticesStride;

};


#endif /* stadium_h */



