#pragma once

#include "common.h"
#include <map>
#include <string>
#include <vector>
#include "Material.h"

class MaterialManager
{
public:
	MaterialManager();
	~MaterialManager();

	bool create_material(const char* a_key_name, std::vector<glm::vec3>& a_material_list, const float a_spec_shininess, const float a_alpha);

	Material* get_material(const char* a_key_name) { return m_material_map_list[a_key_name]; }

private:
	std::map<std::string, Material*> m_material_map_list;

	void clean_material();
};

