#include "ShaderManager.h"

/* Standard includes
*/
#include <iostream>
#include <sstream>

namespace uciniti
{
	void ShaderManager::load_shader(const std::string& a_key_name, shader_type a_type_of_shader, const std::string& a_filepath)
	{
		// If the shader already exists, return.
		if (does_shader_key_already_exist(a_key_name))
			return;
	
		// Create and load the desired shader.
		Shader* new_mapped_shader = new Shader();
		new_mapped_shader->load_from_file(a_type_of_shader, a_filepath);
	
		// Create a new map key with the created shader.
		m_shader_list[a_key_name] = new_mapped_shader;
	}

	void ShaderManager::create_shader_program(const std::string& a_key_name, std::vector<std::string> a_shader_list)
	{
		// If the program already exists, return.
		if (does_program_key_already_exist(a_key_name))
			return;

		// Create the new shader program.
		ShaderProgram* new_mapped_program = new ShaderProgram;
		std::vector<uint> shader_id_list;

		// Loop through each identifier.
		for (size_t i = 0; i < a_shader_list.size(); i++)
		{
			// Verify there is a shader at the identifier passed.
			if (!verify_shader_map_key(a_shader_list[i]))
				return;

			// Push into the uint vector the uint mapped with the identifier passed.
			shader_id_list.push_back(m_shader_list.at(a_shader_list[i])->get_shader_id());
		}

		// Verify there are shader ID's to pass.
		if (shader_id_list.size() <= 0)
		{
			printf("Could not find any ID's when creating shader program!\n");
		}

		// Create the shader program.
		new_mapped_program->create_shader_program(shader_id_list);

		// Map the ShaderProgram* to the program map with the key name given.
		m_program_list[a_key_name] = new_mapped_program;
	}

	void ShaderManager::use_program(const std::string& a_program_key)
	{
		// Verify the program key provided.
		if (!verify_program_map_key(a_program_key))
			return;

		m_program_list.at(a_program_key)->use_program();
	}

	uint ShaderManager::get_program_id(const std::string& a_program_key)
	{
		return m_program_list.at(a_program_key)->get_program_id();
	}
	
	bool ShaderManager::does_shader_key_already_exist(const std::string& a_key)
	{
		// Using the .find() search for the key passed through.
		// The iterator stores the address of the key value pair.
		auto shader_list_iterator = m_shader_list.find(a_key);
	
		// If we reached the end of the m_shader_list, the key was
		// not found in the map.
		if (shader_list_iterator == m_shader_list.end())
			return false; // Return false, shader doesn't exist.
		else
		{
			// Shader key was found.
			printf("\nKey: '%s' already exists in shader map list!\n", a_key.c_str());
			return true;
		}
	}

	bool ShaderManager::verify_shader_map_key(const std::string& a_key)
	{
		// Using the .find() search for the key passed through.
		// The iterator stores the address of the key value pair.
		auto shader_list_iterator = m_shader_list.find(a_key);

		// If we reached the end of the m_shader_list, the key was
		// not found in the map.
		if (shader_list_iterator == m_shader_list.end())
		{
			printf("\nKey: '%s' does not exist in shader map list!\n", a_key.c_str());
			return false; // Return false, key not verified.
		}
		else
		{
			// Key was found and verified.
			return true;
		}
	}

	bool ShaderManager::does_program_key_already_exist(const std::string& a_key)
	{
		// Using the .find() search for the key passed through.
		// The iterator stores the address of the key value pair.
		auto program_list_iterator = m_program_list.find(a_key);

		// If we reached the end of the m_program_list, the key was
		// not found in the map.
		if (program_list_iterator == m_program_list.end())
			return false; // Return false, program doesn't exist.
		else
		{
			// Shader key was found.
			printf("\nKey: '%s' already exists in program map list!\n", a_key.c_str());
			return true;
		}
	}

	bool ShaderManager::verify_program_map_key(const std::string& a_key)
	{
		// Using the .find() search for the key passed through.
		// The iterator stores the address of the key value pair.
		auto program_list_iterator = m_program_list.find(a_key);

		// If we reached the end of the m_program_list, the key was
		// not found in the map.
		if (program_list_iterator == m_program_list.end())
		{
			printf("\nKey: '%s' does not exist in program map list!\n", a_key.c_str());
			return false; // Return false, key not verified.
		}
		else
		{
			// Key was found and verified.
			return true;
		}
	}

	void ShaderManager::clean_manager()
	{
		// Delete Shader* pointers
		for (std::pair<std::string, Shader*> this_pair : m_shader_list)
		{
			delete this_pair.second;
		}

		// Delete ShaderProgram* pointers
		for (std::pair<std::string, ShaderProgram*> this_pair : m_program_list)
		{
			delete this_pair.second;
		}
	}
}