#include "Mesh.h"
#include <fstream>
#include <sstream>
#include "VertexBuffer.h"
#include "VertexArray.h"
#include <iterator>
#include "glm/glm.hpp"
#include <unordered_map>
#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>


Mesh::Mesh(const std::string& path)
{
	Assimp::Importer Importer;
	const aiScene* pScene = Importer.ReadFile(path.c_str(), aiProcess_Triangulate | aiProcess_GenSmoothNormals | aiProcess_FlipUVs | aiProcess_JoinIdenticalVertices);

	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;



	return;

	LoadObj(path, vertices, indices);

	//TODO: should be set by the obj loader
	BufferLayout layout;
	layout.AddLayoutElement(3, GL_FLOAT, false, sizeof(float) * (3 + 2 + 3), 0);
	layout.AddLayoutElement(2, GL_FLOAT, false, sizeof(float) * (3 + 2 + 3), sizeof(float) * 3);
	layout.AddLayoutElement(3, GL_FLOAT, false, sizeof(float) * (3 + 2 + 3), sizeof(float) * (3 + 2));

	Init(&vertices[0].position.x, sizeof(float) * (3 + 2 + 3), vertices.size(), &indices[0], indices.size(), layout);
}
Mesh::Mesh(float* vertices, const unsigned vertexSize, const unsigned vertexCount, unsigned* indices, const unsigned indexCount, const BufferLayout& layout)
{	
	Init(vertices, vertexSize, vertexCount, indices, indexCount, layout);
}

Mesh::~Mesh()
{
	delete(_vao);
	delete(_vbo);
	delete(_ibo);
}

void Mesh::Init(float* vertices, const unsigned int vertexSize, const unsigned int vertexCount, unsigned int * indices, const unsigned int indexCount, const BufferLayout& layout)
{
	_vao = new VertexArray();
	_vbo = new VertexBuffer(&vertices[0], vertexSize * vertexCount, layout);
	_ibo = new IndexBuffer(&indices[0], indexCount);
	_vao->AddVertexBuffer(_vbo);
	_vao->SetIndexBuffer(_ibo);
	_vao->Unbind();
}

void Mesh::DrawGui()
{


}

void Mesh::LoadObj(const std::string& path, std::vector<Vertex>& vertices, std::vector<unsigned int>& indices)
{
	//TODO: support more obj files, only require position, not normal/uv, materials, start index
	
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
			std::istringstream positionStream(line.substr(2));
			glm::vec3 position;
			float x, y, z;
			positionStream >> x; positionStream >> y; positionStream >> z;
			position = glm::vec3(x, y, z);
			tempPositions.push_back(position);
		}
		else if (line.substr(0, 2) == "vt")
		{
			std::istringstream uvStream(line.substr(3));
			glm::vec2 uv;
			float u, v;
			uvStream >> u; uvStream >> v;
			uv = glm::vec2(u, v);
			tempUvs.push_back(uv);
		}
		else if (line.substr(0, 2) == "vn")
		{
			std::istringstream normalStream(line.substr(3));
			glm::vec3 normal;
			float x, y, z;
			normalStream >> x; normalStream >> y; normalStream >> z;
			normal = glm::vec3(x, y, z);
			tempNormals.push_back(normal);
		}
		else if (line.substr(0, 2) == "f ")
		{
			int a, b, c; // position index
			int A, B, C; // uv index
			int d, f, g; // normal index
			const char* chh = line.c_str();
			int matches = sscanf(chh, "f %i/%i/%i %i/%i/%i %i/%i/%i", &a, &A, &d, &b, &B, &f, &c, &C, &g);
			a--; b--; c--;
			A--; B--; C--;
			d--; f--; g--;
			positionIndices.push_back(a); uvIndices.push_back(A); normalIndices.push_back(d);
			positionIndices.push_back(b); uvIndices.push_back(B); normalIndices.push_back(f);
			positionIndices.push_back(c); uvIndices.push_back(C); normalIndices.push_back(g);
		}
	}
	//Construct vertices
	const int length = positionIndices.size();


	std::unordered_map<unsigned int, int> vertexIndices;

	for (int i = 0; i < length; i++)
	{
		Vertex vertex
		{ 
			tempPositions[positionIndices[i]],
			tempUvs[uvIndices[i]],
			tempNormals[normalIndices[i]]
		};
		vertices.push_back(vertex);
		unsigned int key = positionIndices[i] * 1000000000 + uvIndices[i] * 1000000 + normalIndices[i];
		if (vertexIndices.find(key) == vertexIndices.end())
			vertexIndices[key] = i;


		indices.push_back(vertexIndices[key]);
	}


}
