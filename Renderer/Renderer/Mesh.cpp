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
	switch (a_vertex_type)
	{	
		case BASE_VERTEX:
		{
			// Loop through the passed vertice array, jump by 6 spaces each time to return
			// to the "start" of each new "segment" of data example:
			// verts[] = { 3, 0, -2, 0.5f, 0.5f, 0.5f,   <-- Stats at "3", reads all the data on that line to the final "0.5f"
			//			  -3, 0, 2, 1.0f, 1.0f, 1.0f };  <-- Jumped += 6 positions to "-3" and reads all data to final "1.0f"
			for (size_t i = 0; i < m_vert_count; i += 6)
			{
				// Create a tempory vertex holder for pushing into the the <vector>
				auto vert = base_vertex();

				// Read all the data on that "segment" of the array
				vert.m_position = glm::vec3(a_vertices[i], a_vertices[i + 1], a_vertices[i + 2]);
				vert.m_colour = glm::vec3(a_vertices[i + 3], a_vertices[i + 4], a_vertices[i + 5]);

				// Pust the data into the <vertor> storage
				m_base_vert.push_back(vert);
			}

			// Setup the meshes ID's
			setup_base_mesh();
			break;
		}
		case STANDARD_VERTEX:
		{
			// Read above lines: 16 -> 19. Instead of jumping by 6, jump by 8.
			for (size_t i = 0; i < m_vert_count; i += 8)
			{
				// Create a tempory vertex holder for pushing into the the <vector>
				auto vert = standard_vertex();

				// Read all the data on that "segment" of the array
				vert.m_position = glm::vec3(a_vertices[i], a_vertices[i + 1], a_vertices[i + 2]);
				vert.m_normal = glm::vec3(a_vertices[i + 3], a_vertices[i + 4], a_vertices[i + 5]);
				vert.m_tex_coords = glm::vec2(a_vertices[i + 6], a_vertices[i + 7]);

				// Pust the data into the <vertor> storage
				m_standard_vert.push_back(vert);
			}

			// Setup the meshes ID's
			setup_standard_mesh();
			break;
		}
		case FULL_VERTEX:
		{
			// Read above lines: 16 -> 19. Instead of jumping by 6, jump by 19.
			for (size_t i = 0; i < m_vert_count; i += 19)
			{
				// Create a tempory vertex holder for pushing into the the <vector>
				auto vert = full_vertex();

				// Read all the data on that "segment" of the array
				vert.m_position = glm::vec3(a_vertices[i], a_vertices[i + 1], a_vertices[i + 2]);
				vert.m_colour = glm::vec3(a_vertices[i + 3], a_vertices[i + 4], a_vertices[i + 5]);

				vert.m_normal = glm::vec3(a_vertices[i + 6], a_vertices[i + 7], a_vertices[i + 8]);
				vert.m_tangent = glm::vec4(a_vertices[i + 9], a_vertices[i + 10], a_vertices[i + 11], a_vertices[i + 12]);

				vert.m_tex_coords001 = glm::vec2(a_vertices[i + 13], a_vertices[i + 14]);
				vert.m_tex_coords002 = glm::vec2(a_vertices[i + 15], a_vertices[i + 16]);
				vert.m_tex_coords003 = glm::vec2(a_vertices[i + 17], a_vertices[i + 18]);

				// Pust the data into the <vertor> storage
				m_full_vert.push_back(vert);
			}

			// Setup the meshes ID's
			setup_full_mesh();
			break;
		}
		default:
		{
			printf("No vertex type detected!\n");
			return;
		}
	}
}

Mesh::~Mesh()
{
	// Make sure the object is cleared
	clear_mesh();
}

