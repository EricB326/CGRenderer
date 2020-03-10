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

		// Update the camera each frame.
		m_main_camera->update(Time::get_delta_time());

		// Set lights data and render.
		set_lights(m_model_matrix, Time::get_current_time());

		// Set lamp data and render.
		set_red_lamp(m_model_matrix, Time::get_total_time(), glm::vec3(0.0f, 0.0f, 0.0f));
		set_blue_lamp(m_model_matrix, Time::get_total_time(), glm::vec3(0.0f, 0.0f, 0.0f));

		// Set soulspear data and render.
		set_soulspear(m_model_matrix, Time::get_total_time());
	}

	void Render::set_lights(glm::mat4 a_model_matrix, double a_current_time)
	{
		// Direction light section.
		// Colours.
		m_shaders->bind_uniform_vector_data(m_shaders->get_program_id("lambertian_program"), "uniform_directional_light.base.ambient_colour", uciniti::uniform_type::UNIFORM_3f, glm::value_ptr(m_main_light->get_ambient_colour()));
		m_shaders->bind_uniform_vector_data(m_shaders->get_program_id("lambertian_program"), "uniform_directional_light.base.diffuse_colour", uciniti::uniform_type::UNIFORM_3f, glm::value_ptr(m_main_light->get_diffuse_colour()));
		m_shaders->bind_uniform_vector_data(m_shaders->get_program_id("lambertian_program"), "uniform_directional_light.base.specular_colour", uciniti::uniform_type::UNIFORM_3f, glm::value_ptr(m_main_light->get_specular_colour()));

		// Intensities.
		m_shaders->bind_uniform_data(m_shaders->get_program_id("lambertian_program"), "uniform_directional_light.base.ambient_intensity", uciniti::uniform_type::UNIFORM_1f, m_main_light->get_ambient_intensity(), 0);
		m_shaders->bind_uniform_data(m_shaders->get_program_id("lambertian_program"), "uniform_directional_light.base.diffuse_intensity", uciniti::uniform_type::UNIFORM_1f, m_main_light->get_diffuse_intensity(), 0);
		m_shaders->bind_uniform_data(m_shaders->get_program_id("lambertian_program"), "uniform_directional_light.base.specular_intensity", uciniti::uniform_type::UNIFORM_1f, m_main_light->get_specular_intensity(), 0);

		// Direction.
		//m_main_light->set_light_direction(glm::vec3(glm::cos(a_current_time), 0.0f, glm::sin(a_current_time)));
		m_shaders->bind_uniform_vector_data(m_shaders->get_program_id("lambertian_program"), "uniform_directional_light.direction", uciniti::uniform_type::UNIFORM_3f, glm::value_ptr(m_main_light->get_light_direction()));

		// Point lights section.
		m_shaders->bind_uniform_data(m_shaders->get_program_id("lambertian_program"), "point_light_count", uciniti::uniform_type::UNIFORM_1i, 0, m_point_light_count);
		for (size_t i = 0; i < m_point_light_count; i++)
		{
			std::string light_index = "point_lights[" + std::to_string(i) + "].";

			// Colours.
			m_shaders->bind_uniform_vector_data(m_shaders->get_program_id("lambertian_program"), (light_index + "base.ambient_colour").c_str(), uciniti::uniform_type::UNIFORM_3f, glm::value_ptr(m_point_lights[i]->get_ambient_colour()));
			m_shaders->bind_uniform_vector_data(m_shaders->get_program_id("lambertian_program"), (light_index + "base.diffuse_colour").c_str(), uciniti::uniform_type::UNIFORM_3f, glm::value_ptr(m_point_lights[i]->get_diffuse_colour()));
			m_shaders->bind_uniform_vector_data(m_shaders->get_program_id("lambertian_program"), (light_index + "base.specular_colour").c_str(), uciniti::uniform_type::UNIFORM_3f, glm::value_ptr(m_point_lights[i]->get_specular_colour()));

			// Intensities.
			m_shaders->bind_uniform_data(m_shaders->get_program_id("lambertian_program"), (light_index + "base.ambient_intensity").c_str(), uciniti::uniform_type::UNIFORM_1f, m_point_lights[i]->get_ambient_intensity(), 0);
			m_shaders->bind_uniform_data(m_shaders->get_program_id("lambertian_program"), (light_index + "base.diffuse_intensity").c_str(), uciniti::uniform_type::UNIFORM_1f, m_point_lights[i]->get_diffuse_intensity(), 0);
			m_shaders->bind_uniform_data(m_shaders->get_program_id("lambertian_program"), (light_index + "base.specular_intensity").c_str(), uciniti::uniform_type::UNIFORM_1f, m_point_lights[i]->get_specular_intensity(), 0);

			// Position.
			m_point_lights[0]->set_light_position(glm::vec3(glm::cos(a_current_time) * 2.0f , 0.0f, (glm::sin(a_current_time) * 2.0f - 3.0f)));
			m_point_lights[1]->set_light_position(glm::vec3(glm::sin(a_current_time * 3.0f), (glm::cos(a_current_time * 3.0f) * 2.0f), (glm::sin(a_current_time * 3.0f) - 3.0f)));
			m_shaders->bind_uniform_vector_data(m_shaders->get_program_id("lambertian_program"), (light_index + "position").c_str(), uciniti::uniform_type::UNIFORM_3f, glm::value_ptr(m_point_lights[i]->get_light_position()));

			// Attenuation.
			m_shaders->bind_uniform_data(m_shaders->get_program_id("lambertian_program"), (light_index + "constant").c_str(), uciniti::uniform_type::UNIFORM_1f, m_point_lights[i]->get_light_constant(), 0);
			m_shaders->bind_uniform_data(m_shaders->get_program_id("lambertian_program"), (light_index + "linear").c_str(), uciniti::uniform_type::UNIFORM_1f, m_point_lights[i]->get_light_linear(), 0);
			m_shaders->bind_uniform_data(m_shaders->get_program_id("lambertian_program"), (light_index + "exponent").c_str(), uciniti::uniform_type::UNIFORM_1f, m_point_lights[i]->get_light_exponent(), 0);
		}
	}

	void Render::set_soulspear(glm::mat4 a_model_matrix, double a_current_time)
	{
		// User created shader program.
		m_shaders->use_program("lambertian_program");

		m_shaders->bind_uniform_matrix_data(m_shaders->get_program_id("lambertian_program"), "projection_view_matrix", uciniti::uniform_type::UNIFORM_MATRIX_4fv, 1, glm::value_ptr(m_main_camera->get_projection_view()), false);
		m_shaders->bind_uniform_data(m_shaders->get_program_id("lambertian_program"), "time", uciniti::uniform_type::UNIFORM_1f, a_current_time, 0);

		a_model_matrix = glm::mat4(1.0f);
		a_model_matrix = glm::translate(a_model_matrix, glm::vec3(0.0f, 0.0f, -3.0f));
		a_model_matrix = glm::rotate(a_model_matrix, glm::radians((float)a_current_time * 30.0f), glm::vec3(0.0f, 1.0f, 0.0f));
		a_model_matrix = glm::scale(a_model_matrix, glm::vec3(0.003f, 0.003f, 0.003f));

		m_shaders->bind_uniform_matrix_data(m_shaders->get_program_id("lambertian_program"), "model_matrix", uciniti::uniform_type::UNIFORM_MATRIX_4fv, 1, glm::value_ptr(a_model_matrix), false);

		glm::mat3 normal_matrix = glm::inverseTranspose(a_model_matrix);
		m_shaders->bind_uniform_matrix_data(m_shaders->get_program_id("lambertian_program"), "normal_matrix", uciniti::uniform_type::UNIFORM_MATRIX_3fv, 1, glm::value_ptr(normal_matrix), false);


		m_shaders->bind_uniform_data<uint>(m_shaders->get_program_id("lambertian_program"), "uniform_material.bump_map", uciniti::uniform_type::UNIFORM_1i, 0.0f, uciniti::MaterialManager::get_material_map("earth_material", uciniti::material_map_type::BUMP_MAP)->get_handle() - 5);
		m_shaders->bind_uniform_data<uint>(m_shaders->get_program_id("lambertian_program"), "uniform_material.albedo_map", uciniti::uniform_type::UNIFORM_1i, 0.0f, uciniti::MaterialManager::get_material_map("earth_material", uciniti::material_map_type::DIFFUSE_MAP)->get_handle() - 5);
		m_shaders->bind_uniform_vector_data(m_shaders->get_program_id("lambertian_program"), "uniform_material.ambient_colour", uciniti::uniform_type::UNIFORM_3f, glm::value_ptr(uciniti::MaterialManager::get_material("earth_material")->get_ambient()));
		m_shaders->bind_uniform_vector_data(m_shaders->get_program_id("lambertian_program"), "uniform_material.diffuse_colour", uciniti::uniform_type::UNIFORM_3f, glm::value_ptr(uciniti::MaterialManager::get_material("earth_material")->get_diffuse()));
		m_shaders->bind_uniform_vector_data(m_shaders->get_program_id("lambertian_program"), "uniform_material.specular_colour", uciniti::uniform_type::UNIFORM_3f, glm::value_ptr(uciniti::MaterialManager::get_material("earth_material")->get_specular()));
		m_shaders->bind_uniform_data(m_shaders->get_program_id("lambertian_program"), "uniform_material.specular_shininess", uciniti::uniform_type::UNIFORM_1f, uciniti::MaterialManager::get_material("earth_material")->get_specular_shininess(), 0);

		TextureManager::use_texture("earth_material_bump_map", 0);
		TextureManager::use_texture("earth_material_diffuse_map", 1);
		m_mesh_list[3]->render_mesh();
	}

	void Render::set_red_lamp(glm::mat4 a_model_matrix, double a_current_time, glm::vec3 a_position)
	{
		// User created shader program.
		m_shaders->use_program("lamp_program");

		m_shaders->bind_uniform_data(m_shaders->get_program_id("lamp_program"), "time", uciniti::uniform_type::UNIFORM_1f, a_current_time, 0);
		m_shaders->bind_uniform_matrix_data(m_shaders->get_program_id("lamp_program"), "projection_view_matrix", uciniti::uniform_type::UNIFORM_MATRIX_4fv, 1, glm::value_ptr(m_main_camera->get_projection_view()), false);		
		m_shaders->bind_uniform_vector_data(m_shaders->get_program_id("lamp_program"), "lamp_colour", uciniti::uniform_type::UNIFORM_4f, glm::value_ptr(glm::vec4(1.0f, 0.0f, 0.0f, 1.0f)));

		a_model_matrix = glm::mat4(1.0f);
		a_model_matrix = glm::translate(a_model_matrix, m_point_lights[0]->get_light_position());
		a_model_matrix = glm::scale(a_model_matrix, glm::vec3(0.01f, 0.01f, 0.01f));

		m_shaders->bind_uniform_matrix_data(m_shaders->get_program_id("lamp_program"), "model_matrix", uciniti::uniform_type::UNIFORM_MATRIX_4fv, 1, glm::value_ptr(a_model_matrix), false);

		m_mesh_list[0]->render_mesh();
	}

	void Render::set_blue_lamp(glm::mat4 a_model_matrix, double a_current_time, glm::vec3 a_position)
	{
		// User created shader program.
		m_shaders->use_program("lamp_program");

		m_shaders->bind_uniform_data(m_shaders->get_program_id("lamp_program"), "time", uciniti::uniform_type::UNIFORM_1f, a_current_time, 0);
		m_shaders->bind_uniform_matrix_data(m_shaders->get_program_id("lamp_program"), "projection_view_matrix", uciniti::uniform_type::UNIFORM_MATRIX_4fv, 1, glm::value_ptr(m_main_camera->get_projection_view()), false);
		m_shaders->bind_uniform_vector_data(m_shaders->get_program_id("lamp_program"), "lamp_colour", uciniti::uniform_type::UNIFORM_4f, glm::value_ptr(glm::vec4(0.0f, 0.0f, 1.0f, 1.0f)));

		a_model_matrix = glm::mat4(1.0f);
		a_model_matrix = glm::translate(a_model_matrix, m_point_lights[1]->get_light_position());
		a_model_matrix = glm::scale(a_model_matrix, glm::vec3(0.01f, 0.01f, 0.01f));

		m_shaders->bind_uniform_matrix_data(m_shaders->get_program_id("lamp_program"), "model_matrix", uciniti::uniform_type::UNIFORM_MATRIX_4fv, 1, glm::value_ptr(a_model_matrix), false);

		m_mesh_list[1]->render_mesh();
	}

	bool Render::create_geometry()
	{
		Mesh* red_lamp = new Mesh();
		Mesh* blue_lamp = new Mesh();
		Mesh* soulspear = new Mesh();
		Mesh* earth = new Mesh();

		if (!red_lamp->load_obj("..//Models//Free3D//Crate//crate.obj", "red_lamp_material")) { return false; }
		if (!blue_lamp->load_obj("..//Models//Free3D//Crate//crate.obj", "blue_lamp_material")) { return false; }
		if (!soulspear->load_obj("..//Models//Soulspear//soulspear.obj", "soulspear_material", true, true)) { return false; }
		if (!earth->load_obj("..//Models//Free3D//Earth//earth.obj", "earth_material", true, true)) { return false; }
		

		m_mesh_list.push_back(red_lamp);
		m_mesh_list.push_back(blue_lamp);
		m_mesh_list.push_back(soulspear);
		m_mesh_list.push_back(earth);


		return true;
	}

	bool Render::create_textures()
	{
		return false;
	}

	bool Render::create_shaders()
	{
		m_shaders = new ShaderManager();

		// Load lamp shader.
		if (!m_shaders->load_shader("lamp_vert", uciniti::shader_type::VERTEX, "..//Shaders//lamp_vert.glsl")) { return false; }
		if (!m_shaders->load_shader("lamp_frag", uciniti::shader_type::FRAGMENT, "..//Shaders//lamp_frag.glsl")) { return false; }

		// Load Lambertian shader.
		if (!m_shaders->load_shader("lambertian_vert", uciniti::shader_type::VERTEX, "..//Shaders//lambertian_vert.glsl")) { return false; }
		if (!m_shaders->load_shader("lambertian_frag", uciniti::shader_type::FRAGMENT, "..//Shaders//lambertian_frag.glsl")) { return false; }
		
		// Load soulspear shader.
		if (!m_shaders->load_shader("soulspear_vert", uciniti::shader_type::VERTEX, "..//Shaders//soulspear_vert.glsl")) { return false; }
		if (!m_shaders->load_shader("soulspear_frag", uciniti::shader_type::FRAGMENT, "..//Shaders//soulspear_frag.glsl")) { return false; }		

		// Create programs.
		if (!m_shaders->create_shader_program("lamp_program", { "lamp_vert", "lamp_frag" })) { return false; }
		if (!m_shaders->create_shader_program("lambertian_program", { "lambertian_vert", "lambertian_frag" })) { return false; }
		if (!m_shaders->create_shader_program("soulspear_program", { "soulspear_vert", "soulspear_frag" })) { return false; }

		return true;
	}

	bool Render::create_lights()
	{
		// White, low intensity, "sun" directional light.
		m_main_light = new DirectionalLight(glm::vec3(0.0f, 0.0f, -1.0f),
			glm::vec3(.1f, .1f, .1f),
			glm::vec3(1.0f, 1.0f, 1.0f),
			glm::vec3(1.0f, 1.0f, 1.0f),
			0.1f, 0.3f, 0.4f);

		// Red point light.
		m_point_lights[0] = new PointLight(glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(1.0f, 0.09f, 0.032f),
			glm::vec3(1.0f, 0.0f, 0.0f),
			glm::vec3(1.0f, 0.0f, 0.0f),
			glm::vec3(1.0f, 0.0f, 0.0f),
			1.0f, 1.0f, 0.6f);
		m_point_light_count++;

		// Blue point light.
		m_point_lights[1] = new PointLight(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(1.0f, 0.09f, 0.032f),
			glm::vec3(0.0f, 0.0f, 1.0f),
			glm::vec3(0.0f, 0.0f, 1.0f),
			glm::vec3(0.0f, 0.0f, 1.0f),
			0.1f, 1.0f, 0.6f);
		m_point_light_count++;

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

		for (auto point_light : m_point_lights)
		{
			delete point_light;
			point_light = nullptr;
		}
	}
}