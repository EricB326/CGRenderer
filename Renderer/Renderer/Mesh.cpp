#include "Mesh.h"

/* .OBJ importing includes
*/
#define TINYOBJLOADER_IMPLEMENTATION
#include "..\Dependencies\tiny_obj\tiny_obj_loader.h"

#include <chrono>
#include "MaterialManager.h"
#include "TextureManager.h"
#include <vector>


namespace uciniti
{
	/* @detail A foward declaration of the create_texture_maps() function.
		This is done because the library tinyobj needs specific implementation,
		not allowing for its variables its variables to be created outside of the
		class where the implementation is done. You cannot move this implementation
		to the header file as the functions the library uses are only usable when the
		implementation is within the same file.
	*/
	void create_texture_maps(tinyobj::material_t a_material, bool a_should_textures_flip, const char* a_material_name);

	Mesh::Mesh()
		: m_empty_mesh(true), m_is_geometry_user_defined(false), m_VAO(0), m_VBO(0), m_EBO(0), m_index_count(0)
	{}

	Mesh::Mesh(const char* a_filepath, const char* a_material_name, bool a_load_textures, bool a_flip_textures)
		: m_empty_mesh(true), m_is_geometry_user_defined(false), m_VAO(0), m_VBO(0), m_EBO(0), m_index_count(0)
	{
		load_obj(a_filepath, a_material_name, a_load_textures, a_flip_textures);
	}

	Mesh::Mesh(GLfloat* a_vertices, uint* a_indices, uint a_num_of_verts, uint a_num_of_indices, vertex_type a_vertex_type)
		: m_empty_mesh(true), m_is_geometry_user_defined(true), m_VAO(0), m_VBO(0), m_EBO(0), m_index_count(a_num_of_indices)
	{
		// Check to see if the vertex type is invalid
		if ((uint)a_vertex_type < 0 || (uint)a_vertex_type > 1)
		{
			std::cout << "Failed to construct mesh! Invalid vertex typed passed!\n";
			return;
		}
	
		// Check what vetex struct is being populated
		switch (a_vertex_type)
		{
			case vertex_type::BASE_VERTEX:
			{
				// Loop through the passed vertice array, jump by 6 spaces each time to return
				// to the "start" of each new "segment" of data example:
				// verts[] = { 3, 0, -2, 0.5f, 0.5f, 0.5f,   <-- Stats at "3", reads all the data on that line to the final "0.5f"
				//			  -3, 0, 2, 1.0f, 1.0f, 1.0f };  <-- Jumped += 6 positions to "-3" and reads all data to final "1.0f"
				for (size_t i = 0; i < a_num_of_verts; i += 6)
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
				setup_base_mesh(m_VAO, m_VBO, m_EBO, a_indices);
				break;
			}
			case vertex_type::STANDARD_VERTEX:
			{
				// Read above lines: 16 -> 19. Instead of jumping by 6, jump by 18.
				for (size_t i = 0; i < a_num_of_verts; i += 18)
				{
					// Create a tempory vertex holder for pushing into the the <vector>
					auto vert = standard_vertex();
	
					// Read all the data on that "segment" of the array
					vert.m_position = glm::vec4(a_vertices[i], a_vertices[i + 1], a_vertices[i + 2], a_vertices[i + 3]);
					vert.m_normal = glm::vec4(a_vertices[i + 4], a_vertices[i + 5], a_vertices[i + 6], a_vertices[i + 7]);
	
					vert.m_tangent = glm::vec4(a_vertices[i + 8], a_vertices[i + 9], a_vertices[i + 10], a_vertices[i + 11]);
	
					vert.m_tex_coords = glm::vec2(a_vertices[i + 12], a_vertices[i + 13]);
	
					// Pust the data into the <vertor> storage
					m_standard_vert.push_back(vert);
				}
	
				// Setup the meshes ID's
				setup_standard_mesh(m_VAO, m_VBO, m_EBO, a_indices);
				break;
			}
			case vertex_type::FULL_VERTEX:
			{
				// Read above lines: 16 -> 19. Instead of jumping by 6, jump by 19.
				for (size_t i = 0; i < a_num_of_verts; i += 19)
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
				setup_full_mesh(m_VAO, m_VBO, m_EBO, a_indices);
				break;
			}
			default:
			{
				printf("No vertex type detected!\n");
				return;
			}
		}
	}

