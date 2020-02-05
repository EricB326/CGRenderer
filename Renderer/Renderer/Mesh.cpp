#include "Mesh.h"

Mesh::Mesh(GLfloat* a_vertices, uint* a_indices, uint a_num_of_verts, uint a_num_of_indices, int a_vertex_type)
	: m_VAO(0), m_VBO(0), m_EBO(0), m_indices(a_indices), m_index_count(a_num_of_indices), m_vert_count(a_num_of_verts)
{
	// Check to see if the vertex type is invalid
	if (a_vertex_type < 0 || a_vertex_type > 1)
	{
		std::cout << "Failed to construct mesh! Invalid vertex typed passed!\n";
		return;
	}

	// Check what vetex struct is being populated
	if (a_vertex_type == BASE_VERTEX)
	{
		for (size_t i = 0; i < m_vert_count; i += 6)
		{
			auto vert = base_vertex();

			vert.m_position = glm::vec3(a_vertices[i], a_vertices[i + 1], a_vertices[i + 2]);
			vert.m_colour = glm::vec3(a_vertices[i + 3], a_vertices[i + 4], a_vertices[i + 5]);

			m_basic_verticies.push_back(vert);
		}
	}
	else if (a_vertex_type == STANDARD_VERTEX)
	{
		for (size_t i = 0; i < m_vert_count; i++)
		{
			auto vert = vertex();

			vert.m_position = glm::vec3(a_vertices[i], a_vertices[i + 1], a_vertices[i + 2]);
			vert.m_colour = glm::vec3(a_vertices[i + 3], a_vertices[i + 4], a_vertices[i + 5]);

			m_basic_verticies.push_back(vert);
		}
	}

	setup_mesh();
}

Mesh::~Mesh()
{
	// Make sure the object is cleared
	clear_mesh();
}

void Mesh::render_mesh()
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
		glDrawElements(GL_TRIANGLES, m_index_count, GL_UNSIGNED_INT, 0);
	else
		glDrawArrays(GL_TRIANGLES, 0, 3);

	// Unbind the VAO and EBO
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Mesh::render()
{
	render_mesh();
}

void Mesh::draw()
{
	render_mesh();
}

void Mesh::setup_mesh()
{
	// Create the vertex array ID and bind it
	glGenVertexArrays(1, &m_VAO);
	glBindVertexArray(m_VAO);

	// Create the vertex buffer ID and bind it
	glGenBuffers(1, &m_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	// Attach the vertex data to the VBO
	glBufferData(GL_ARRAY_BUFFER, sizeof(base_vertex) * m_basic_verticies.size(), &m_basic_verticies[0], GL_STATIC_DRAW);
	//glBufferData(GL_ARRAY_BUFFER, sizeof(a_vertices[0]) * a_numOfVertices, a_vertices, GL_STATIC_DRAW);

	// Create EBO buffer ID and bind it
	glGenBuffers(1, &m_EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
	// Attach the indices data to the EBO
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * m_index_count, &m_indices[0], GL_STATIC_DRAW);
	//glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(a_indices[0]) * a_numOfIndices, a_indices, GL_STATIC_DRAW);

	// Create and enable the attribute for the vertices
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(base_vertex), (GLvoid*)0);
	// Create and enable the attribute for the colours
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(base_vertex), (GLvoid*)offsetof(base_vertex, m_colour));

	// Create and enable the attribute for the vertices
	//glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(a_vertices[0]) * 8, 0);
	//glEnableVertexAttribArray(0);
	//// Create and enable the attribute for the colours
	//glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(a_vertices[0]) * 8, (GLvoid*)(sizeof(a_vertices[0]) * 3));
	//glEnableVertexAttribArray(1);
	//// Create and enable the attribute for the texture coords
	//glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(a_vertices[0]) * 8, (GLvoid*)(sizeof(a_vertices[0]) * 3));
	//glEnableVertexAttribArray(1);
	//// Create and enable the attribute for the normals
	//glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(a_vertices[0]) * 8, (GLvoid*)(sizeof(a_vertices[0]) * 6));
	//glEnableVertexAttribArray(2);

	// Unbind the VBO, EBO, and VAO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Mesh::clear_mesh()
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
	m_index_count = 0;
}