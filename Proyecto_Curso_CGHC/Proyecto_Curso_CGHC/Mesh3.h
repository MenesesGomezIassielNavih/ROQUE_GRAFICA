#pragma once
#include <glfw3.h>
class Mesh3
{
public:
	Mesh3();
	void CreateMesh(GLfloat* vertices, unsigned int* indices, unsigned int numOfVertices, unsigned int numberOfIndices);
	void RenderMesh();
	void RenderMeshGeometry();
	void ClearMesh();
	~Mesh3();

private:
	GLuint VAO, VBO, IBO;
	GLsizei indexCount;

};

class MeshColor3
{
public:
	MeshColor3();
	void CreateMeshColor(GLfloat* vertices, unsigned int numOfVertices);
	void RenderMeshColor();
	void ClearMeshColor();
	~MeshColor3();
private:
	GLuint VAO, VBO;
	GLsizei  vertexCount;
};