void Mesh::render_mesh()
{
	// Error checking for none existant IDs
	if (!m_VAO)
	{
		printf("VAO ID does not exist!\n");
		return;
	}

	if (!m_VBO)
	{
		printf("VBO ID does not exist!\n");
		return;
	}

	// Specify the VAO and EBO we are working with
	glBindVertexArray(m_VAO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);

	// Check if the program should draw using indicies
	if (m_EBO)
		glDrawElements(GL_TRIANGLES, m_index_count, GL_UNSIGNED_INT, 0);
	else
		glDrawArrays(GL_TRIANGLES, 0, 3 * m_index_count);

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

void Mesh::setup_base_mesh()
{
	// Create the vertex array ID and bind it
	glGenVertexArrays(1, &m_VAO);
	glBindVertexArray(m_VAO);

	// Create the vertex buffer ID and bind it
	glGenBuffers(1, &m_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	// Attach the vertex data to the VBO
	glBufferData(GL_ARRAY_BUFFER, sizeof(base_vertex) * m_base_vert.size(), &m_base_vert[0], GL_STATIC_DRAW);

	// Create EBO buffer ID and bind it
	glGenBuffers(1, &m_EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
	// Attach the indices data to the EBO
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * m_index_count, &m_indices[0], GL_STATIC_DRAW);

	// Create and enable the attribute for the position
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(base_vertex), (GLvoid*)0);
	// Create and enable the attribute for the colours
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(base_vertex), (GLvoid*)offsetof(base_vertex, m_colour));

	// Unbind the VBO, EBO, and VAO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Mesh::setup_standard_mesh()
{
	// Create the vertex array ID and bind it
	glGenVertexArrays(1, &m_VAO);
	glBindVertexArray(m_VAO);

	// Create the vertex buffer ID and bind it
	glGenBuffers(1, &m_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	// Attach the vertex data to the VBO
	glBufferData(GL_ARRAY_BUFFER, sizeof(standard_vertex) * m_standard_vert.size(), &m_standard_vert[0], GL_STATIC_DRAW);

	// Create EBO buffer ID and bind it
	glGenBuffers(1, &m_EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
	// Attach the indices data to the EBO
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * m_index_count, &m_indices[0], GL_STATIC_DRAW);

	// Create and enable the attribute for the position
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(standard_vertex), (GLvoid*)0);
	// Create and enable the attribute for the normals
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(standard_vertex), (GLvoid*)offsetof(standard_vertex, m_normal));
	// Create and enable the attribute for the texels (U/V)
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(standard_vertex), (GLvoid*)offsetof(standard_vertex, m_tex_coords));

	// Unbind the VBO, EBO, and VAO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Mesh::setup_full_mesh()
{
	// Create the vertex array ID and bind it
	glGenVertexArrays(1, &m_VAO);
	glBindVertexArray(m_VAO);

	// Create the vertex buffer ID and bind it
	glGenBuffers(1, &m_VBO);
	glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
	// Attach the vertex data to the VBO
	glBufferData(GL_ARRAY_BUFFER, sizeof(full_vertex) * m_full_vert.size(), &m_full_vert[0], GL_STATIC_DRAW);

	// Create EBO buffer ID and bind it
	glGenBuffers(1, &m_EBO);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
	// Attach the indices data to the EBO
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * m_index_count, &m_indices[0], GL_STATIC_DRAW);

	// Create and enable the attribute for the position
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(full_vertex), (GLvoid*)0);
	// Create and enable the attribute for the colour
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(full_vertex), (GLvoid*)offsetof(full_vertex, m_colour));
	// Create and enable the attribute for the normal
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(full_vertex), (GLvoid*)offsetof(full_vertex, m_normal));
	// Create and enable the attribute for the tangent
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(full_vertex), (GLvoid*)offsetof(full_vertex, m_tangent));
	// Create and enable the attribute for the texels (U/V) 001
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 2, GL_FLOAT, GL_FALSE, sizeof(full_vertex), (GLvoid*)offsetof(full_vertex, m_tex_coords001));
	// Create and enable the attribute for the texels (U/V) 002
	glEnableVertexAttribArray(5);
	glVertexAttribPointer(5, 2, GL_FLOAT, GL_FALSE, sizeof(full_vertex), (GLvoid*)offsetof(full_vertex, m_tex_coords002));
	// Create and enable the attribute for the texels (U/V) 003
	glEnableVertexAttribArray(6);
	glVertexAttribPointer(6, 2, GL_FLOAT, GL_FALSE, sizeof(full_vertex), (GLvoid*)offsetof(full_vertex, m_tex_coords003));

	// Unbind the VBO, EBO, and VAO
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}

void Mesh::clear_mesh()
{
	// Clear all the buffers off the graphics card and reset the IDs
	if (m_VAO)
	{
		glDeleteVertexArrays(1, &m_VAO);
		m_VAO = 0;
	}

	if (m_VBO)
	{
		glDeleteBuffers(1, &m_VBO);
		m_VBO = 0;
	}

	if (m_EBO)
	{
		glDeleteBuffers(1, &m_EBO);
		m_EBO = 0;
	}

	// Reset the index count
	m_index_count = 0;
}