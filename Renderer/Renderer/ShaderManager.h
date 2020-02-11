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
	class ShaderManager
	{
	public:
		ShaderManager() {}
		~ShaderManager() { clean_manager(); }

		void load_shader(const std::string& a_key_name, shader_type a_type_of_shader, const std::string& a_filepath);

		void create_shader_program(const std::string& a_key_name, std::vector<std::string> a_shader_list);

		void use_program(const std::string& a_program_key);

		uint get_program_id(const std::string& a_program_key);

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
		bool does_shader_key_already_exist(const std::string& a_key);

		bool verify_shader_map_key(const std::string& a_key);

		bool does_program_key_already_exist(const std::string& a_key);

		bool verify_program_map_key(const std::string& a_key);

		void clean_manager();
	};
}

#endif // !SHADER_MANAGER_H