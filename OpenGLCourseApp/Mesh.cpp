#include "Mesh.h"



Mesh::Mesh()
{
	m_VAO = m_VBO = m_IBO = 0;
	m_indexCount = 0;
}

void Mesh::createMesh(GLfloat * vertices, unsigned int * indices, unsigned int numVertices, unsigned int numIndices)
{
	m_indexCount = numIndices;

	//num of arrays, where to store id 
	glGenVertexArrays(1, &m_VAO);
	glBindVertexArray(m_VAO);

	glGenBuffers(1, &m_IBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * numIndices, indices, GL_STATIC_DRAW);

	glGenBuffers(1, &m_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	//static draw values not changing after put in buffer
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * numVertices, vertices, GL_STATIC_DRAW);


	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(vertices[0])* 8, 0);
	glEnableVertexAttribArray(0);

	//setting for texture coordinates
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(vertices[0]) *8, (void*)(sizeof(vertices[0])*3));
	glEnableVertexAttribArray(1);

	//for normals
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(vertices[0]) * 8, (void*)(sizeof(vertices[0]) * 5));
	glEnableVertexAttribArray(2);

	//unbinding buffer and array
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Mesh::renderMesh()
{
	if (m_VAO && m_VBO && m_IBO)
	{
		glBindVertexArray(m_VAO);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBO);
		glDrawElements(GL_TRIANGLES, m_indexCount, GL_UNSIGNED_INT, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}
}


// clear mesh data off of graphics card
void Mesh::clearMesh()
{
	if (m_IBO)
	{
		glDeleteBuffers(1, &m_IBO);
		m_IBO = 0;
	}
	if (m_VBO)
	{
		glDeleteBuffers(1, &m_VBO);
		m_VBO= 0;
	}
	if (m_VAO)
	{
		glDeleteVertexArrays(1, &m_VAO);
		m_VAO = 0;
	}
	m_indexCount = 0;
}

Mesh::~Mesh()
{
	clearMesh();
}
