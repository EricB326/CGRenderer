#ifndef SHADER_H
#define SHADER_H

/* Standard lib includes
*/
#include <string>
#include <vector>
#include <map>
#include <fstream>

/* Graphic includes
*/
#include "gl_core_4_5.h"
#include "glfw3.h"

/* User defined includes
*/
#include "common.h"

/* @brief Data for the different types of shaders that
	can be created.
*/
enum class shader_type : uint
{
	VERTEX,
	TESSELLATION_EVALUATION,
	TESSELLATION_CONTROL,
	GEOMETRY,
	FRAGMENT
};

class Shader
{
public:
	/******************************************************/
	// Functions
	/* @brief Constructor zeros all values. Expects the user
		to later call load_from_file().
	*/
	Shader() : m_shader_id(0) {}

	/* @brief Constructor zeros all values. Calls for the user
		load_from_file() passing the data recieved.
	   @param Type of shader that will be passed to load.
	   @param File path of the shader that will be passed to load.
	*/
	Shader(const shader_type a_type_of_shader, const std::string& a_filepath);

	/* @brief Loads, reads, and creates shaders of the correct type
		with the data passed in from the user.
	   @param The type of shader being loaded. Ie, vert, frag, etc.
	   @param String of the filename/location to read data from.
	*/
	void load_from_file(const shader_type a_type_of_shader, const std::string& a_filepath);

	/* @brief Retrives the shader id.
	   @return Returns a unit of the shader id
	*/
	uint get_shader_id() { return m_shader_id; }

private:
	/******************************************************/
	// Variables
	/* @brief Stores the id of the created program.
	*/
	uint m_shader_id;

	/******************************************************/
	// Functions
	/* @brief Opens a file stream to read the data within the file location.
		stores that data into a buffer for later use.
	   @param String for the path of the file being read from.
	   @param String for the buffer have the data written too.
	   @return Retuns true if the data was successfully read, false
		if the data could not be read.
	*/
	bool read_file_content(const std::string& a_filepath, std::string& a_string_buffer);

	/* @brief Checks if the data within a file is empty or not.
	   @param File stream being checked for content.
	   @return Retuns true if it is empty, false if not.
	*/
	bool is_file_empty(std::ifstream& a_file_content);

	/* @brief Creates any type of shader the user passes in via the
		data passed in.
	   @param The data within the shader file.
	   @param The type of shader being created on the GPU. Ie, vertex, frag, etc.
	*/
	void create_shader(const std::string& a_file_content, GLenum a_type_of_shader);
};

#endif // !SHADER_H