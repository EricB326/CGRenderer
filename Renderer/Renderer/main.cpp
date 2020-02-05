#include "glm.hpp"
#include "ext.hpp"

#include "gl_core_4_5.h"
#include "glfw3.h"

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

#include "common.h"
#include "Mesh.h"

int main()
{
	/*** Initialise OpenGL and create window ***/
	if (!glfwInit()) 
	{ 
		printf("Failed to initialise the window!\n"); 
		return 1; 
	}

	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Renderer", nullptr, nullptr);

	if (window == nullptr)
	{
		printf("Failed to create a window!\n");
		glfwTerminate();
		return 2;
	}

	glfwMakeContextCurrent(window);

	if (ogl_LoadFunctions() == ogl_LOAD_FAILED)
	{
		printf("Failed to load OpenGL functionality!\n");
		glfwDestroyWindow(window);
		glfwTerminate();
		return 3;
	}

	auto major = ogl_GetMajorVersion();
	auto minor = ogl_GetMinorVersion();
	printf("GL: %i.%i\n", major, minor);

	/*** Mesh data ***/
	std::vector<Mesh*> mesh_list;

	// Large floor plane points
	//								Positions             Colour
	//							X     Y      Z      R      G     B
	GLfloat floor_verts[] = { -1.0f, 0.0f, -1.0f,  0.0f,  0.0f, 0.0f,   // Back Left
							   1.0f, 0.0f, -1.0f, 1.0f,  0.0f, 0.0f,   // Back Right
							  -1.0f, 0.0f,  1.0f,  0.0f, 1.0f, 0.0f,   // Front Left
							   1.0f, 0.0f,  1.0f, 1.0f, 1.0f, 0.0f }; // Front Right

	GLuint floor_indices[] = { 1, 0, 2, 2, 3, 1 };

	// Create the points on a cube
	//					        Positions             Colours       
	//						 X      Y      Z      R      G      B   
	//GLfloat vertices[] = { -1.0f, -1.0f,  0.0f,  1.0f,  0.0f,  0.0f,   // Bot left	     -- 0
	//						1.0f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,   // Bot right	     -- 1
	//					   -1.0f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,   // Top left	     -- 2
	//						1.0f,  1.0f,  0.0f,  1.0f,  0.0f,  1.0f,   // Top right	     -- 3
	//					   -1.0f, -1.0f, -1.0f,  1.0f, -1.0f, -1.0f,   // Back bot left  -- 4
	//						1.0f, -1.0f, -1.0f,  1.0f, -1.0f, -1.0f,   // Back bot right -- 5
	//					   -1.0f,  1.0f, -1.0f,  1.0f,  1.0f, -1.0f,   // Back top left  -- 6
	//						1.0f,  1.0f, -1.0f,  1.0f,  1.0f, -1.0f }; // Back top right -- 7
	//
	//// Create the faces of a cube
	//GLuint indices[] = { 0, 2, 6, 6, 4, 0,  // Left face
	//					 1, 3, 7, 7, 5, 1,  // Right face
	//					 4, 6, 7, 7, 5, 4,  // Back face
	//					 0, 2, 3, 3, 1, 0,  // Front face
	//					 0, 4, 5, 5, 1, 0,  // Bot face
	//					 2, 6, 7, 7, 3, 2}; // Top face

	/*** Create and 'load' floor mesh ***/
	Mesh* floor_object = new Mesh(floor_verts, floor_indices, 24, 6, BASE_VERTEX);
	mesh_list.push_back(floor_object);

	/*** Camera ***/
	glm::mat4 projection = glm::perspective(glm::radians(90.0f), (float)WIDTH / (float)HEIGHT, 1.0f, 100.0f);
	glm::mat4 view = glm::lookAt(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 model = glm::mat4(1.0f);

	/*** Grab Shaders ***/
	uint vertex_shader_id = 0;
	uint fragment_shader_id = 0;
	uint shader_program_id = 0;

	// Load shader from file into a string
	std::string shader_data;
	std::ifstream in_file_stream("..//Shaders//simple_vert.glsl", std::ifstream::in);

	// Load the source into a string for compilation
	std::stringstream string_stream;
	if (in_file_stream.is_open())
	{
		string_stream << in_file_stream.rdbuf();
		shader_data = string_stream.str();
		in_file_stream.close();
	}

	// Allocate space for the shader program
	vertex_shader_id = glCreateShader(GL_VERTEX_SHADER);
	// Convert to raw char*
	const char* data = shader_data.c_str();
	// Send in the char* to the shader location
	glShaderSource(vertex_shader_id, 1, (const GLchar**)&data, 0);
	// Build shader
	glCompileShader(vertex_shader_id);

	// Variables for checking errors
	GLint success = GL_FALSE;
	GLchar errorLog[1024] = { 0 };

	// Get information on if we successfully compiled or not
	glGetShaderiv(vertex_shader_id, GL_COMPILE_STATUS, &success);

	// Check if the result of the compile was unsuccessful
	if (!success)
	{
		// Get the length of the error
		GLint log_length = 0;
		glGetShaderiv(vertex_shader_id, GL_INFO_LOG_LENGTH, &log_length);
		// Create the error buffer
		char* log = new char[log_length];
		// Copy the error from the buffer
		glGetShaderInfoLog(vertex_shader_id, log_length, 0, log);

		// Create the error message
		std::string error_message(log);
		error_message += "SHADER_FAILED_TO_COMPILE";
		printf(error_message.c_str());

		// Clean up
		delete[] log;
	}

	/*** Fragment Shader ***/
	std::ifstream in_file_stream_frag("..//Shaders//simple_frag.glsl", std::ifstream::in);
	std::stringstream frag_string_stream;

	// Load the source into a string for compilation
	if (in_file_stream_frag.is_open())
	{
		frag_string_stream << in_file_stream_frag.rdbuf();
		shader_data = frag_string_stream.str();
		in_file_stream_frag.close();
	}

	// Allocate space for the shader program
	fragment_shader_id = glCreateShader(GL_FRAGMENT_SHADER);
	// Convert to raw char*
	data = shader_data.c_str();
	// Send in the char* to the shader location
	glShaderSource(fragment_shader_id, 1, (const GLchar**)&data, 0);
	// Build shader
	glCompileShader(fragment_shader_id);

	// Variables for checking errors
	success = GL_FALSE;

	// Get information on if we successfully compiled or not
	glGetShaderiv(fragment_shader_id, GL_COMPILE_STATUS, &success);

	// Check if the result of the compile was unsuccessful
	if (!success)
	{
		// Get the length of the error
		GLint log_length = 0;
		glGetShaderiv(fragment_shader_id, GL_INFO_LOG_LENGTH, &log_length);
		// Create the error buffer
		char* log = new char[log_length];
		// Copy the error from the buffer
		glGetShaderInfoLog(fragment_shader_id, log_length, 0, log);

		// Create the error message
		std::string error_message(log);
		error_message += "SHADER_FAILED_TO_COMPILE";
		printf(error_message.c_str());
		
		// Clean up
		delete[] log;
	}

	/*** Link the shaders to the shader program ***/
	// Create the new shader program
	shader_program_id = glCreateProgram();

	// Attach both shaders by ID and type
	glAttachShader(shader_program_id, vertex_shader_id);
	glAttachShader(shader_program_id, fragment_shader_id);

	// Finally link the program
	glLinkProgram(shader_program_id);

	// Check for linking errors
	success = GL_FALSE;
	glGetProgramiv(shader_program_id, GL_LINK_STATUS, &success);

	if (!success)
	{
		// Get the length of the error
		GLint log_length = 0;
		glGetProgramiv(shader_program_id, GL_INFO_LOG_LENGTH, &log_length);
		// Create the error buffer
		char* log = new char[log_length];
		// Copy the error from the buffer
		glGetProgramInfoLog(shader_program_id, log_length, 0, log);

		// Create the error message
		std::string error_message(log);
		error_message += "SHADER_FAILED_TO_COMPILE";
		printf(error_message.c_str());

		// Clean up
		delete[] log;
	}


	/*** Background colour ***/
	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);

	glm::vec4 colour = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

	uint uniform_projection_location = 0, uniform_model_location = 0, uniform_colour_location = 0;

	/*** Game Loop ***/
	while (!glfwWindowShouldClose(window) && glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS)
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);

		// User created shader program
		glUseProgram(shader_program_id);

		uniform_projection_location = glGetUniformLocation(shader_program_id, "projection_view_matrix");
		uniform_model_location = glGetUniformLocation(shader_program_id, "model_matrix");
		uniform_colour_location = glGetUniformLocation(shader_program_id, "colour");

		glm::mat4 projection_view = projection * view;

		glUniformMatrix4fv(uniform_projection_location, 1, false, glm::value_ptr(projection_view));


		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, -1.0f, 0.0f));
		//model = glm::rotate(model, 0.016f, glm::vec3(0.0, 0.0, 1.0));

		//colour = glm::vec4(glm::sin(glfwGetTime()), -glm::sin(glfwGetTime()), 0.5, 1.0);

		glUniformMatrix4fv(uniform_model_location, 1, false, glm::value_ptr(model));
		glUniform4fv(uniform_colour_location, 1, glm::value_ptr(colour));

		mesh_list[0]->render_mesh();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	/*** Clean memory ***/
	//glDeleteBuffers(1, &VAO);
	//glDeleteBuffers(1, &VBO);

	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}