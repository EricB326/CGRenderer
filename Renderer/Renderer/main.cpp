/* Standard lib includes
*/
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

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
FreeCamera* main_camera;
std::vector<Mesh*> mesh_list;
std::vector<ShaderManager*> shader_list;

void clean_memory();
bool init_window();
void create_geometry();
void create_shaders();

int main()
{
	// Check for memory leaks
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	// Initialize window
	if (!init_window())
		return -1;

	// Create geometry
	create_geometry();

	// Create shaders
	create_shaders();

	/*** Camera ***/
	main_camera = new FreeCamera();
	main_camera->set_perspective(glm::radians(75.0f), (float)WIDTH / (float)HEIGHT, 0.01f, 100.0f);
	main_camera->set_look_at(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 model = glm::mat4(1.0f);

	/*** Background colour ***/
	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);

	glm::vec4 colour;

	//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);

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
		//m_main_camera->set_look_at(glm::vec3(glm::sin(current_time) * 7, 3, glm::cos(current_time) * 7), glm::vec3(0), glm::vec3(0, 1, 0));

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);

		// User created shader program
		shader_list[0]->use_shader();

		uniform_projection_location = glGetUniformLocation(shader_list[0]->get_program(), "projection_view_matrix");
		uniform_model_location = glGetUniformLocation(shader_list[0]->get_program(), "model_matrix");
		uniform_colour_location = glGetUniformLocation(shader_list[0]->get_program(), "colour");

		glUniformMatrix4fv(uniform_projection_location, 1, false, glm::value_ptr(main_camera->get_projection_view()));

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, -1.0f, 0.0f));
		colour = glm::vec4(0.0f, 1.0f, 0.0f, 1.0f);

		glUniformMatrix4fv(uniform_model_location, 1, false, glm::value_ptr(model));
		glUniform4fv(uniform_colour_location, 1, glm::value_ptr(colour));

		mesh_list[0]->render_mesh();

		model = glm::mat4(1.0f);
		model = glm::translate(model, glm::vec3(0.0f, 0.5f, 0.0f));
		//model = glm::rotate(model, (float)glfwGetTime(), glm::vec3(0.5f, 1.0f, 0.0f));
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
	Mesh* floor_object = new Mesh(floor_verts, floor_indices, 24, 6, BASE_VERTEX);
	mesh_list.push_back(floor_object);
	Mesh* cube_object = new Mesh(vertices, indices, 48, 36, BASE_VERTEX);
	mesh_list.push_back(cube_object);
}

void create_shaders()
{
	/*** Make Shaders ***/
	ShaderManager* shader_to_push = new ShaderManager();

	shader_to_push->load_from_file("simple_vert", "..//Shaders//simple_vert.glsl", shader_type::VERTEX);
	shader_to_push->load_from_file("simple_frag", "..//Shaders//simple_frag.glsl", shader_type::FRAGMENT);

	shader_to_push->create_shader_program({ "simple_vert", "simple_frag" });
	shader_list.push_back(shader_to_push);
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

	// Delete each shader in shader_list
	for (size_t i = 0; i < shader_list.size(); i++)
	{
		delete shader_list[i];
		shader_list[i] = nullptr;
	}
	shader_list.clear();
}