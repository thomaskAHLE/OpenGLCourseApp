#pragma once
#include<GL/glew.h>

class Mesh
{
public:
	Mesh();
	void createMesh(GLfloat *vertices, unsigned int * indices, unsigned int numVertices, unsigned int numIndices);
	void renderMesh();
	void clearMesh();
	~Mesh();
private:
	GLuint m_VAO, m_VBO, m_IBO;
	GLsizei m_indexCount;
};

