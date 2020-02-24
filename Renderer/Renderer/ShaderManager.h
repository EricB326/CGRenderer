#ifndef SHADER_MANAGER_H
#define SHADER_MANAGER_H

/* Standard lib includes
*/
#include <string>
#include <vector>
#include <map>

/* Graphic includes
*/
#include "gl_core_4_5.h"
#include "glfw3.h"

/* User defined includes
*/
#include "common.h"
#include "Shader.h"
#include "ShaderProgram.h"

namespace uciniti 
{
	enum class uniform_type : uint
	{
		UNIFORM_1f,
		UNIFORM_1i,
		UNIFORM_1d,
		UNIFORM_1ui,

		UNIFORM_MATRIX_2fv,
		UNIFORM_MATRIX_3fv,
		UNIFORM_MATRIX_4fv,
	};

	class ShaderManager
	{
	public:
		/******************************************************/
		// Functions
		/* @brief Base constructor.
		*/
		ShaderManager() {}
		
		/* @brief Cleans up the shader manager on destruction.
		*/
		~ShaderManager() { clean_manager(); }

		/* @brief Allows the user to identify shaders they wanna load by
			string. Calls the Shader class 
		   @param String of what the user would like to store the shader as
			in the map.
		   @param The type of shader being loaded. Ie, vert, frag, etc.
		   @param String of the filename/location to read data from.
		*/
		bool load_shader(const std::string& a_key_name, shader_type a_type_of_shader, const std::string& a_filepath);
		
		/* @brief Creates a shader program and attaches shaders via the
			map keys to it. Finally links it all.
		   @param Vector of Strings that store the different keys to search
			within the map when attaching shaders to the program
		*/
		bool create_shader_program(const std::string& a_key_name, std::vector<std::string> a_shader_list);

		void bind_uniform_data(const uint a_program_id, const char* a_uniform_location, const int a_count, const float a_data_to_pass, const uniform_type a_type_to_bind);
		void bind_uniform_matrix_data(const uint a_program_id, const char* a_uniform_location, const int a_count, bool a_should_transpose, const float* a_data_to_pass, const uniform_type a_type_to_bind);

		/* @brief Uses the shader program for the next draw call.
		   @param The key of the shader program the user would like to use.
		*/
		void use_program(const std::string& a_program_key);

		/* TODO: Comment. */
		void reload_shader_program(const std::string& a_program_key, const std::vector<std::string> a_shader_list);

		/* @brief Retrives the program ID.
		   @param Key of the shader program the user would like to key of.
		   @return Returns a uint with the ID.
		*/
		inline uint get_program_id(const std::string& a_program_key) { return m_program_list.at(a_program_key)->get_program_id(); }

	private:
		/******************************************************/
		// Variables
		/* @brief List of compiled shaders mapping user created identifier names
			to the created Shader().
		*/
		std::map<std::string, Shader*> m_shader_list;

		/* @brief List of compiled shaders mapping user created identifier names
			to the created ShaderProgram().
		*/
		std::map<std::string, ShaderProgram*> m_program_list;

		/******************************************************/
		// Functions
		/* @brief Checks if the key passed already exists within the
			shader map.
		   @param The key being searched within the map.
		   @return True if the key exists, false if not.
		*/
		bool does_shader_key_already_exist(const std::string& a_key);

		/* @brief Checks if the key passed already exists within the
			program map.
		   @param The key being searched within the map.
		   @return True if the key exists, false if not.
		*/
		bool does_program_key_already_exist(const std::string& a_key);

		void delete_program_at_key(const std::string& a_program_key);

		/* @brief Cleans the shader manager.
		*/
		void clean_manager();
	};
}

#endif // !SHADER_MANAGER_H