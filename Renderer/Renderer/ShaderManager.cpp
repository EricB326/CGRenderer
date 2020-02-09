#include "ShaderManager.h"

#include <iostream>
#include <sstream>

bool ShaderManager::load_from_file(const std::string a_shader_name, const std::string& a_filename, shader_type a_type)
{
	// Read the code from the file
	std::string shader_string;
	if (read_file_data(a_filename, shader_string))
	{
		// Switch what type of shader is being created based
		// on what the user passed.
		switch (a_type)
		{
			case shader_type::VERTEX:
			{
				// Store the users passed name as the key for the
				// shader ID passed back from the GPU.
				m_shader_list[a_shader_name] = create_shader(shader_string, GL_VERTEX_SHADER);
				// Verify the created shader was successfully mapped to the shader list map.
				mapping_check(a_shader_name);
				return true;
			}
			case shader_type::TESSELLATION_EVALUATION:
			{
				// @see case shader_type::VERTEX:
				m_shader_list[a_shader_name] = create_shader(shader_string, GL_TESS_EVALUATION_SHADER);
				mapping_check(a_shader_name);
				return true;
			}
			case shader_type::TESSELLATION_CONTROL:
			{
				// @see case shader_type::VERTEX:
				m_shader_list[a_shader_name] = create_shader(shader_string, GL_TESS_CONTROL_SHADER);
				mapping_check(a_shader_name);
				return true;
			}
			case shader_type::GEOMETRY:
			{
				// @see case shader_type::VERTEX:
				m_shader_list[a_shader_name] = create_shader(shader_string, GL_GEOMETRY_SHADER);
				mapping_check(a_shader_name);
				return true;
			}
			case shader_type::FRAGMENT:
			{
				// @see case shader_type::VERTEX:
				m_shader_list[a_shader_name] = create_shader(shader_string, GL_FRAGMENT_SHADER);
				mapping_check(a_shader_name);
				return true;
			}
			default:
			{
				std::cout << "Shader type passed is invalid!\n";
				break;
			}
		}
	}

	return false;
}

void ShaderManager::create_shader_program(const std::vector<std::string>& a_key_list)
{
	// Create the empty shader program.
	m_shader_program_id = glCreateProgram();

	// Verify the shader program was successfully made.
	if (!m_shader_program_id)
	{
		std::cout << "Creating shader program failed!\n";
		return;
	}

	// Attach the shaders.
	for (size_t i = 0; i < a_key_list.size(); i++)
	{
		// Attach each shader passed to the program.
		GLuint shader_to_attach = get_shader(a_key_list.at(i));

		// -1 represent an error thrown by get_shader().
		if (shader_to_attach == -1)
		{
			return;
		}

		glAttachShader(m_shader_program_id, shader_to_attach);
	}

	// Link the program.
	glLinkProgram(m_shader_program_id);

	// Check for linking errors
	GLint success = GL_FALSE;
	glGetProgramiv(m_shader_program_id, GL_LINK_STATUS, &success);

	if (!success)
	{
		// Get the length of the error
		GLint log_length = 0;
		glGetProgramiv(m_shader_program_id, GL_INFO_LOG_LENGTH, &log_length);
		// Create the error buffer
		char* log = new char[log_length];
		// Copy the error from the buffer
		glGetProgramInfoLog(m_shader_program_id, log_length, 0, log);

		// Create the error message
		std::string error_message(log);
		error_message += "SHADER_FAILED_TO_COMPILE";
		printf(error_message.c_str());

		// Clean up
		delete[] log;
	}
}

uint ShaderManager::get_shader(const std::string& a_key)
{
	// Using the .find() search for the key passed through.
	// The iterator stores the address of the key value pair.
	m_shader_list_iterator = m_shader_list.find(a_key);

	// If we reached the end of the m_shader_list, the key was
	// not found in the map.
	if (m_shader_list_iterator == m_shader_list.end())
	{
		std::cout << "\nKey: '" << a_key << "' was not found! Perhaps verify you spelt the key correctly?\n";

		// Return error value.
		return -1;
	}
	else
	{
		std::cout << "\nKey passed was present: " << m_shader_list_iterator->first << " contains the shader ID: " << m_shader_list_iterator->second << "\n";

		// Return the second part of the map, in this case the shader ID.
		return m_shader_list_iterator->second;
	}
}

