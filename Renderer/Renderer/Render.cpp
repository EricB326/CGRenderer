#include "Render.h"

#include "Application.h"
#include "Time.h"

namespace uciniti
{
	Render::Render()
		: m_main_camera(nullptr), m_shaders(nullptr), m_main_light(nullptr), m_model_matrix(glm::mat4(1.0f))
	{}

	Render::~Render()
	{
		clean_memory();
	}

	void Render::start()
	{
		if (!create_geometry())
		{
			printf("ERROR: create_geometry() call. Failed to create all geometry.\n");
			clean_memory();
			return;
		}

		//create_textures();

		if (!create_shaders())
		{
			printf("ERROR: create_shaders() call. Failed to create all shaders.\n");
			clean_memory();
			return;
		}

		if (!create_lights())
		{
			printf("ERROR: create_lights() call. Failed to create all lights.\n");
			clean_memory();
			return;
		}

		m_main_camera = new FreeCamera();
		m_main_camera->set_perspective(glm::radians(75.0f), (float)WIDTH / (float)HEIGHT, 0.01f, 100.0f);
		m_main_camera->set_look_at(glm::vec3(0.0f, 1.0f, 3.0f), glm::vec3(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	}

	void Render::update()
	{
		// Clear the different buffer bits.
		clear_screen();
		// Enable test types (depth test, etc).
		enable_test_types();

		m_main_light->set_light_direction(glm::vec3(cosf((float)Time::get_total_time()), 0.0f, sinf((float)Time::get_total_time())));

		// Update the camera each frame.
		m_main_camera->update(Time::get_delta_time());

		set_soulspear(m_model_matrix, Time::get_total_time());
	}

	void Render::set_soulspear(glm::mat4 a_model_matrix, double a_current_time)
	{
		// User created shader program.
		m_shaders->use_program("soulspear_program");

		m_shaders->bind_uniform_data(m_shaders->get_program_id("soulspear_program"), "time", uciniti::uniform_type::UNIFORM_1f, a_current_time, 0);
		m_shaders->bind_uniform_matrix_data(m_shaders->get_program_id("soulspear_program"), "projection_view_matrix", uciniti::uniform_type::UNIFORM_MATRIX_4fv, 1, glm::value_ptr(m_main_camera->get_projection_view()), false);
		m_shaders->bind_uniform_matrix_data(m_shaders->get_program_id("soulspear_program"), "view_matrix", uciniti::uniform_type::UNIFORM_MATRIX_4fv, 1, glm::value_ptr(m_main_camera->get_view_matrix()), false);
		m_shaders->bind_uniform_matrix_data(m_shaders->get_program_id("soulspear_program"), "projection_matrix", uciniti::uniform_type::UNIFORM_MATRIX_4fv, 1, glm::value_ptr(m_main_camera->get_projection()), false);

		// Light section.
		m_shaders->bind_uniform_vector_data(m_shaders->get_program_id("soulspear_program"), "uniform_directional_light.base.light_ambient_colour", uciniti::uniform_type::UNIFORM_3f, glm::value_ptr(m_main_light->get_ambient_colour()));
		m_shaders->bind_uniform_vector_data(m_shaders->get_program_id("soulspear_program"), "uniform_directional_light.base.light_diffuse_colour", uciniti::uniform_type::UNIFORM_3f, glm::value_ptr(m_main_light->get_diffuse_colour()));
		m_shaders->bind_uniform_vector_data(m_shaders->get_program_id("soulspear_program"), "uniform_directional_light.base.light_specular_colour", uciniti::uniform_type::UNIFORM_3f, glm::value_ptr(m_main_light->get_specular_colour()));
		m_shaders->bind_uniform_data(m_shaders->get_program_id("soulspear_program"), "uniform_directional_light.base.light_ambient_intensity", uciniti::uniform_type::UNIFORM_1f, m_main_light->get_ambient_intensity(), 0);
		m_shaders->bind_uniform_data(m_shaders->get_program_id("soulspear_program"), "uniform_directional_light.base.light_diffuse_intensity", uciniti::uniform_type::UNIFORM_1f, m_main_light->get_diffuse_intensity(), 0);
		m_shaders->bind_uniform_data(m_shaders->get_program_id("soulspear_program"), "uniform_directional_light.base.light_specular_intensity", uciniti::uniform_type::UNIFORM_1f, m_main_light->get_specular_intensity(), 0);

		//main_light.set_light_direction(glm::normalize(glm::vec3(0.0f, glm::sin(a_current_time * 1.0f), glm::cos(a_current_time * 1.0f))));
		m_shaders->bind_uniform_vector_data(m_shaders->get_program_id("soulspear_program"), "uniform_directional_light.light_direction", uciniti::uniform_type::UNIFORM_3f, glm::value_ptr(m_main_light->get_light_direction()));

		a_model_matrix = glm::mat4(1.0f);
		a_model_matrix = glm::translate(a_model_matrix, glm::vec3(0.0f, -1.0f, -3.0f));
		//a_model_matrix = glm::rotate(a_model_matrix, glm::radians((float)a_current_time * 30.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		//model = glm::scale(model, glm::vec3(0.01f, 0.01f, 0.01f));

		m_shaders->bind_uniform_matrix_data(m_shaders->get_program_id("soulspear_program"), "model_matrix", uciniti::uniform_type::UNIFORM_MATRIX_4fv, 1, glm::value_ptr(a_model_matrix), false);

		glm::mat3 normal_matrix = glm::inverseTranspose(a_model_matrix);
		m_shaders->bind_uniform_matrix_data(m_shaders->get_program_id("soulspear_program"), "normal_matrix", uciniti::uniform_type::UNIFORM_MATRIX_3fv, 1, glm::value_ptr(normal_matrix), false);


		m_shaders->bind_uniform_data<uint>(m_shaders->get_program_id("soulspear_program"), "uniform_material.bump_map", uciniti::uniform_type::UNIFORM_1i, 0.0f, uciniti::MaterialManager::get_material_map("soulspear_material", uciniti::material_map_type::BUMP_MAP)->get_handle() - 1);
		m_shaders->bind_uniform_data<uint>(m_shaders->get_program_id("soulspear_program"), "uniform_material.diffuse_map", uciniti::uniform_type::UNIFORM_1i, 0.0f, uciniti::MaterialManager::get_material_map("soulspear_material", uciniti::material_map_type::DIFFUSE_MAP)->get_handle() - 1);
		m_shaders->bind_uniform_data<uint>(m_shaders->get_program_id("soulspear_program"), "uniform_material.specular_map", uciniti::uniform_type::UNIFORM_1i, 0.0f, uciniti::MaterialManager::get_material_map("soulspear_material", uciniti::material_map_type::SPECULAR_MAP)->get_handle() - 1);
		m_shaders->bind_uniform_vector_data(m_shaders->get_program_id("soulspear_program"), "uniform_material.mat_ambient_colour", uciniti::uniform_type::UNIFORM_3f, glm::value_ptr(uciniti::MaterialManager::get_material("soulspear_material")->get_ambient()));
		m_shaders->bind_uniform_vector_data(m_shaders->get_program_id("soulspear_program"), "uniform_material.mat_diffuse_colour", uciniti::uniform_type::UNIFORM_3f, glm::value_ptr(uciniti::MaterialManager::get_material("soulspear_material")->get_diffuse()));
		m_shaders->bind_uniform_vector_data(m_shaders->get_program_id("soulspear_program"), "uniform_material.mat_specular_colour", uciniti::uniform_type::UNIFORM_3f, glm::value_ptr(uciniti::MaterialManager::get_material("soulspear_material")->get_specular()));
		m_shaders->bind_uniform_data(m_shaders->get_program_id("soulspear_program"), "uniform_material.mat_specular_shininess", uciniti::uniform_type::UNIFORM_1f, uciniti::MaterialManager::get_material("soulspear_material")->get_specular_shininess(), 0);

		TextureManager::use_texture("bump_map", 0);
		TextureManager::use_texture("diffuse_map", 1);
		TextureManager::use_texture("specular_map", 2);
		m_mesh_list[0]->render_mesh();
	}

	bool Render::create_geometry()
	{
		Mesh* soulspear = new Mesh();

		if (!soulspear->load_obj("..//Models//Soulspear//soulspear.obj", "soulspear_material", true, true)) { return false; }

		m_mesh_list.push_back(soulspear);

		return true;
	}

	bool Render::create_textures()
	{
		return false;
	}

	bool Render::create_shaders()
	{
		m_shaders = new ShaderManager();

		// Load soulspear shader.
		if (!m_shaders->load_shader("soulspear_vert", uciniti::shader_type::VERTEX, "..//Shaders//soulspear_vert.glsl")) { return false; }
		if (!m_shaders->load_shader("soulspear_frag", uciniti::shader_type::FRAGMENT, "..//Shaders//soulspear_frag.glsl")) { return false; }

		// Create programs.
		if (!m_shaders->create_shader_program("soulspear_program", { "soulspear_vert", "soulspear_frag" })) { return false; }

		return true;
	}

	bool Render::create_lights()
	{
		m_main_light = new DirectionalLight(glm::vec3(1.0f, 0.0f, -1.0f),
											glm::vec3(1.0f, 1.0f, 1.0f),
											glm::vec3(0.61424f, 0.04136f, 0.04136f),
											glm::vec3(0.727811f, 0.626959f, 0.626959f),
											0.4f, 1.0f, 1.0f);
		

		//m_main_light = new DirectionalLight(glm::vec3(1.0f, 0.0f, -1.0f),
		//	glm::vec3(1.0f, 1.0f, 1.0f),
		//	glm::vec3(1.0f, 1.0f, 1.0f),
		//	glm::vec3(1.0f, 1.0f, 1.0f),
		//	1.0f, 1.0f, 1.0f);

		return true;
	}


	void Render::clear_screen()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	}

	void Render::enable_test_types()
	{
		glEnable(GL_DEPTH_TEST);
	}

	void Render::clean_memory()
	{
		// Destroy all pointers.
		delete m_main_camera;
		m_main_camera = nullptr;

		delete m_shaders;
		m_shaders = nullptr;

		delete m_main_light;
		m_main_light = nullptr;

		MaterialManager::delete_all_materials();
		TextureManager::delete_all_textures();

		for (auto mesh : m_mesh_list)
		{
			delete mesh;
			mesh = nullptr;
		}
	}
}