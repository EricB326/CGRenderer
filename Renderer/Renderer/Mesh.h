#pragma once

// Standard includes
#include <iostream>

// Graphics includes
#include "gl_core_4_5.h"

class Mesh
{
public:
	Mesh();
	~Mesh();

	void CreateMesh(GLfloat* a_vertices, unsigned int* a_indices, unsigned int a_numOfVertices, unsigned int a_numOfIndices);
	void RenderMesh();
	void ClearMesh();

private:
	GLuint m_VAO, m_VBO, m_EBO;
	GLsizei m_indexCount;
};
