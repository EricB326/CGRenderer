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

/* @brief Enum class of the different types of vertex structs
	can be created. Data is stored as unsigned ints.*/
enum class vertex_type : uint
{
	BASE_VERTEX,
	STANDARD_VERTEX,
	FULL_VERTEX
};

/* @brief Struct which is used as a holder for crucial rendering
	data for any sub meshes that may be loaded with a .obj file.
*/
struct sub_mesh_data
{
	/* @brief Data required for rendering.
	*/
	GLuint m_VAO, m_VBO, m_EBO;

	/* @brief The data passed through from the index
	array.
	*/
	const uint* m_indices;

	/* @brief Number of indicies and verts passed to
		the class.
	*/
	GLsizei m_index_count, m_vert_count;
};

namespace uciniti
{
	/* @brief Handles the creation and rendering of vertices in
		the engine.
	*/
	class Mesh
	{
	public:
		/******************************************************/
		// Functions
		/* @brief Constructor zeros all values with no params.
		*/
		Mesh();

		/* @brief Constructs the mesh to be rendered via a loaded .obj file.
			Passes the filepath over to a load function to handle reading the
			data. Also handles setting up materials, textures, and if the textures
			should be flipped.
		   @param Const char* filepath of the .obj file being loaded.
		   @param Const char* user made name to use as a material identifier.
		   @param Bool on if the load_obj() function called should load textures as well.
		   @param Bool on if the loaded textures should be flipped.
		*/
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

		/* @brief Clears all the data stored within the class. Such as; array objects, buffer objetcs,
			etc.
		*/
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
		bool load_obj(const char* a_filepath, const char* a_material_name, bool a_load_textures = true, bool a_should_textures_flip = false);

	private:
		/******************************************************/
		// Variables
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

		/* @brief Holds multiple (if required) sub meshes with their own respective
			data crucial for rendering. IE, VAO, VBO, EBO, indicies, etc.
		*/
		std::vector<sub_mesh_data> sub_meshes;

		/* @brief Checks to see if the mesh is currently occupied
			by another model. True if empty, false if occupied.
		*/
		bool m_empty_mesh;

		/* @brief Sets to true if the geometry is being passed
			by the user. False if it is loaded via files.
		*/
		bool m_is_geometry_user_defined;

		/* @brief Data necessary for rendering geometry. Used
				  for user passed data.
		*/
		GLuint m_VAO, m_VBO, m_EBO, m_index_count;

		/******************************************************/
		// Functions
		/* @brief Handles calculating the tangents of each vertex in the
			mesh. Also orthonormalizes each tangent and calculates the
			handedness.
		*/
		void calculate_vertex_tangents(std::vector<standard_vertex>& a_vertices, const uint* a_indices, const GLsizei a_index_count);

		/* @brief Initializes all the base vertex array/buffer objects
			to be ready for rendering the mesh.
		*/
		void setup_base_mesh(GLuint a_vao, GLuint a_vbo, GLuint a_ebo, const uint* a_indices);

		/* @brief Initializes all the standard vertex array/buffer objects
			to be ready for rendering the mesh.
		*/
		void setup_standard_mesh(GLuint a_vao, GLuint a_vbo, GLuint a_ebo, const uint* a_indices);

		/* @brief Initializes all the full vertex array/buffer objects
			to be ready for rendering the mesh.
		*/
		void setup_full_mesh(GLuint a_vao, GLuint a_vbo, GLuint a_ebo, const uint* a_indices);

		/* @brief Clears all the buffers and resets the
			array/vertex objects.
		*/
		void clear_mesh();
	};
}

#endif // !MESH_H