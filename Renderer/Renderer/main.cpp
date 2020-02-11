/* Standard lib includes
*/
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <chrono>

/* Graphic includes
*/
#include "gl_core_4_5.h"
#include "glfw3.h"

/* Math includes
*/
#include "glm.hpp"
#include "ext.hpp"

/* User defined includes
*/
#include "common.h"
#include "Mesh.h"
#include "ShaderManager.h"
#include "FreeCamera.h"

GLFWwindow* window;
uciniti::FreeCamera* main_camera;
std::vector<uciniti::Mesh*> mesh_list;
uciniti::ShaderManager* shaders;

void clean_memory();
bool init_window();
void create_geometry();
bool create_shaders();

int main()
{
	// Check for memory leaks
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	// Initialize window
	if (!init_window())
		return -1;

	// Check current time point.
	auto measure_time_start = std::chrono::high_resolution_clock::now();
	// Create geometry
	create_geometry();
	// Check time point after running the function.
	auto measure_time_stop = std::chrono::high_resolution_clock::now();
	// Work out the difference in time points to calculate how long it took the function to run.
	auto duration_of_function = std::chrono::duration_cast<std::chrono::microseconds>(measure_time_stop - measure_time_start);
	printf("Creating geometry took: %.3f milliseconds!\n", (float)duration_of_function.count() / 1000);

	// Check current time point.
	measure_time_start = std::chrono::high_resolution_clock::now();
	// Create shaders
	if (!create_shaders())
	{
		printf("ERROR: create_shaders() call! Failed to create shader. See console for output.\n");
	}
	// Check time point after running the function.
	measure_time_stop = std::chrono::high_resolution_clock::now();
	// Work out the difference in time points to calculate how long it took the function to run.
	duration_of_function = std::chrono::duration_cast<std::chrono::microseconds>(measure_time_stop - measure_time_start);
	printf("Creating shaders took: %.3f milliseconds!\n", (float)duration_of_function.count() / 1000);

	/*** Camera ***/
	main_camera = new uciniti::FreeCamera();
	main_camera->set_perspective(glm::radians(75.0f), (float)WIDTH / (float)HEIGHT, 0.01f, 100.0f);
	main_camera->set_look_at(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 model = glm::mat4(1.0f);

	/*** Background colour ***/
	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);

	glm::vec4 colour;


	uint uniform_projection_location = 0, uniform_model_location = 0, uniform_colour_location = 0;

	// Delta time
	GLfloat delta_time = 0.0f;
	GLfloat last_time = 0.0f;

	/*** Game Loop ***/
	while (!glfwWindowShouldClose(window) && glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS)
	{
		GLfloat current_time = glfwGetTime();
		delta_time = current_time - last_time;
		last_time = current_time;

		glfwPollEvents();

		main_camera->update(delta_time);

		// orbit
		main_camera->set_look_at(glm::vec3(glm::sin(current_time) * 7, 3, glm::cos(current_time) * 7), glm::vec3(0), glm::vec3(0, 1, 0));

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);

		// User created shader program
		shaders->use_program("simple_program");

		uniform_projection_location = glGetUniformLocation(shaders->get_program_id("simple_program"), "projection_view_matrix");
		uniform_model_location = glGetUniformLocation(shaders->get_program_id("simple_program"), "model_matrix");
		uniform_colour_location = glGetUniformLocation(shaders->get_program_id("simple_program"), "colour");

		glUniformMatrix4fv(uniform_projection_location, 1, false, glm::value_ptr(main_camera->get_projection_view()));

		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, -1.0f, 0.0f));
		colour = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);

		glUniformMatrix4fv(uniform_model_location, 1, false, glm::value_ptr(model));
		glUniform4fv(uniform_colour_location, 1, glm::value_ptr(colour));

		mesh_list[0]->render_mesh();

		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glLineWidth(5.0f);

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 1.5f, 0.0f));
		model = glm::rotate(model, current_time, glm::vec3(0.5f, 1.0f, 0.0f));
		model = glm::scale(model, glm::vec3(1.0f, 1.0f, 2.0f));
		colour = glm::vec4(1.0f, 0.0f, 1.0f, 1.0f);

		glUniformMatrix4fv(uniform_model_location, 1, false, glm::value_ptr(model));
		glUniform4fv(uniform_colour_location, 1, glm::value_ptr(colour));

		mesh_list[1]->render_mesh();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	/*** Clean memory ***/
	clean_memory();

	glfwDestroyWindow(window);
	glfwTerminate();

	return 0;
}

