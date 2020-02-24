#ifndef MESH_H
#define MESH_H

/* Standard lib includes
*/
#include <iostream>
#include <vector>

/* Graphic includes
*/
#include "gl_core_4_5.h"

/* Math includes
*/
#include "glm.hpp"

/* User defined includes
*/
#include "common.h"

/* @brief Struct for storing data of a smaller and simplier
    vertex being created.
*/
struct base_vertex
{
	/* @brief Position of vertex.
	*/
	glm::vec3 m_position;

	/* @brief Colour of vertex.
	*/
	glm::vec3 m_colour;
};

/* @brief Struct for storing data of a standard vertex
    with commonly used data.
*/
struct standard_vertex
{
	/* @brief Position of vertex.
	*/
	glm::vec4 m_position;

	/* @brief Normals of vertex.
	*/
	glm::vec4 m_normal;

	/* @brief Tangents of vertex.
	*/
	glm::vec4 m_tangent;

	/* @brief Bitangents of vertex.
	*/
	glm::vec4 m_bitangent;

	/* @brief Texture coords of vertex.
	*/
	glm::vec2 m_tex_coords;
};

/* @brief Struct for storing data of a full vertex
	with multiple data types
*/
struct full_vertex
{
	/* @brief Position of vertex.
	*/
	glm::vec3 m_position;

	/* @brief Colour of vertex.
	*/
	glm::vec3 m_colour;

	/* @brief Normals of vertex.
	*/
	glm::vec3 m_normal;

	/* @brief Tangent of vertex.
	*/
	glm::vec4 m_tangent;

	/* @brief First set of texture coords of vertex.
	*/
	glm::vec2 m_tex_coords001;

	/* @brief Second set of texture coords of vertex.
	*/
	glm::vec2 m_tex_coords002;

	/* @brief Third set of texture coords of vertex.
	*/
	glm::vec2 m_tex_coords003;
};

enum class vertex_type : uint
{
	BASE_VERTEX,
	STANDARD_VERTEX,
	FULL_VERTEX
};

/* @brief Handles the creation and rendering of vertices in
   the renderer. 
*/
namespace uciniti
{
	class Mesh
	{
	public:
		/******************************************************/
		// Functions
		/* @brief Constructor zeros all values with no params.
		*/
		Mesh();

		/* TODO: Comment. */
		Mesh(const char* a_filepath, const char* a_material_name, bool a_load_textures = true, bool a_flip_textures = false);

		/* @brief Constructor zeros all values with no params.
			Resulting in no data being passed to the vertex structs.
		   @param The data stored in the vertices array.
		   @param The data used to connect the points in the meshes array.
		   @param The number of vertices passed through.
		   @param The number of indices in the index array
		   @param The type of vertex being created, 0 = BASIC_VERTEX | 1 = STANDARD_VERTEX (see "common.h").
		*/
		Mesh(GLfloat* a_vertices, uint* a_indices, uint a_num_of_verts, uint a_num_of_indices, vertex_type a_vertex_type);

		/* TODO: Comment. */
		~Mesh() { clear_mesh(); }

		/* @brief Renders the prepared mesh data
		*/
		void render_mesh();

		/* @brief Another way for users to render,
			calls render_mesh().
		*/
		void render();

		/* @brief Another way for users to render,
			calls render_mesh().
		*/
		void draw();

		/* @brief Handles calling tinyobj to import .obj files.
		   @param File path of the .obj file.
		   @param Bool on if textures should be loaded.
		   @param Should V textures be flipped. 
		   @return True if successfully loaded, false if error occured.
		*/
		bool load_obj(const char* a_filepath, const char* a_material_name, bool a_load_textures = true, bool a_flip_textures = false);

	private:
		/******************************************************/
		// Variables
		/* @brief Data required for rendering.
		*/
		GLuint m_VAO, m_VBO, m_EBO;

		/* @brief Declareation of the base_vertex struct, to
			prepare for initialized data.
		*/
		std::vector<base_vertex> m_base_vert;

		/* @brief Declareation of the standard_vertex struct, to
			prepare for initialized data.
		*/
		std::vector<standard_vertex> m_standard_vert;

		/* @brief Declareation of the full_vertex struct, to
			prepare for initialized data.
		*/
		std::vector<full_vertex> m_full_vert;

		/* @brief The data passed through from the index
			array.
		*/
		const uint* m_indices;

		/* @brief Number of indicies and verts passed to
			the class.
		*/
		GLsizei m_index_count, m_vert_count;

		/* @brief Checks to see if the mesh is currently occupied
			by another model. True if empty, false if occupied.
		*/
		bool m_empty_mesh;

		/******************************************************/
		// Functions
		/* TODO: Comment. */
		void calculate_vertex_tangents(std::vector<standard_vertex>& a_vertices, const uint* a_indices);

		/* @brief Initializes all the base vertex array/buffer objects
			to be ready for rendering the mesh.
		*/
		void setup_base_mesh();

		/* @brief Initializes all the standard vertex array/buffer objects
			to be ready for rendering the mesh.
		*/
		void setup_standard_mesh();

		/* @brief Initializes all the full vertex array/buffer objects
			to be ready for rendering the mesh.
		*/
		void setup_full_mesh();

		/* @brief Clears all the buffers and resets the
			array/vertex objects.
		*/
		void clear_mesh();

	};
}

#endif // !MESH_H