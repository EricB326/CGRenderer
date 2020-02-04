#include "Mesh.h"

Mesh::Mesh()
{
	// Set all data to zero
	m_VAO = 0;
	m_VBO = 0;
	m_EBO = 0;
	m_indexCount = 0;
}

Mesh::~Mesh()
{
	// Make sure the object is cleared
	ClearMesh();
}

void Mesh::CreateMesh(GLfloat* a_vertices, unsigned int* a_indices, unsigned int a_numOfVertices, unsigned int a_numOfIndices)
{
	// Set the index count
	m_indexCount = a_numOfIndices;

	// Create the vertex array ID and bind it
	glGenVertexArrays(1, &m_VAO);
	glBindVertexArray(m_VAO);

	// Create EBO buffer ID and bind it
	glGenBuffers(1, &m_EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
	// Attach the indices data to the EBO
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(a_indices[0]) * a_numOfIndices, a_indices, GL_STATIC_DRAW);

	// Create the vertex buffer ID and bind it
	glGenBuffers(1, &m_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	// Attach the vertex data to the VBO
	glBufferData(GL_ARRAY_BUFFER, sizeof(a_vertices[0]) * a_numOfVertices, a_vertices, GL_STATIC_DRAW);

	// Create and enable the attribute for the vertices
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(float), 0);
	glEnableVertexAttribArray(0);
	// Create and enable the attribute for the colours
	//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(a_vertices[0]) * 8, (GLvoid*)(sizeof(a_vertices[0]) * 3));
	//glEnableVertexAttribArray(1);
	//// Create and enable the attribute for the texture coords
	//glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(a_vertices[0]) * 8, (GLvoid*)(sizeof(a_vertices[0]) * 3));
	//glEnableVertexAttribArray(1);
	//// Create and enable the attribute for the normals
	//glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(a_vertices[0]) * 8, (GLvoid*)(sizeof(a_vertices[0]) * 5));
	//glEnableVertexAttribArray(2);

	// Unbind the VBO, EBO, and VAO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Mesh::RenderMesh()
{
	// Error checking for none existant IDs
	if (m_VAO == 0)
	{
		std::cout << "VAO ID does not exist!\n";
		return;
	}

	if (m_VBO == 0)
	{
		std::cout << "VBO ID does not exist!\n";
		return;
	}

	// Specify the VAO and EBO we are working with
	glBindVertexArray(m_VAO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);

	// Check if the program should draw using indicies or classic
	if (m_EBO != 0)
		glDrawElements(GL_TRIANGLES, m_indexCount, GL_UNSIGNED_INT, 0);
	else
		glDrawArrays(GL_TRIANGLES, 0, 3);

	// Unbind the VAO and EBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Mesh::ClearMesh()
{
	// Clear all the buffers off the graphics card and reset the IDs
	if (m_VAO != 0)
	{
		glDeleteVertexArrays(1, &m_VAO);
		m_VAO = 0;
	}

	if (m_VBO != 0)
	{
		glDeleteBuffers(1, &m_VBO);
		m_VBO = 0;
	}

	if (m_EBO != 0)
	{
		glDeleteBuffers(1, &m_EBO);
		m_EBO = 0;
	}

	// Reset the index count
	m_indexCount = 0;
}