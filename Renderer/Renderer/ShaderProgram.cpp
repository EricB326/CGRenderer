#include "ShaderProgram.h"
#include "Shader.h"

namespace uciniti
{
	ShaderProgram::ShaderProgram(const std::vector<uint>& a_shader_list)
		: m_program_id(0)
	{
		create_shader_program(a_shader_list);
	}

	void ShaderProgram::create_shader_program(const std::vector<uint>& a_shader_list)
	{
		// Create the empty shader program.
		m_program_id = glCreateProgram();

		// Verify the shader program was successfully made.
		if (!m_program_id)
		{
			printf("Creating shader program failed!\n");
			return;
		}

		// Verify there is shaders to populate the program
		if (a_shader_list.size() <= 0)
		{
			printf("No shader key's passed when creating the shader program!\n");
			return;
		}

		// Attach the shaders.
		for (size_t i = 0; i < a_shader_list.size(); i++)
		{
			// Attach each shader passed to the program.
			glAttachShader(m_program_id, a_shader_list[i]);
		}

		// Link the program.
		glLinkProgram(m_program_id);

		// Check for linking errors.
		GLint success = GL_FALSE;
		glGetProgramiv(m_program_id, GL_LINK_STATUS, &success);

		if (!success)
		{
			// Get the length of the error.
			GLint log_length = 0;
			glGetProgramiv(m_program_id, GL_INFO_LOG_LENGTH, &log_length);
			// Create the error buffer.
			char* log = new char[log_length];
			// Copy the error from the buffer.
			glGetProgramInfoLog(m_program_id, log_length, 0, log);

			// Create the error message.
			std::string error_message(log);
			error_message += "SHADER_FAILED_TO_COMPILE";
			printf(error_message.c_str());

			// Clean up.
			delete[] log;
		}
	}

	void ShaderProgram::use_program()
	{
		// Check that the shader program ID has been created
		if (!m_program_id)
		{
			printf("Error when attempting to use shader! Shader has no ID!\n");
			return;
		}

		glUseProgram(m_program_id);
	}

	void ShaderProgram::clean_program()
	{
		// Delete the program off the graphics card and set the ID to zero
		if (m_program_id)
		{
			glDeleteProgram(m_program_id);
			m_program_id = 0;
		}
	}
}