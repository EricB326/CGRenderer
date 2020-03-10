#ifndef SHADER_PROGRAM_H
#define SHADER_PROGRAM_H

/* Standard lib includes
*/
#include <string>
#include <vector>

/* Graphic includes
*/
#include "gl_core_4_5.h"
#include "glfw3.h"

/* User defined includes
*/
#include "common.h"

namespace uciniti
{
	/* @brief Class ShaderProgram. Handles the creation, and use of
		compiled shader programes. Stores a program ID for referring
		to in other classes.
	*/
	class ShaderProgram
	{
	public:
		/******************************************************/
		// Functions
		/* @brief Constructor zeros all values. Expects the user
			to later call create_shader_program().
		*/
		ShaderProgram() : m_program_id(0) {}

		/* @brief Constructor zeros all values. Calls for the user
			create_shader_program() passing the data recieved.
		   @param Vector of ID's to be attached to the program.
		*/
		ShaderProgram(const std::vector<uint>& a_shader_list);

		/* @brief Cleans up the shader program on destruction.
		*/
		~ShaderProgram() { clean_program(); }

		/* @brief Creates a shader program and attaches shaders via the
			map keys to it. Finally links it all.
		   @param Vector of uints that store the different ID's to attach
			to the shader program.
		*/
		void create_shader_program(const std::vector<uint>& a_shader_list);

		/* @brief Uses the shader program for the next draw call.
		*/
		void use_program();

		/* @brief Retrives the shader program ID stored.
		   @return Unsigned int of the ID from the GPU.
		*/
		inline uint get_program_id() { return m_program_id; }

	private:
		/******************************************************/
		// Variables
		/* @brief Stores the id of the created program.
		*/
		uint m_program_id;

		/******************************************************/
		// Functions
		/* @brief Cleans the shader program and resets values.
		*/
		void clean_program();
	};
}

#endif // !SHADER_PROGRAM_H