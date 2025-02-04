#ifndef cylinder_h
#define cylinder_h

#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.h"

#include <vector>

using namespace std;
using namespace glm;

#define PI 3.1416

class Cylinder
{
public:
	Cylinder(float topRadius = 1.0f, float baseRadius = 1.0f,
		float height = 1.0f, int stackCount = 100, int sectorCount = 36)
	{
		set(topRadius, baseRadius, height, stackCount, sectorCount);

		generateData();

		glGenVertexArrays(1, &VAO);
		glBindVertexArray(VAO);

		glGenBuffers(1, &VBO);
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, this->getVertexVectorSize(), 
			this->getVertices(), GL_STATIC_DRAW);

		glGenBuffers(1, &EBO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->getIndexVectorSize(),
			this->getIndices(), GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, (void*)(0 * sizeof(float)));
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, (void*)(3 * sizeof(float)));
		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, (void*)(6 * sizeof(float)));

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);

		glBindVertexArray(0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

	}
	~Cylinder() {}
	
	void set(float topRadius, float baseRadius, float height,
		int stackCount, int sectorCount)
	{
		this->topRadius = topRadius;
		this->baseRadius = baseRadius;
		this->height = height;
		this->stackCount = stackCount;
		this->sectorCount = sectorCount;
	}

	unsigned int getVertexVectorSize() const
	{
		return (unsigned int)vertices.size() * sizeof(float);
	}
	const float* getVertices() const
	{
		return vertices.data();
	}

	unsigned int getIndexVectorCount() const
	{
		return (unsigned int)indices.size();
	}
	unsigned int getIndexVectorSize() const
	{
		return (unsigned int)indices.size() * sizeof(unsigned int);
	}
	const unsigned int* getIndices() const
	{
		return indices.data();
	}

	void draw(Shader& shader) const
	{
		shader.use();

		glBindVertexArray(VAO);

		glDrawElements(GL_TRIANGLES, this->getIndexVectorCount(),
			GL_UNSIGNED_INT, (void*)0);

		glBindVertexArray(0);
	}
private:
	void add(float x, float y, float z, float nx, float ny, float nz,
		float u, float v)
	{
		vertices.push_back(x), vertices.push_back(y), vertices.push_back(z);
		vertices.push_back(nx), vertices.push_back(ny), vertices.push_back(nz);
		vertices.push_back(u), vertices.push_back(v);
	}

	void updateNormal(int idx1, int idx2, int idx3)
	{
		idx1 *= 8, idx2 *= 8, idx3 *= 8;

		float x1 = vertices[idx1 + 0], y1 = vertices[idx1 + 1], z1 = vertices[idx1 + 2];
		float x2 = vertices[idx2 + 0], y2 = vertices[idx2 + 1], z2 = vertices[idx2 + 2];
		float x3 = vertices[idx3 + 0], y3 = vertices[idx3 + 1], z3 = vertices[idx3 + 2];

		float v21x = x1 - x2;
		float v21y = y1 - y2;
		float v21z = z1 - z2;

		float v23x = x3 - x2;
		float v23y = y3 - y2;
		float v23z = z3 - z2;

		float nx = v23y * v21z - v23z * v21y;
		float ny = v23z * v21x - v23x * v21z;
		float nz = v23x * v21y - v23y * v21x;

		float length = sqrtf(nx * nx + ny * ny + nz * nz);
		
		nx /= length;
		ny /= length;
		nz /= length;

		//cout << vertices[idx2 + 3] << " " << vertices[idx2 + 4] << " " << vertices[idx2 + 5] << endl;
		//cout << nx << " " << ny << " " << nz << endl;

		vertices[idx2 + 3] = -nx, vertices[idx2 + 4] = -ny, vertices[idx2 + 5] = -nz;
	}

	void generateData()
	{
		vector<float> topCircle, baseCircle;
		for (int i = 0; i <= stackCount; ++i)
		{
			float y = (height / 2.0f) - (float)i / stackCount * height;
			float radius = topRadius + (float)i / stackCount * (baseRadius - topRadius);

			float ny = 0.0f;

			float v = 1 - (float)i / stackCount;

			for (int j = 0; j <= sectorCount; ++j)
			{
				float sectorAngle = (float)j / sectorCount * 2 * PI;

				float x = radius * cosf(sectorAngle);
				float z = radius * sinf(sectorAngle);

				float nx = cosf(sectorAngle);
				float nz = sinf(sectorAngle);

				float u = (float)j / sectorCount;

				add(x, y, z, nx, ny, nz, u, v);

				if (i == 0)
				{
					float u = nx / 2.0f + 0.5f;
					float v = nz / 2.0f + 0.5f;

					topCircle.push_back(x), topCircle.push_back(y), topCircle.push_back(z);
					topCircle.push_back(0.0f), topCircle.push_back(1.0f), topCircle.push_back(0.0f);
					topCircle.push_back(u), topCircle.push_back(v);
				}
				else if (i == stackCount)
				{
					float u = nx / 2.0f + 0.5f;
					float v = nz / 2.0f + 0.5f;

					baseCircle.push_back(x), baseCircle.push_back(y), baseCircle.push_back(z);
					baseCircle.push_back(0.0f), baseCircle.push_back(-1.0f), baseCircle.push_back(0.0f);
					baseCircle.push_back(u), baseCircle.push_back(v);
				}
			}
		}

		for (auto a : topCircle)
		{
			vertices.push_back(a);
		}
		for (auto a : baseCircle)
		{
			vertices.push_back(a);
		}

		add(0.0f, height / 2.0f, 0.0f, 0.0f, 1.0f, 0.f, 0.5f, 0.5f);
		add(0.0f, -(height / 2.0f), 0.0f, 0.0f, -1.0f, 0.f, 0.5f, 0.5f);

		for (int i = 0; i < stackCount; ++i)
		{
			int k1 = i * (sectorCount + 1), k2 = k1 + (sectorCount + 1);
			for (int j = 0; j < sectorCount; ++j, ++k1, ++k2)
			{
				indices.push_back(k1), indices.push_back(k2), indices.push_back(k2 + 1);
				indices.push_back(k2 + 1), indices.push_back(k1 + 1), indices.push_back(k1);
				
				if (i != 0)
				{
					updateNormal(k1, k2, k2 + 1);
					updateNormal(k2 + 1, k1 + 1, k1);
				}
			}
		}

		// top Circle
		int k = (stackCount + 1) * (sectorCount + 1);
		int center = (stackCount + 3) * (sectorCount + 1);

		for (int i = 0; i < sectorCount; ++i, ++k)
		{
			indices.push_back(center), indices.push_back(k), indices.push_back(k + 1);
		}

		// base Circle
		k = (stackCount + 2) * (sectorCount + 1);
		center = center + 1;

		for (int i = 0; i < sectorCount; ++i, ++k)
		{
			indices.push_back(center), indices.push_back(k), indices.push_back(k + 1);
		}
	}


	float topRadius, baseRadius;
	float height;
	float stackCount, sectorCount;

	unsigned int VAO, VBO, EBO;

	vector<float> coordinates;
	vector<float> normals;
	vector<float> textureCoordinates;

	vector<float> vertices;
	vector<unsigned int> indices;

	int stride = (3 + 3 + 2) * sizeof(float);
};

#endif
