#include "Shader.h"
#include <sstream>

namespace uciniti
{
	Shader::Shader(const shader_type a_type_of_shader, const std::string& a_filepath)
		: m_shader_id(0)
	{
		load_from_file(a_type_of_shader, a_filepath);
	}

	void Shader::load_from_file(const shader_type a_type_of_shader, const std::string& a_filepath)
	{
		// Read the code form the file.
		std::string shader_string;
		if (read_file_content(a_filepath, shader_string))
		{
			// Switch what type of shader is being created based
			// on what the user passed.
			switch (a_type_of_shader)
			{
			case shader_type::VERTEX:
			{
				create_shader(shader_string, GL_VERTEX_SHADER);
				break;
			}
			case shader_type::TESSELLATION_EVALUATION:
			{
				create_shader(shader_string, GL_TESS_EVALUATION_SHADER);
				break;
			}
			case shader_type::TESSELLATION_CONTROL:
			{
				create_shader(shader_string, GL_TESS_CONTROL_SHADER);
				break;
			}
			case shader_type::GEOMETRY:
			{
				create_shader(shader_string, GL_GEOMETRY_SHADER);
				break;
			}
			case shader_type::FRAGMENT:
			{
				create_shader(shader_string, GL_FRAGMENT_SHADER);
				break;
			}
			default:
				break;
			}
		}
	}

	bool Shader::read_file_content(const std::string& a_filepath, std::string& a_string_buffer)
	{
		// Open the file stream.
		std::ifstream in_file_stream(a_filepath.c_str(), std::ifstream::in);

		// Verify the file exists by checking if the file stream successfully opened it.
		std::stringstream string_stream;
		if (in_file_stream.is_open())
		{
			// Verify the file is not empt and there is content to be read.
			if (is_file_empty(in_file_stream))
			{
				// Warn the user of the lack of content.
				printf("Contents of shader file \\%s\\ is empty!\nHave you written any shader code?", a_filepath.c_str());
				return false;
			}

			// Read the data from the file stream to the string stream.
			string_stream << in_file_stream.rdbuf();
			// Send the read data from the string stream to the referenced shader buffer.
			a_string_buffer = string_stream.str();
			// Close the file.
			in_file_stream.close();

			// Successful file data transfer
			return true;
		}

		// File failed to open
		printf("Failed to open shader file \\%s\\!\n", a_filepath.c_str());
		return false;
	}

	bool Shader::is_file_empty(std::ifstream& a_file_content)
	{
		// Check if the next character is the end of the file, if the next
		// character is, than return true. We are at the end.
		return a_file_content.peek() == std::ifstream::traits_type::eof();
	}

	void Shader::create_shader(const std::string& a_file_content, GLenum a_type_of_shader)
	{
		// Create the individual id to be returned.
		GLuint shader_to_create_id = glCreateShader(a_type_of_shader);

		// Convert the shader data to c char*.
		const char* shader_data = a_file_content.c_str();
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

			// Set shader_id to error value
			m_shader_id = 0;
		}

		// Successful creation of the shader
		m_shader_id = shader_to_create_id;
	}
}