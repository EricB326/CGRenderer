/* Standard includes
*/
#include <iostream>
#include <sstream>

/* User defined includes
*/
#include "ShaderManager.h"

namespace uciniti
{
	bool ShaderManager::load_shader(const std::string& a_key_name, shader_type a_type_of_shader, const std::string& a_filepath)
	{
		// If the shader already exists, return.
		if (does_shader_key_already_exist(a_key_name))
		{
			printf("\nERROR: load_shader() call. Key: '%s' already exists in the shader map!\n", a_key_name.c_str());
			return false;
		}
	
		// Create and load the desired shader.
		Shader* new_mapped_shader = new Shader();
		new_mapped_shader->load_from_file(a_type_of_shader, a_filepath);
	
		// Create a new map key with the created shader.
		m_shader_list[a_key_name] = new_mapped_shader;

		return true;
	}

	bool ShaderManager::create_shader_program(const std::string& a_key_name, std::vector<std::string> a_shader_list)
	{
		// If the program already exists, return.
		if (does_program_key_already_exist(a_key_name))
		{
			printf("\nERROR: create_shader_program() call. Key: '%s' already located in map!. Create a new key name.\n", a_key_name.c_str());
			return false;
		}

		// Create the new shader program.
		ShaderProgram* new_mapped_program = new ShaderProgram();
		std::vector<uint> shader_id_list;

		// Loop through each identifier.
		for (size_t i = 0; i < a_shader_list.size(); i++)
		{
			// Verify there is a shader at the identifier passed.
			if (!does_shader_key_already_exist(a_shader_list[i]))
			{
				printf("ERROR: create_shader_program() call. Cannot find key: '%s' to attach!\n", a_key_name.c_str());
				return false;
			}

			// Push into the uint vector the uint mapped with the identifier passed.
			shader_id_list.push_back(m_shader_list.at(a_shader_list[i])->get_shader_id());
		}

		// Verify there are shader ID's to pass.
		if (shader_id_list.size() <= 0)
		{
			printf("ERROR: create_shader_program() call. Could not find any shader ID's!\n");
			return false;
		}

		// Create the shader program.
		new_mapped_program->create_shader_program(shader_id_list);

		// Map the ShaderProgram* to the program map with the key name given.
		m_program_list[a_key_name] = new_mapped_program;

		return true;
	}

	void ShaderManager::use_program(const std::string& a_program_key)
	{
		// Verify the program key provided.
		if (!does_program_key_already_exist(a_program_key))
		{
			printf("\nERROR: use_program() call. Cannot find program: '%s'!", a_program_key.c_str());
			return;
		}

		m_program_list.at(a_program_key)->use_program();
	}

	bool ShaderManager::does_shader_key_already_exist(const std::string& a_key)
	{
		// Using the .find() search for the key passed through.
		// The iterator stores the address of the key value pair.
		auto shader_list_iterator = m_shader_list.find(a_key);
	
		// If we reached the end of the m_shader_list, the key was
		// not found in the map.
		if (shader_list_iterator == m_shader_list.end())
			return false; // Return false, shader key doesn't exist.
		else
			return true; // Return true, shader key was found.
	}

	bool ShaderManager::does_program_key_already_exist(const std::string& a_key)
	{
		// Using the .find() search for the key passed through.
		// The iterator stores the address of the key value pair.
		auto program_list_iterator = m_program_list.find(a_key);

		// If we reached the end of the m_program_list, the key was
		// not found in the map.
		if (program_list_iterator == m_program_list.end())
			return false; // Return false, program key not verified.
		else
			return true; // Return true, program key was found.
	}

	void ShaderManager::delete_program_at_key(const std::string& a_program_key)
	{
		// Attempt to search for the program key passed.
		std::map<std::string, ShaderProgram*>::iterator map_iterator = m_program_list.find(a_program_key);

		// If the program key was found, delete the shader program at the key.
		if (map_iterator != m_program_list.end())
		{
			delete map_iterator->second;
			map_iterator->second = nullptr;

			m_program_list.erase(map_iterator);
		}
	}

	void ShaderManager::bind_uniform_vector_data(const uint a_program_id, const char* a_uniform_location, const uniform_type a_type_to_bind, const float* a_data_to_pass)
	{
		// Switches what type of data is being bound based on the uniform_type passed.
		switch (a_type_to_bind)
		{
		case uciniti::uniform_type::UNIFORM_3f:
			glUniform3f(glGetUniformLocation(a_program_id, a_uniform_location), a_data_to_pass[0], a_data_to_pass[1], a_data_to_pass[2]);
			break;
		case uciniti::uniform_type::UNIFORM_4f:
			glUniform4f(glGetUniformLocation(a_program_id, a_uniform_location), a_data_to_pass[0], a_data_to_pass[1], a_data_to_pass[2], a_data_to_pass[3]);
			break;
		default:
			printf("ERROR: bind_uniform_data() call. Could not find matrix uniform type passed.\n");
			break;
		}
	}

	void ShaderManager::bind_uniform_matrix_data(const uint a_program_id, const char* a_uniform_location, const uniform_type a_type_to_bind, const int a_count, const float* a_data_to_pass, bool a_should_transpose)
	{
		// Switches what type of data is being bound based on the uniform_type passed.
		switch (a_type_to_bind)
		{
		case uciniti::uniform_type::UNIFORM_MATRIX_2fv:
			glUniformMatrix2fv(glGetUniformLocation(a_program_id, a_uniform_location), a_count, a_should_transpose, a_data_to_pass);
			break;
		case uciniti::uniform_type::UNIFORM_MATRIX_3fv:
			glUniformMatrix3fv(glGetUniformLocation(a_program_id, a_uniform_location), a_count, a_should_transpose, a_data_to_pass);
			break;
		case uciniti::uniform_type::UNIFORM_MATRIX_4fv:
			glUniformMatrix4fv(glGetUniformLocation(a_program_id, a_uniform_location), a_count, a_should_transpose, a_data_to_pass);
			break;
		default:
			printf("ERROR: bind_uniform_data() call. Could not find matrix uniform type passed.\n");
			break;
		}
	}

	void ShaderManager::clean_manager()
	{
		// Delete Shader* pointers
		for (std::pair<std::string, Shader*> this_pair : m_shader_list)
			delete this_pair.second;

		// Delete ShaderProgram* pointers
		for (std::pair<std::string, ShaderProgram*> this_pair : m_program_list)
			delete this_pair.second;
	}
}