	void Mesh::render_mesh()
	{
		// If the geometry is user defined, don't bother checking
		// for sub meshes as it is not possible. Only for loaded
		// meshes via files.
		if (m_is_geometry_user_defined)
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
		else
		{
			for (auto& sub_mesh : sub_meshes)
			{
				// Error checking for none existant IDs
				if (!sub_mesh.m_VAO)
				{
					printf("VAO ID does not exist!\n");
					return;
				}

				if (!sub_mesh.m_VBO)
				{
					printf("VBO ID does not exist!\n");
					return;
				}

				// Specify the VAO and EBO we are working with
				glBindVertexArray(sub_mesh.m_VAO);
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, sub_mesh.m_EBO);

				// Check if the program should draw using indicies
				if (sub_mesh.m_EBO)
					glDrawElements(GL_TRIANGLES, sub_mesh.m_index_count, GL_UNSIGNED_INT, 0);
				else
					glDrawArrays(GL_TRIANGLES, 0, 3 * sub_mesh.m_index_count);

				// Unbind the VAO and EBO
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
				glBindVertexArray(0);
			}
		}
	}

	void Mesh::render()
	{
		render_mesh();
	}

	void Mesh::draw()
	{
		render_mesh();
	}
	
	bool Mesh::load_obj(const char* a_filepath, const char* a_material_name, bool a_load_textures, bool a_should_textures_flip)
	{
		// Check to see if the mesh already has a model loaded.
		if (!m_empty_mesh)
		{
			clear_mesh();
		}
		// Store the seperate objects and their faces.
		std::vector<tinyobj::shape_t> shapes;
		// Materials and textures per face.
		std::vector<tinyobj::material_t> materials;
		// Contain errors that occur while loading files.
		std::string err;

		std::string file = a_filepath;
		std::string folder = file.substr(0, file.find_last_of('/') + 1);
		
		// Check current time point.
		if (!tinyobj::LoadObj(shapes, materials, err, a_filepath, folder.c_str()))
		{
			printf("\nERROR: load_obj() call. Failed to load file '%s'!.\n", a_filepath);
			printf("%s\n", err.c_str());
			return false;
		}

		// False, the geometry is not from the user but instead a file.
		m_is_geometry_user_defined = false;

		// Pass the material data.
		std::vector<glm::vec3> loaded_mesh_materials;
		float loaded_specular_shininess = 0.0f, loaded_alpha = 0.0f;
		for (const auto& material : materials)
		{
			loaded_mesh_materials.push_back((glm::vec3)(material.ambient[0]));
			loaded_mesh_materials.push_back((glm::vec3)(material.diffuse[0]));
			loaded_mesh_materials.push_back((glm::vec3)(material.specular[0]));
			loaded_mesh_materials.push_back((glm::vec3)(material.emission[0]));

			loaded_specular_shininess = material.shininess;
			loaded_alpha = material.dissolve;

		}

		MaterialManager::inst().create_material(a_material_name, loaded_mesh_materials, loaded_specular_shininess, loaded_alpha);

		for (const auto& material : materials)
		{
			create_texture_maps(material, a_should_textures_flip, a_material_name);
		}

		// Pass data to the correct struct class.
		for (const auto& shape : shapes)
		{
			sub_mesh_data sub_mesh;
			glGenVertexArrays(1, &sub_mesh.m_VAO);
			glGenBuffers(1, &sub_mesh.m_VBO);
			glGenBuffers(1, &sub_mesh.m_EBO);

			// Set the size vertex vector, as well as check how many verts there are.
			m_standard_vert.resize(shape.mesh.positions.size() / 3);
			size_t vert_count = m_standard_vert.size();

			// Copy indice data into member variables.
			sub_mesh.m_index_count = shape.mesh.indices.size();
			sub_mesh.m_indices = shape.mesh.indices.data();

			// Populate the vertex.
			for (size_t i = 0; i < vert_count; i++)
			{
				// If there is mesh positional data, populate the position.
				if (!shape.mesh.positions.empty())
					m_standard_vert[i].m_position = glm::vec4(shape.mesh.positions[i * 3], shape.mesh.positions[i * 3 + 1], shape.mesh.positions[i * 3 + 2], 1.0f);
				// Invert the texture if the texture requires it.
				if (!shape.mesh.texcoords.empty())
					m_standard_vert[i].m_tex_coords = glm::vec2(shape.mesh.texcoords[i * 2], a_should_textures_flip ? 1.0f - shape.mesh.texcoords[i * 2 + 1] : shape.mesh.texcoords[i * 2 + 1]);
				// If there is mesh normals data, populate the normal.
				if (!shape.mesh.normals.empty())
					m_standard_vert[i].m_normal = glm::vec4(shape.mesh.normals[i * 3], shape.mesh.normals[i * 3 + 1], shape.mesh.normals[i * 3 + 2], 0.0f);

				//printf("Index: %i - norm0: %f | norm1: %f | norm2: %f", i, shape.mesh.normals[i * 3], shape.mesh.normals[i * 3 + 1], shape.mesh.normals[i * 3 + 2]);
				//printf("\n");
			}

			// Calculate tangents for normal mapping.
			if (!shape.mesh.normals.empty() && !shape.mesh.texcoords.empty())
				calculate_vertex_tangents(m_standard_vert, sub_mesh.m_indices, sub_mesh.m_index_count);

			// Prepare the mesh for rendering.
			m_index_count = sub_mesh.m_index_count;
			setup_standard_mesh(sub_mesh.m_VAO, sub_mesh.m_VBO, sub_mesh.m_EBO, sub_mesh.m_indices);

			sub_meshes.push_back(sub_mesh);
		}


		return true;
	}

	void Mesh::calculate_vertex_tangents(std::vector<standard_vertex>& a_vertices, const uint* a_indices, const GLsizei a_index_count)
	{
		/* Reference:
		   Title: Foundations of Game Engine Development, Volume 2: Rendering
		   Author: Eric Lengyel
		   Date: 2019
		   Availability: http://foundationsofgameenginedev.com/
		*/

		unsigned int vertexCount = (unsigned int)a_vertices.size();
		glm::vec4* tan1 = new glm::vec4[vertexCount * 2];
		glm::vec4* tan2 = tan1 + vertexCount;
		memset(tan1, 0, vertexCount * sizeof(glm::vec4) * 2);

		unsigned int indexCount = a_index_count;
		for (unsigned int a = 0; a < indexCount; a += 3) {
			long i1 = a_indices[a];
			long i2 = a_indices[a + 1];
			long i3 = a_indices[a + 2];

			const glm::vec4& v1 = a_vertices[i1].m_position;
			const glm::vec4& v2 = a_vertices[i2].m_position;
			const glm::vec4& v3 = a_vertices[i3].m_position;

			const glm::vec2& w1 = a_vertices[i1].m_tex_coords;
			const glm::vec2& w2 = a_vertices[i2].m_tex_coords;
			const glm::vec2& w3 = a_vertices[i3].m_tex_coords;

			float x1 = v2.x - v1.x;
			float x2 = v3.x - v1.x;
			float y1 = v2.y - v1.y;
			float y2 = v3.y - v1.y;
			float z1 = v2.z - v1.z;
			float z2 = v3.z - v1.z;

			float s1 = w2.x - w1.x;
			float s2 = w3.x - w1.x;
			float t1 = w2.y - w1.y;
			float t2 = w3.y - w1.y;

			float r = 1.0F / (s1 * t2 - s2 * t1);
			glm::vec4 sdir((t2 * x1 - t1 * x2) * r, (t2 * y1 - t1 * y2) * r,
				(t2 * z1 - t1 * z2) * r, 0);
			glm::vec4 tdir((s1 * x2 - s2 * x1) * r, (s1 * y2 - s2 * y1) * r,
				(s1 * z2 - s2 * z1) * r, 0);

			tan1[i1] += sdir;
			tan1[i2] += sdir;
			tan1[i3] += sdir;

			tan2[i1] += tdir;
			tan2[i2] += tdir;
			tan2[i3] += tdir;
		}

		for (unsigned int a = 0; a < vertexCount; a++) {
			const glm::vec3& n = glm::vec3(a_vertices[a].m_normal);
			const glm::vec3& t = glm::vec3(tan1[a]);

			// Gram-Schmidt orthogonalize
			a_vertices[a].m_tangent = glm::vec4(glm::normalize(t - n * glm::dot(n, t)), 0);

			// Calculate handedness (direction of bitangent)
			a_vertices[a].m_tangent.w = (glm::dot(glm::cross(glm::vec3(n), glm::vec3(t)), glm::vec3(tan2[a])) < 0.0F) ? 1.0F : -1.0F;

			// calculate bitangent (ignoring for our Vertex, here just for reference)
			//vertices[a].bitangent = glm::vec4(glm::cross(glm::vec3(vertices[a].normal), glm::vec3(vertices[a].tangent)) * vertices[a].tangent.w, 0);
			//vertices[a].tangent.w = 0;
		}

		//// Store the amount of vertices.
		//uint vertex_count = (uint)a_vertices.size();
		//
		//// Allocate storage for the tangents and bitangents.
		//glm::vec4* tangent = new glm::vec4[vertex_count * 2];
		//glm::vec4* bitangent = tangent + vertex_count;
		//
		//// Initialize the tangent and bitangent vectors to zero.
		//for (size_t i = 0; i < vertex_count; i++)
		//{
		//	tangent[i] = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);
		//	bitangent[i] = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);
		//}
		//
		//// Calculate tangent and bitangent for each triangle.
		//for (size_t i = 0; i < a_index_count; i += 3)
		//{
		//	long index0 = a_indices[i];
		//	long index1 = a_indices[i + 1];
		//	long index2 = a_indices[i + 2];
		//	
		//	const glm::vec3& point0 = a_vertices[index0].m_position;
		//	const glm::vec3& point1 = a_vertices[index1].m_position;
		//	const glm::vec3& point2 = a_vertices[index2].m_position;
		//	
		//	const glm::vec2& w0 = a_vertices[index0].m_tex_coords;
		//	const glm::vec2& w1 = a_vertices[index1].m_tex_coords;
		//	const glm::vec2& w2 = a_vertices[index2].m_tex_coords;
		//	
		//	glm::vec3 e1 = point1 - point0, e2 = point2 - point0;
		//	float x1 = w1.x - w0.x, x2 = w2.x - w0.x;
		//	float y1 = w1.y - w0.y, y2 = w2.y - w0.y;
		//	
		//	float r = 1.0f / (x1 * y2 - x2 * y1);
		//	glm::vec3 t = (e1 * y2 - e2 * y1) * r;
		//	glm::vec3 b = (e2 * x1 - e1 * x2) * r;
		//	
		//	tangent[index0] += glm::vec4(t, 0.0f);
		//	tangent[index2] += glm::vec4(t, 0.0f);
		//	tangent[index1] += glm::vec4(t, 0.0f);
		//	
		//	bitangent[index0] += glm::vec4(b, 0.0f);
		//	bitangent[index2] += glm::vec4(b, 0.0f);
		//	bitangent[index1] += glm::vec4(b, 0.0f);
		//}
		//
		//// Orthonormalize each tanget and calculate handedness.
		//for (size_t i = 0; i < vertex_count; i++)
		//{
		//	const glm::vec3& t = tangent[i];
		//	const glm::vec3& b = bitangent[i];
		//	const glm::vec3& n = a_vertices[i].m_normal;
		//
		//	// Gram-Schmidt orthogonalize.
		//	a_vertices[i].m_tangent = glm::vec4(glm::normalize(t - n * glm::dot(n, t)), 0.0f);
		//
		//	// Calculate handedness (direction of bitangent).
		//	a_vertices[i].m_tangent.w = (glm::dot(glm::cross(t, b), n) > 0.0f) ? 1.0f : -1.0f;
		//
		//	//printf("tan0: %i | tan1: %i | tan2: %i | tan3: %i", a_vertices[i].m_tangent.x, a_vertices[i].m_tangent.y, a_vertices[i].m_tangent.z, a_vertices[i].m_tangent.w);
		//	//printf("\n");
		//}
		
		// Clean memory
		delete[] tan1;
		//delete[] tangent;
	}

	void Mesh::setup_base_mesh(GLuint a_vao, GLuint a_vbo, GLuint a_ebo, const uint* a_indices)
	{
		// If the geometry was passed by the user,
		// then the vao/vbo/ebo was not generated within
		// the load_obj() call (because it was not necessary to call).
		// Generate geometry now in the setup call.
		if (m_is_geometry_user_defined)
		{
			glGenVertexArrays(1, &m_VAO);
			glGenBuffers(1, &m_VBO);
			glGenBuffers(1, &m_EBO);
		}
		else
		{
			// Else this function was called via load_obj, therefore
			// the geometry was not created by the user and instead a
			// .obj file. So set the vao/vbo/ebo to the current mesh/submeshes
			// generated ID's.
			m_VAO = a_vao;
			m_VBO = a_vbo;
			m_EBO = a_ebo;
		}

		// Bind the vao.
		glBindVertexArray(m_VAO);

		// Bind the vbo.
		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
		// Attach the vertex data to the VBO
		glBufferData(GL_ARRAY_BUFFER, sizeof(base_vertex) * m_base_vert.size(), &m_base_vert[0], GL_STATIC_DRAW);

		// Bind the ebo.
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
		// Attach the indices data to the EBO
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * m_index_count, &a_indices[0], GL_STATIC_DRAW);

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

	void Mesh::setup_standard_mesh(GLuint a_vao, GLuint a_vbo, GLuint a_ebo, const uint* a_indices)
	{
		// If the geometry was passed by the user,
		// then the vao/vbo/ebo was not generated within
		// the load_obj() call (because it was not necessary to call).
		// Generate geometry now in the setup call.
		if (m_is_geometry_user_defined)
		{
			glGenVertexArrays(1, &m_VAO);
			glGenBuffers(1, &m_VBO);
			glGenBuffers(1, &m_EBO);
		}
		else
		{
			// Else this function was called via load_obj, therefore
			// the geometry was not created by the user and instead a
			// .obj file. So set the vao/vbo/ebo to the current mesh/submeshes
			// generated ID's.
			m_VAO = a_vao;
			m_VBO = a_vbo;
			m_EBO = a_ebo;
		}

		// Bind the vao.
		glBindVertexArray(a_vao);

		// Bind the vbo.
		glBindBuffer(GL_ARRAY_BUFFER, a_vbo);
		// Attach the vertex data to the VBO
		glBufferData(GL_ARRAY_BUFFER, sizeof(standard_vertex) * m_standard_vert.size(), &m_standard_vert[0], GL_STATIC_DRAW);

		// Bind the ebo.
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, a_ebo);
		// Attach the indices data to the EBO
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * m_index_count, &a_indices[0], GL_STATIC_DRAW);

		// Create and enable the attribute for the position
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, sizeof(standard_vertex), (GLvoid*)0);
		// Create and enable the attribute for the normals
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(standard_vertex), (GLvoid*)offsetof(standard_vertex, m_normal));
		// Create and enable the attribute for the tangents
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, sizeof(standard_vertex), (GLvoid*)offsetof(standard_vertex, m_tangent));
		
		// Create and enable the attribute for the texels (U/V)
		glEnableVertexAttribArray(3);
		glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(standard_vertex), (GLvoid*)offsetof(standard_vertex, m_tex_coords));

		// Unbind the VBO, EBO, and VAO
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}

	void Mesh::setup_full_mesh(GLuint a_vao, GLuint a_vbo, GLuint a_ebo, const uint* a_indices)
	{
		// If the geometry was passed by the user,
		// then the vao/vbo/ebo was not generated within
		// the load_obj() call (because it was not necessary to call).
		// Generate geometry now in the setup call.
		if (m_is_geometry_user_defined)
		{
			glGenVertexArrays(1, &m_VAO);
			glGenBuffers(1, &m_VBO);
			glGenBuffers(1, &m_EBO);
		}
		else
		{
			// Else this function was called via load_obj, therefore
			// the geometry was not created by the user and instead a
			// .obj file. So set the vao/vbo/ebo to the current mesh/submeshes
			// generated ID's.
			m_VAO = a_vao;
			m_VBO = a_vbo;
			m_EBO = a_ebo;
		}

		// Bind the vao.
		glBindVertexArray(m_VAO);

		// Bind the vbo.
		glBindBuffer(GL_ARRAY_BUFFER, m_VBO);
		// Attach the vertex data to the VBO
		glBufferData(GL_ARRAY_BUFFER, sizeof(full_vertex) * m_full_vert.size(), &m_full_vert[0], GL_STATIC_DRAW);

		// Bind the ebo.
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_EBO);
		// Attach the indices data to the EBO
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(uint) * m_index_count, &a_indices[0], GL_STATIC_DRAW);

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

	void create_texture_maps(tinyobj::material_t a_material, bool a_should_textures_flip, const char* a_material_name)
	{
		if (a_material.alpha_texname != "\0")
		{
			// If the model creator passed in their entire system as the 
			// texture file path, handle removing all the junk data.
			int location = std::string(a_material.alpha_texname).rfind("\\");
			std::string new_file_name = std::string(a_material.alpha_texname).substr(location + 1);
			std::string new_texture_path = std::string("..//Textures//" + new_file_name);

			// Pass the texture file data.
			TextureManager& texture = TextureManager::inst();
			texture.create_texture("alpha_map", new_texture_path.c_str());
			MaterialManager::inst().add_loaded_map(a_material_name, texture.get_texture("alpha_map"), material_map_type::ALPHA_MAP);
		}

		if (a_material.ambient_texname != "\0")
		{
			// If the model creator passed in their entire system as the 
			// texture file path, handle removing all the junk data.
			int location = std::string(a_material.ambient_texname).rfind("\\");
			std::string new_file_name = std::string(a_material.ambient_texname).substr(location + 1);
			std::string new_texture_path = std::string("..//Textures//" + new_file_name);

			// Pass the texture file data.
			TextureManager& texture = TextureManager::inst();
			texture.create_texture("ambient_map", new_texture_path.c_str());
			MaterialManager::inst().add_loaded_map(a_material_name, texture.get_texture("ambient_map"), material_map_type::AMBIENT_MAP);
		}

		if (a_material.bump_texname != "\0")
		{
			// If the model creator passed in their entire system as the 
			// texture file path, handle removing all the junk data.
			int location = std::string(a_material.bump_texname).rfind("\\");
			std::string new_file_name = std::string(a_material.bump_texname).substr(location + 1);
			std::string new_texture_path = std::string("..//Textures//" + new_file_name);
			
			// Pass the texture file data.
			TextureManager& texture = TextureManager::inst();
			texture.create_texture("bump_map", new_texture_path.c_str());
			MaterialManager::inst().add_loaded_map(a_material_name, texture.get_texture("bump_map"), material_map_type::BUMP_MAP);
		}

		if (a_material.diffuse_texname != "\0")
		{
			// If the model creator passed in their entire system as the 
			// texture file path, handle removing all the junk data.
			int location = std::string(a_material.diffuse_texname).rfind("\\");
			std::string new_file_name = std::string(a_material.diffuse_texname).substr(location + 1);
			std::string new_texture_path = std::string("..//Textures//" + new_file_name);

			// Pass the texture file data.
			TextureManager& texture = TextureManager::inst();
			texture.create_texture("diffuse_map", new_texture_path.c_str());
			MaterialManager::inst().add_loaded_map(a_material_name, texture.get_texture("diffuse_map"), material_map_type::DIFFUSE_MAP);
		}

		if (a_material.displacement_texname != "\0")
		{
			// If the model creator passed in their entire system as the 
			// texture file path, handle removing all the junk data.
			int location = std::string(a_material.displacement_texname).rfind("\\");
			std::string new_file_name = std::string(a_material.displacement_texname).substr(location + 1);
			std::string new_texture_path = std::string("..//Textures//" + new_file_name);

			// Pass the texture file data.
			TextureManager& texture = TextureManager::inst();
			texture.create_texture("displacement_map", new_texture_path.c_str());
			MaterialManager::inst().add_loaded_map(a_material_name, texture.get_texture("displacement_map"), material_map_type::DISPLACEMENT_MAP);
		}

		if (a_material.specular_highlight_texname != "\0")
		{
			// If the model creator passed in their entire system as the 
			// texture file path, handle removing all the junk data.
			int location = std::string(a_material.specular_highlight_texname).rfind("\\");
			std::string new_file_name = std::string(a_material.specular_highlight_texname).substr(location + 1);
			std::string new_texture_path = std::string("..//Textures//" + new_file_name);

			// Pass the texture file data.
			TextureManager& texture = TextureManager::inst();
			texture.create_texture("specular_highlight_map", new_texture_path.c_str());
			MaterialManager::inst().add_loaded_map(a_material_name, texture.get_texture("specular_highlight_map"), material_map_type::SPECULAR_HIGHLIGHT_MAP);
		}

		if (a_material.specular_texname != "\0")
		{
			// If the model creator passed in their entire system as the 
			// texture file path, handle removing all the junk data.
			int location = std::string(a_material.specular_texname).rfind("\\");
			std::string new_file_name = std::string(a_material.specular_texname).substr(location + 1);
			std::string new_texture_path = std::string("..//Textures//" + new_file_name);

			// Pass the texture file data.
			TextureManager& texture = TextureManager::inst();
			texture.create_texture("specular_map", new_texture_path.c_str());
			MaterialManager::inst().add_loaded_map(a_material_name, texture.get_texture("specular_map"), material_map_type::SPECULAR_MAP);
		}
	}

	void Mesh::clear_mesh()
	{
		if (m_is_geometry_user_defined)
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
		else
		{
			for (auto& sub_mesh : sub_meshes)
			{
				// Clear all the buffers off the graphics card and reset the IDs
				if (sub_mesh.m_VAO)
				{
					glDeleteVertexArrays(1, &sub_mesh.m_VAO);
					sub_mesh.m_VAO = 0;
				}

				if (sub_mesh.m_VBO)
				{
					glDeleteBuffers(1, &sub_mesh.m_VBO);
					sub_mesh.m_VBO = 0;
				}

				if (sub_mesh.m_EBO)
				{
					glDeleteBuffers(1, &sub_mesh.m_EBO);
					sub_mesh.m_EBO = 0;
				}

				// Reset the index count
				sub_mesh.m_index_count = 0;
			}
		}
	}
}