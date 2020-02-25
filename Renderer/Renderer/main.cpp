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
#include "TextureManager.h"

GLFWwindow* window;
uciniti::FreeCamera* main_camera;
std::vector<uciniti::Mesh*> mesh_list;
uciniti::ShaderManager* shaders;

void clean_memory();
bool init_window();
bool create_geometry();
bool create_textures();
bool create_shaders();

int main()
{
	// Check for memory leaks.
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	// Initialize window.
	if (!init_window())
		return -1;

	// Check current time point.
	auto measure_time_start = std::chrono::high_resolution_clock::now();
	printf("Creating geometry... ");
	// Create geometry.
	if (!create_geometry())
	{
		printf("ERROR: create_geometry() call. Failed to create shader. See console for output.\n");

		clean_memory();
		return -1;
	}
	// Check time point after running the function.
	auto measure_time_stop = std::chrono::high_resolution_clock::now();
	// Work out the difference in time points to calculate how long it took the function to run.
	auto duration_of_function = std::chrono::duration_cast<std::chrono::microseconds>(measure_time_stop - measure_time_start);
	printf("Time elapsed: %.3f milliseconds!\n", (float)duration_of_function.count() / 1000);


	measure_time_start = std::chrono::high_resolution_clock::now();
	// Create textures.
	printf("Creating textures... ");
	if (!create_textures())
	{
		printf("ERROR: create_textures() call. Failed to create texture. See console for output.\n");

		clean_memory();
		return -2;
	}
	measure_time_stop = std::chrono::high_resolution_clock::now();
	duration_of_function = std::chrono::duration_cast<std::chrono::microseconds>(measure_time_stop - measure_time_start);
	printf("Time elapsed: %.3f milliseconds!\n", (float)duration_of_function.count() / 1000);

	measure_time_start = std::chrono::high_resolution_clock::now();
	// Create shaders.
	printf("Creating shaders... ");
	if (!create_shaders())
	{
		printf("ERROR: create_shaders() call. Failed to create shader. See console for output.\n");

		clean_memory();
		return -3;
	}
	measure_time_stop = std::chrono::high_resolution_clock::now();
	duration_of_function = std::chrono::duration_cast<std::chrono::microseconds>(measure_time_stop - measure_time_start);
	printf("Time elapsed: %.3f milliseconds!\n", (float)duration_of_function.count() / 1000);

	/*** Camera ***/
	main_camera = new uciniti::FreeCamera();
	main_camera->set_perspective(glm::radians(75.0f), (float)WIDTH / (float)HEIGHT, 0.01f, 100.0f);
	main_camera->set_look_at(glm::vec3(0.0f, 1.0f, 3.0f), glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	glm::mat4 model = glm::mat4(1.0f);

	/*** Background colour ***/
	glClearColor(0.2f, 0.2f, 0.2f, 1.0f);

	// Delta time
	double delta_time = 0.0f;
	double last_time = 0.0f;

	// Used for delaying blocks of code.
	double run_delay = 0.0;

	uciniti::TextureManager::inst().get_all_texture_names();

	/*** Render Loop ***/
	while (!glfwWindowShouldClose(window) && glfwGetKey(window, GLFW_KEY_ESCAPE) != GLFW_PRESS)
	{
		double current_time = glfwGetTime();
		delta_time = current_time - last_time;
		last_time = current_time;

		glfwPollEvents();

		main_camera->update(delta_time);

		// orbit
		//main_camera->set_look_at(glm::vec3(glm::sin(current_time) * 7, 3, glm::cos(current_time) * 7), glm::vec3(0), glm::vec3(0, 1, 0));

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glEnable(GL_DEPTH_TEST);

		// User created shader program.
		shaders->use_program("crate_program");

		// Check for a shader reload.
		if (glfwGetKey(window, GLFW_KEY_R) && !run_delay)
			shaders->reload_shader_program("crate_program", { "crate_vert", "crate_frag" });
		// Delay the user from being able to constantly reload the shader.
		if (glfwGetKey(window, GLFW_KEY_R))
			run_delay++;
		else
			run_delay = 0.0;

		shaders->bind_uniform_data(shaders->get_program_id("crate_program"), "time", 1, (float)current_time, uciniti::uniform_type::UNIFORM_1f);
		shaders->bind_uniform_matrix_data(shaders->get_program_id("crate_program"), "projection_view_matrix", 1, false, glm::value_ptr(main_camera->get_projection_view()), uciniti::uniform_type::UNIFORM_MATRIX_4fv);

		model = glm::mat4(1.0f);
		//model = glm::translate(model, glm::vec3(0.0f, 20.0f, 0.0f));
		//model = glm::rotate(model, (float)current_time, glm::vec3(0.5f, 1.0f, 0.0f));
		//model = glm::rotate(model, glm::radians(90.0f), glm::vec3(1.0f, 1.0f, 0.0f));
		//model = glm::scale(model, glm::vec3(0.001f, 0.001f, 0.001f));
		//model = glm::scale(model, glm::vec3(3.0f, 3.0f, 3.0f));
		//colour = glm::vec4(1.0f, 0.0f, 1.0f, 1.0f);

		shaders->bind_uniform_matrix_data(shaders->get_program_id("crate_program"), "model_matrix", 1, false, (GLfloat*)&model, uciniti::uniform_type::UNIFORM_MATRIX_4fv);
		//glUniformMatrix4fv(0, 0, false, )

		uciniti::TextureManager::inst().use_texture("crate_texture");
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

bool create_geometry()
{
	/*** Mesh data ***/
	// Large floor plane points.
	//								Positions             Colour
	//							 X      Y       Z      R      G     B
	GLfloat floor_verts[] = { -10.0f,  0.0f, -10.0f,  0.0f,  0.0f,  0.0f,   // Back Left
							   10.0f,  0.0f, -10.0f,  1.0f,  0.0f,  0.0f,   // Back Right
							  -10.0f,  0.0f,  10.0f,  0.0f,  1.0f,  0.0f,   // Front Left
							   10.0f,  0.0f,  10.0f,  1.0f,  1.0f,  0.0f }; // Front Right

	GLuint floor_indices[] = { 1, 0, 2, 2, 3, 1 };

	/*** Create and 'load' floor mesh ***/
	uciniti::Mesh* floor_object = new uciniti::Mesh(floor_verts, floor_indices, 24, 6, vertex_type::BASE_VERTEX);
	mesh_list.push_back(floor_object);

	// Create loaded .obj model.
	//uciniti::Mesh* alien_bug = new uciniti::Mesh();
	//uciniti::Mesh* soulspear = new uciniti::Mesh();
	//uciniti::Mesh* stanford_bunny = new uciniti::Mesh();
	uciniti::Mesh* crate = new uciniti::Mesh();

	//bool loaded = alien_bug->load_obj("..//Models//KazChesna//Alienbug_LP.obj");
	//bool loaded2 = soulspear->load_obj("..//Models//Soulspear//soulspear.obj", "soulspear");
	//bool loaded3 = stanford_bunny->load_obj("..//Models//Stanford//Bunny.obj", "bunny");
	bool loaded4 = crate->load_obj("..//Models//Free3D//Crate//Crate1.obj", "crate");

	// Check for success.
	if (/*!loaded ||*/ /*!loaded2 || !loaded3 ||*/ !loaded4)
		return false;
	// Add the model to the list if successful.
	//mesh_list.push_back(alien_bug);
	//mesh_list.push_back(soulspear);
	//mesh_list.push_back(stanford_bunny);
	mesh_list.push_back(crate);

	return true;
}

bool create_textures()
{
	// Error checking variable for if textures were loaded.
	bool did_texture_load = false;

	// Make sure there is an instance of the texture class.
	uciniti::TextureManager& texture = uciniti::TextureManager::inst();

	// Begin loading textures.
	// Load crate texture.
	did_texture_load = texture.create_texture("crate_texture", "..//Textures//wooden_crate.jpg");
	if (!did_texture_load)
		return false;

	// Load brick texture.
	did_texture_load = texture.create_texture("brick_texture", "..//Textures//brick.jpg");
	if (!did_texture_load)
		return false;

	return true;
}

bool create_shaders()
{
	/*** Make Shaders ***/
	shaders = new uciniti::ShaderManager();

	// Load simple shaders.
	if (!shaders->load_shader("simple_vert", uciniti::shader_type::VERTEX, "..//Shaders//simple_vert.glsl"))
		return false;
	if (!shaders->load_shader("simple_frag", uciniti::shader_type::FRAGMENT, "..//Shaders//simple_frag.glsl"))
		return false;

	// Load crate model shaders
	if (!shaders->load_shader("crate_vert", uciniti::shader_type::VERTEX, "..//Shaders//crate_vert.glsl"))
		return false;
	if (!shaders->load_shader("crate_frag", uciniti::shader_type::FRAGMENT, "..//Shaders//crate_frag.glsl"))
		return false;

	// Create programs.
	if (!shaders->create_shader_program("simple_program", { "simple_vert", "simple_frag" }))
		return false;

	if (!shaders->create_shader_program("crate_program", { "crate_vert", "crate_frag" }))
		return false;

	// Return true, successfully created shaders.
	return true;
}

void clean_memory()
{
	// Delete the camera.
	delete main_camera;
	main_camera = nullptr;

	// Delete each mesh in mesh_list.
	for (size_t i = 0; i < mesh_list.size(); i++)
	{
		delete mesh_list[i];
		mesh_list[i] = nullptr;
	}
	mesh_list.clear();

	// Delete shaders.
	delete shaders;
	shaders = nullptr;
}