/* User defined includes
*/
#include "Application.h"

/* @brief Beginning of the rendering software, allows the user to create
	a new application. With the new application they can start the rendering
	loop which will take care of calling appropriate functions throught the 
	programs runtime. Classes within this application are typically held within
	the "uciniti" namesapce.
*/
int main()
{
	// Check for memory leaks.
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	// Create a new rendering application.
	auto rendering_app = new uciniti::Application();

	// Initialize rendering application and being rendering loop.
	rendering_app->start("Uciniti", 1920, 1080);

	// Delete the rendering application when exited.
	delete rendering_app;
	rendering_app = nullptr;

	return 0;
}

//void set_soulspear(glm::mat4 a_model_matrix, double a_current_time)
//{
//	// User created shader program.
//	shaders->use_program("soulspear_program");
//
//	shaders->bind_uniform_data(shaders->get_program_id("soulspear_program"), "time", uciniti::uniform_type::UNIFORM_1f, a_current_time, 0);
//	shaders->bind_uniform_matrix_data(shaders->get_program_id("soulspear_program"), "projection_view_matrix", uciniti::uniform_type::UNIFORM_MATRIX_4fv, 1, glm::value_ptr(main_camera->get_projection_view()), false);
//	shaders->bind_uniform_matrix_data(shaders->get_program_id("soulspear_program"), "view_matrix", uciniti::uniform_type::UNIFORM_MATRIX_4fv, 1, glm::value_ptr(main_camera->get_view_matrix()), false);
//	shaders->bind_uniform_matrix_data(shaders->get_program_id("soulspear_program"), "projection_matrix", uciniti::uniform_type::UNIFORM_MATRIX_4fv, 1, glm::value_ptr(main_camera->get_projection()), false);
//
//	// Light section.
//	shaders->bind_uniform_vector_data(shaders->get_program_id("soulspear_program"), "uniform_directional_light.base.light_ambient_colour", uciniti::uniform_type::UNIFORM_3f, glm::value_ptr(main_light.get_ambient_colour()));
//	shaders->bind_uniform_vector_data(shaders->get_program_id("soulspear_program"), "uniform_directional_light.base.light_diffuse_colour", uciniti::uniform_type::UNIFORM_3f, glm::value_ptr(main_light.get_diffuse_colour()));
//	shaders->bind_uniform_vector_data(shaders->get_program_id("soulspear_program"), "uniform_directional_light.base.light_specular_colour", uciniti::uniform_type::UNIFORM_3f, glm::value_ptr(main_light.get_specular_colour()));
//	shaders->bind_uniform_data(shaders->get_program_id("soulspear_program"), "uniform_directional_light.base.light_ambient_intensity", uciniti::uniform_type::UNIFORM_1f, main_light.get_ambient_intensity(), 0);
//	shaders->bind_uniform_data(shaders->get_program_id("soulspear_program"), "uniform_directional_light.base.light_diffuse_intensity", uciniti::uniform_type::UNIFORM_1f, main_light.get_diffuse_intensity(), 0);
//	shaders->bind_uniform_data(shaders->get_program_id("soulspear_program"), "uniform_directional_light.base.light_specular_intensity", uciniti::uniform_type::UNIFORM_1f, main_light.get_specular_intensity(), 0);
//
//	//main_light.set_light_direction(glm::normalize(glm::vec3(0.0f, glm::sin(a_current_time * 1.0f), glm::cos(a_current_time * 1.0f))));
//	shaders->bind_uniform_vector_data(shaders->get_program_id("soulspear_program"), "uniform_directional_light.light_direction", uciniti::uniform_type::UNIFORM_3f, glm::value_ptr(main_light.get_light_direction()));
//
//	a_model_matrix = glm::mat4(1.0f);
//	a_model_matrix = glm::translate(a_model_matrix, glm::vec3(0.0f, -1.0f, -3.0f));
//	//a_model_matrix = glm::rotate(a_model_matrix, glm::radians((float)a_current_time * 30.0f), glm::vec3(0.0f, 1.0f, 0.0f));
//	//model = glm::scale(model, glm::vec3(0.01f, 0.01f, 0.01f));
//
//	shaders->bind_uniform_matrix_data(shaders->get_program_id("soulspear_program"), "model_matrix", uciniti::uniform_type::UNIFORM_MATRIX_4fv, 1, glm::value_ptr(a_model_matrix), false);
//
//	shaders->bind_uniform_data<uint>(shaders->get_program_id("soulspear_program"), "uniform_material.bump_map", uciniti::uniform_type::UNIFORM_1i, 0.0f, uciniti::MaterialManager::get_material_map("soulspear_material", uciniti::material_map_type::BUMP_MAP)->get_handle() - 1);
//	shaders->bind_uniform_data<uint>(shaders->get_program_id("soulspear_program"), "uniform_material.diffuse_map", uciniti::uniform_type::UNIFORM_1i, 0.0f, uciniti::MaterialManager::get_material_map("soulspear_material", uciniti::material_map_type::DIFFUSE_MAP)->get_handle() - 1);
//	shaders->bind_uniform_data<uint>(shaders->get_program_id("soulspear_program"), "uniform_material.specular_map", uciniti::uniform_type::UNIFORM_1i, 0.0f, uciniti::MaterialManager::get_material_map("soulspear_material", uciniti::material_map_type::SPECULAR_MAP)->get_handle() - 1);
//	shaders->bind_uniform_vector_data(shaders->get_program_id("soulspear_program"), "uniform_material.mat_ambient_colour", uciniti::uniform_type::UNIFORM_3f, glm::value_ptr(uciniti::MaterialManager::get_material("soulspear_material")->get_ambient()));
//	shaders->bind_uniform_vector_data(shaders->get_program_id("soulspear_program"), "uniform_material.mat_diffuse_colour", uciniti::uniform_type::UNIFORM_3f, glm::value_ptr(uciniti::MaterialManager::get_material("soulspear_material")->get_diffuse()));
//	shaders->bind_uniform_vector_data(shaders->get_program_id("soulspear_program"), "uniform_material.mat_specular_colour", uciniti::uniform_type::UNIFORM_3f, glm::value_ptr(uciniti::MaterialManager::get_material("soulspear_material")->get_specular()));
//	shaders->bind_uniform_data(shaders->get_program_id("soulspear_program"), "uniform_material.mat_specular_shininess", uciniti::uniform_type::UNIFORM_1f, uciniti::MaterialManager::get_material("soulspear_material")->get_specular_shininess(), 0);
//
//	texture.use_texture("bump_map", 0);
//	texture.use_texture("diffuse_map", 1);
//	texture.use_texture("specular_map", 2);
//	mesh_list[1]->render_mesh();
//}
//bool create_geometry()
//{
//	/*** Mesh data ***/
//	// Large floor plane points.
//	//								Positions             Colour
//	//							 X      Y       Z      R      G     B
//	GLfloat floor_verts[] = { -10.0f,  0.0f, -10.0f,  0.0f,  0.0f,  0.0f,   // Back Left
//							   10.0f,  0.0f, -10.0f,  0.0f,  0.0f,  0.0f,   // Back Right
//							  -10.0f,  0.0f,  10.0f,  0.0f,  0.0f,  0.0f,   // Front Left
//							   10.0f,  0.0f,  10.0f,  0.0f,  0.0f,  0.0f }; // Front Right
//
//	GLuint floor_indices[] = { 1, 0, 2, 2, 3, 1 };
//
//	/*** Create and 'load' floor mesh ***/
//	uciniti::Mesh* floor_object = new uciniti::Mesh(floor_verts, floor_indices, 24, 6, vertex_type::BASE_VERTEX);
//	mesh_list.push_back(floor_object);
//
//	// Create loaded .obj model.
//	//uciniti::Mesh* alien_bug = new uciniti::Mesh();
//	uciniti::Mesh* soulspear = new uciniti::Mesh();
//	//uciniti::Mesh* stanford_bunny = new uciniti::Mesh();
//	//uciniti::Mesh* stanford_dragon = new uciniti::Mesh();
//	//uciniti::Mesh* ivysaur = new uciniti::Mesh();
//	uciniti::Mesh* crate = new uciniti::Mesh();
//	//uciniti::Mesh* mayan_house = new uciniti::Mesh();
//
//	//bool loaded = alien_bug->load_obj("..//Models//KazChesna//Alienbug_LP.obj");
//	if (!soulspear->load_obj("..//Models//Soulspear//soulspear.obj", "soulspear_material", true, true)) { return false; }
//	//if (!stanford_bunny->load_obj("..//Models//Stanford//Bunny.obj", "bunny")) { return false; }
//	//if (!stanford_dragon->load_obj("..//Models//Stanford//Dragon.obj", "dragon_material")) { return false; }
//	//if (!ivysaur->load_obj("..//Models//Free3d//Ivysaur//ivysaur.obj", "ivysaur_material", true, true)) { return false; }
//	if (!crate->load_obj("..//Models//Free3D//Crate//Cube.obj", "crate_material")) { return false; }
//	//if (!mayan_house->load_obj("..//Models//ChallengeModel//MayanHouse//MayanHouse.obj", "mayan_house")) { return false; }
//
//	// Add the model to the list if successful.
//	//mesh_list.push_back(alien_bug);
//	mesh_list.push_back(soulspear);
//	//mesh_list.push_back(stanford_bunny);
//	//mesh_list.push_back(stanford_dragon);
//	//mesh_list.push_back(ivysaur);
//	mesh_list.push_back(crate);
//	//mesh_list.push_back(mayan_house);
//
//	return true;
//}
//bool create_textures()
//{
//	// Error checking variable for if textures were loaded.
//	//bool did_texture_load = false;
//	//
//	//// Make sure there is an instance of the texture class.
//	//uciniti::TextureManager& texture = uciniti::TextureManager::inst();
//	//
//	//// Begin loading textures.
//	//// Load crate texture.
//	//did_texture_load = texture.create_texture("crate_texture", "..//Textures//wooden_crate.jpg");
//	//if (!did_texture_load)
//	//	return false;
//	//
//	//// Load brick texture.
//	//did_texture_load = texture.create_texture("brick_texture", "..//Textures//brick.jpg");
//	//if (!did_texture_load)
//	//	return false;
//	//brick-layed-normal-map.png
//	if (!texture.create_texture("brickwall_texture", "..//Textures//brickwall.jpg"))
//		return false;
//	//if (!texture.create_texture("brickwall_normal", "..//Textures//brickwall_normal.jpg"))
//	if (!texture.create_texture("brickwall_normal", "..//Textures//brick-layed-normal-map.png"))
//		return false;
//	uciniti::MaterialManager::add_loaded_map("crate_material", texture.get_texture("brickwall_normal"), uciniti::material_map_type::BUMP_MAP);
//
//	return true;
//}