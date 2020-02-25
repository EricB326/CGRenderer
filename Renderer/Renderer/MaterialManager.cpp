#include "MaterialManager.h"
#include "Material.h"

MaterialManager::MaterialManager()
{}

MaterialManager::~MaterialManager()
{
	clean_material();
}

bool MaterialManager::create_material(const char* a_key_name, std::vector<glm::vec3>& a_material_list, const float a_spec_shininess, const float a_alpha)
{
	Material* new_material = new Material();
	
	new_material->set_mat_attributes(a_material_list[0], a_material_list[1], a_material_list[2], a_material_list[3], a_spec_shininess, a_alpha);

	m_material_map_list[a_key_name] = new_material;

	return true;
}

void MaterialManager::clean_material()
{
	// Delete Material* pointers
	for (std::pair<std::string, Material*> this_pair : m_material_map_list)
		delete this_pair.second;
}