void ShaderManager::use_shader()
{
	// Check that the shader program ID has been created
	if (!m_shader_program_id)
	{
		std::cout << "Error when attempting to use shader! Shader has no ID!\n";
		return;
	}

	glUseProgram(m_shader_program_id);
}

void ShaderManager::clean_shader()
{
	// Delete the program off the graphics card and set the ID to zero
	if (m_shader_program_id)
	{
		glDeleteProgram(m_shader_program_id);
		m_shader_program_id = 0;
	}
}

GLuint ShaderManager::create_shader(const std::string& const a_shader_data, GLenum a_type)
{
	// Create the individual id to be returned.
	GLuint shader_to_create_id = glCreateShader(a_type);

	// Convert the shader data to c char*.
	const char* shader_data = a_shader_data.c_str();
	// Pass the char* shader_data to the shader location.
	glShaderSource(shader_to_create_id, 1, (const GLchar**)&shader_data, 0);
	// Build the shader.
	glCompileShader(shader_to_create_id);

	// Check for any errors when creating the shader.
	GLint success = GL_FALSE;
	// Get the information of if we were successful or not.
	glGetShaderiv(shader_to_create_id, GL_COMPILE_STATUS, &success);
	// Check the result.
	if (!success)
	{
		// Get the length of the error.
		GLint log_length = 0;
		glGetShaderiv(shader_to_create_id, GL_INFO_LOG_LENGTH, &log_length);
		// Create the error buffer.
		char* log = new char[log_length];
		// Copy the error from the buffer.
		glGetShaderInfoLog(shader_to_create_id, log_length, 0, log);

		// Create the error message.
		std::string error_message(log);
		error_message += "SHADER_FAILED_TO_COMPILE";
		printf(error_message.c_str());

		// Clean up.
		delete[] log;

		// Return error value
		return -1;
	}

	// Successful creation of the shader
	return shader_to_create_id;
}

bool ShaderManager::read_file_data(const std::string& a_filename, std::string& a_shader_buffer)
{
	// Open the file stream.
	std::ifstream in_file_stream(a_filename.c_str(), std::ifstream::in);

	// Verify the file exists by checking if the file stream successfully opened it.
	std::stringstream string_stream;
	if (in_file_stream.is_open())
	{
		// Verify the file is not empt and there is content to be read.
		if (file_is_empty(in_file_stream))
		{
			// Warn the user of the lack of content.
			std::cout << "Contents of shader file \"" << a_filename << "\ is empty!\nHave you written and shader code?\n";
			return false;
		}

		// Read the data from the file stream to the string stream.
		string_stream << in_file_stream.rdbuf();
		// Send the read data from the string stream to the referenced shader buffer.
		a_shader_buffer = string_stream.str();
		// Close the file.
		in_file_stream.close();

		// Successful file data transfer
		return true; 
	}

	// File failed to open
	std::cout << "Failed to open shader file \"" << a_filename << "\!\n";
	return false;
}

bool ShaderManager::file_is_empty(std::ifstream& a_file_content)
{
	// Check if the next character is the end of the file, if the next
	// character is, than return true. We are at the end.
	return a_file_content.peek() == std::ifstream::traits_type::eof();
}

void ShaderManager::mapping_check(const std::string& a_key)
{
	// Using the .find() search for the key passed through.
	// The iterator stores the address of the key value pair.
	m_shader_list_iterator = m_shader_list.find(a_key);

	// If we reached the end of the m_shader_list, the key was
	// not found in the map.
	if (m_shader_list_iterator == m_shader_list.end())
	{
		std::cout << "Mapping was unsuccessful!\n";
	}
	else
	{
		std::cout << "Successfully mapped shader id: " << m_shader_list_iterator->second << " to the shader name (key): " << m_shader_list_iterator->first << "\n";
	}
}
