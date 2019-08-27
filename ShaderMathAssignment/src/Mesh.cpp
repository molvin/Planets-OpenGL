#include "Mesh.h"
#include <fstream>
#include <sstream>
#include "VertexBuffer.h"
#include "VertexArray.h"
#include <iterator>
#include <map>

Mesh::Mesh(const std::string& path)
{
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;

	LoadObj(path, vertices, indices);

	BufferLayout layout;
	layout.AddLayoutElement(3, GL_FLOAT, false, sizeof(float) * (3 + 2 + 3), 0);
	layout.AddLayoutElement(2, GL_FLOAT, false, sizeof(float) * (3 + 2 + 3), sizeof(float) * 3);
	layout.AddLayoutElement(3, GL_FLOAT, false, sizeof(float) * (3 + 2 + 3), sizeof(float) * (3 + 2));

	_vao = new VertexArray();
	_vbo = new VertexBuffer(&vertices[0].position.x, sizeof(float) * (3 + 2 + 3) * vertices.size(), layout);
	_ibo = new IndexBuffer(&indices[0], indices.size());
	_vao->AddVertexBuffer(_vbo);
	_vao->SetIndexBuffer(_ibo);
}
Mesh::~Mesh()
{
	delete(_vao);
	delete(_vbo);
	delete(_ibo);
}

void Mesh::LoadObj(const std::string& path, std::vector<Vertex>& vertices, std::vector<unsigned int>& indices)
{
	std::ifstream file(path, std::ios::in);
	std::string line;

	std::vector<int> positionIndices;
	std::vector<int> uvIndices;
	std::vector<int> normalIndices;

	std::vector<glm::vec3> tempPositions;
	std::vector<glm::vec2> tempUvs;
	std::vector<glm::vec3> tempNormals;

	while(std::getline(file, line))
	{
		if (line.substr(0, 2) == "v ") 
		{
			std::istringstream vertStream(line.substr(2));
			glm::vec3 position;
			float x, y, z;
			vertStream >> x; vertStream >> y; vertStream >> z;
			position = glm::vec3(x, y, z);
			tempPositions.push_back(position);

			//printf("Vertex(x: %f, y: %f, z: %f)\n", x, y, z);
		}
		else if (line.substr(0, 2) == "vt")
		{
			std::istringstream uvStream(line.substr(3));
			glm::vec2 uv;
			float u, v;
			uvStream >> u; uvStream >> v;
			uv = glm::vec2(u, v);
			tempUvs.push_back(uv);

			//printf("UV(u: %f, v: %f)\n", u, v);
		}
		else if (line.substr(0, 2) == "vn")
		{
			std::istringstream normalStream(line.substr(3));
			glm::vec3 normal;
			float x, y, z;
			normalStream >> x; normalStream >> y; normalStream >> z;
			normal = glm::vec3(x, y, z);
			tempNormals.push_back(normal);

			//printf("Normal(x: %f, y: %f, z: %f)\n", x, y, z);
		}
		else if (line.substr(0, 2) == "f ")
		{
			int a, b, c; // position index
			int A, B, C; // uv index
			int d, f, g; // normal index
			const char* chh = line.c_str();
			sscanf(chh, "f %i/%i/%i %i/%i/%i %i/%i/%i", &a, &A, &d, &b, &B, &f, &c, &C, &g);
			a--; b--; c--;
			A--; B--; C--;
			d--; f--; g--;
			positionIndices.push_back(a); uvIndices.push_back(A); normalIndices.push_back(d);
			positionIndices.push_back(b); uvIndices.push_back(B); normalIndices.push_back(f);
			positionIndices.push_back(c); uvIndices.push_back(C); normalIndices.push_back(g);

			//printf("Face index: %d, %d, %d\n", a, b, c);
			//printf("UV index: %d, %d, %d\n", A, B, C);
			//printf("Normal index: %d, %d, %d\n", d, f, g);
		}
	}
	//Construct vertices
	//TODO: get rid of duplicate vertices, and update indices to reflect that
	const int length = positionIndices.size();
	std::map<Vertex, int> vertexIndices;
	for (int i = 0; i < length; i++)
	{
		Vertex vertex;
		vertex.position = tempPositions[positionIndices[i]];
		vertex.uv = tempUvs[uvIndices[i]];
		vertex.normal = tempNormals[normalIndices[i]];
		vertices.push_back(vertex);


		indices.push_back(i);


		//printf("Vertex[%d]: %d/%d/%d\n", i, positionIndices[i], uvIndices[i], normalIndices[i]);
	}


}