bool init_window()
{
	/*** Initialise OpenGL and create window ***/
	if (!glfwInit())
	{
		printf("Failed to initialise the window!\n");
		return false;
	}

	window = glfwCreateWindow(WIDTH, HEIGHT, "Renderer", nullptr, nullptr);

	if (window == nullptr)
	{
		printf("Failed to create a window!\n");
		glfwTerminate();
		return false;
	}

	glfwMakeContextCurrent(window);

	if (ogl_LoadFunctions() == ogl_LOAD_FAILED)
	{
		printf("Failed to load OpenGL functionality!\n");
		glfwDestroyWindow(window);
		glfwTerminate();
		return false;
	}

	glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	auto major = ogl_GetMajorVersion();
	auto minor = ogl_GetMinorVersion();
	printf("Version of OpenGL: %i.%i\n\n", major, minor);

	return true;
}

void create_geometry()
{
	/*** Mesh data ***/
	// Large floor plane points
	//								Positions             Colour
	//							 X      Y       Z      R      G     B
	GLfloat floor_verts[] = { -10.0f,  0.0f, -10.0f,  0.0f,  0.0f,  0.0f,   // Back Left
							   10.0f,  0.0f, -10.0f,  1.0f,  0.0f,  0.0f,   // Back Right
							  -10.0f,  0.0f,  10.0f,  0.0f,  1.0f,  0.0f,   // Front Left
							   10.0f,  0.0f,  10.0f,  1.0f,  1.0f,  0.0f }; // Front Right

	GLuint floor_indices[] = { 1, 0, 2, 2, 3, 1 };

	// Create the points on a cube
	//					        Positions             Colours       
	//						 X      Y      Z      R      G      B   
	GLfloat vertices[] = { -1.0f, -1.0f,  0.0f,  1.0f,  0.0f,  0.0f,   // Bot left	     -- 0
							1.0f, -1.0f,  0.0f,  0.0f,  1.0f,  0.0f,   // Bot right	     -- 1
						   -1.0f,  1.0f,  0.0f,  0.0f,  0.0f,  1.0f,   // Top left	     -- 2
							1.0f,  1.0f,  0.0f,  1.0f,  0.0f,  1.0f,   // Top right	     -- 3
						   -1.0f, -1.0f, -1.0f,  1.0f, -1.0f, -1.0f,   // Back bot left  -- 4
							1.0f, -1.0f, -1.0f,  1.0f, -1.0f, -1.0f,   // Back bot right -- 5
						   -1.0f,  1.0f, -1.0f,  1.0f,  1.0f, -1.0f,   // Back top left  -- 6
							1.0f,  1.0f, -1.0f,  1.0f,  1.0f, -1.0f }; // Back top right -- 7

	// Create the faces of a cube
	GLuint indices[] = { 0, 2, 6, 6, 4, 0,   // Left face
						 1, 3, 7, 7, 5, 1,   // Right face
						 4, 6, 7, 7, 5, 4,   // Back face
						 0, 2, 3, 3, 1, 0,   // Front face
						 0, 4, 5, 5, 1, 0,   // Bot face
						 2, 6, 7, 7, 3, 2 }; // Top face

	/*** Create and 'load' floor mesh ***/
	uciniti::Mesh* floor_object = new uciniti::Mesh(floor_verts, floor_indices, 24, 6, vertex_type::BASE_VERTEX);
	mesh_list.push_back(floor_object);
	uciniti::Mesh* cube_object = new uciniti::Mesh(vertices, indices, 48, 36, vertex_type::BASE_VERTEX);
	mesh_list.push_back(cube_object);
}

bool create_shaders()
{
	/*** Make Shaders ***/
	shaders = new uciniti::ShaderManager();

	// Load shaders.
	if (!shaders->load_shader("simple_vert", shader_type::VERTEX, "..//Shaders//simple_vert.glsl"))
		return false;
	if (!shaders->load_shader("simple_frag", shader_type::FRAGMENT, "..//Shaders//simple_frag.glsl"))
		return false;

	// Create programs.
	if (!shaders->create_shader_program("simple_program", { "simple_vert", "simple_frag" }))
		return false;

	// Return true, successfully created shaders.
	return true;
}

void clean_memory()
{
	// Delete the camera
	delete main_camera;
	main_camera = nullptr;

	// Delete each mesh in mesh_list
	for (size_t i = 0; i < mesh_list.size(); i++)
	{
		delete mesh_list[i];
		mesh_list[i] = nullptr;
	}
	mesh_list.clear();

	// Delete each shaders
	delete shaders;
}