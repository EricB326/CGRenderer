#ifndef MESH_H
#define MESH_H

// Standard includes
#include <iostream>
#include <vector>
// Graphics includes
#include "gl_core_4_5.h"
// Math includes
#include "glm.hpp"
// User defined includes
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
struct vertex
{
	/* @brief Position of vertex.
	*/
	glm::vec3 m_position;

	/* @brief Normals of vertex.
	*/
	glm::vec3 m_normal;

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

/* @brief Handles the creation and rendering of vertices in
   the renderer. 
*/
class Mesh
{
public:
	/******************************************************/
	// Functions
	/* @brief Constructor zeros all values with no params.
		Resulting in no data being passed to the vertex structs.
	   @param The data stored in the vertices array.
	   @param The data used to connect the points in the meshes array.
	   @param The number of vertices passed through.
	   @param The number of indices in the index array
	   @param The type of vertex being created, 0 = BASIC_VERTEX | 1 = STANDARD_VERTEX (see "common.h").
	*/
	Mesh(GLfloat* a_vertices, uint* a_indices, uint a_num_of_verts, uint a_num_of_indices, int a_vertex_type);
	~Mesh();

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

private:
	/******************************************************/
	// Variables
	/* @brief Data required for rendering.
	*/
	GLuint m_VAO, m_VBO, m_EBO;

	/* @brief Declareation of the basic_vertex struct, to
		prepare for initialized data.
	*/
	std::vector<base_vertex> m_basic_verticies;

	/* @brief Declareation of the vertex struct, to
		prepare for initialized data.
	*/
	std::vector<vertex> m_verticies;

	/* @brief The data passed through from the index
		array.
	*/
	uint* m_indices;

	/* @brief Number of indicies passed to
		the class.
	*/
	GLsizei m_index_count;

	/* @brief Number of verts passed to
		the class.
	*/
	GLsizei m_vert_count;

	/******************************************************/
	// Functions
	/* @brief Initializes all the array/buffer objects to be
	    ready for rendering the mesh.
	*/
	void setup_mesh();

	/* @brief Clears all the buffers and resets the
		array/vertex objects.
	*/
	void clear_mesh();

};


#endif // !MESH_H