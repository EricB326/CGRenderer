#ifndef SHADER_MANAGER_H
#define SHADER_MANAGER_H

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

class ShaderManager
{
public:
	/******************************************************/
	// Functions
	/* @brief Constructor zeros all values.
	*/
	ShaderManager() : m_shader_program_id(0) {};

	/* @brief Destructor that will clean the shader program
	*/
	~ShaderManager() { clean_shader(); };

	/* @brief Loads, reads, and creates shaders of the correct type
		with the data passed in from the user. 
	   @param String of what the user would like to store the shader as
	    in the map.
	   @param String of the filename/location to read data from.
	   @param The type of shader being loaded. Ie, vert, frag, etc.
	   @return Bool that returns true if the file was loaded successfully,
	    or false if it failed to load.
	*/
	bool load_from_file(const std::string a_shader_name, const std::string& a_filename, shader_type a_type);

	/* @brief Creates a shader program and attaches shaders via the
	    map keys to it. Finally links it all.
	   @param Vector of Strings that store the different keys to search
		within the map when attaching shaders to the program
	*/
	void create_shader_program(const std::vector<std::string>& a_key_list);

	/* @brief Retrives a shader ID of a shader stored within the 
		m_shader_list map.
	   @param String of the name (key) being searched within the
	    map.
	   @return Returns a uint of the shader ID stored at that key
	    or a -1 to imply and error when searching. 
	*/
	uint get_shader(const std::string& a_key);

	/* @brief Retrives the shader program ID stored.
	   @return Unsigned int of the ID from the GPU.
	*/
	uint get_program() { return m_shader_program_id; }

	/* @brief Uses the shader program for the next draw call. 
	*/
	void use_shader();

	/* @brief Cleans the shader program and resets values.
	*/
	void clean_shader();

private:
	/******************************************************/
	// Variables
	/* @brief Stores the id of the created program.
	*/
	uint m_shader_program_id;

	/* @brief List of compiled shaders mapping user created identifier names
		to return GPU ID.
	*/
	std::map<std::string, uint> m_shader_list;

	/* @brief Map iterator used for searching through the shader list map
		for specified data.
	*/
	std::map<std::string, uint>::iterator m_shader_list_iterator;

	/******************************************************/
	// Functions
	/* @brief Creates any type of shader the user passes in via the
		data passed in.
	   @param The data within the shader file to be sent to the GPU.
	   @param The type of shader being created on the GPU. Ie, vertex, frag, etc.
	   @return Returns the recieved shader ID from the GPU.
	*/
	GLuint create_shader(const std::string& const a_shader_data, GLenum a_type);

	/* @brief Opens a file stream to read the data within the file location.
	    stores that data into a buffer for later use.
	   @param String for the name/location of the file being read from.
	   @param String for the buffer have the data written too.
	   @return Retuns true if the data was successfully read, false
	    if the data could not be read. 
	*/
	bool read_file_data(const std::string& a_filename, std::string& a_shader_buffer);

	/* @brief Checks if the data within a file is empty or not. 
	   @param File stream being checked for content.
	   @return Retuns true if it is empty, false if not. 
	*/
	bool file_is_empty(std::ifstream& a_file_content);

	/* @brief Used to check if a shader was successfully mapped. 
	   @param The key to be searched for in the map.
	*/
	void mapping_check(const std::string& a_key);
};

#endif // !SHADER_MANAGER_H