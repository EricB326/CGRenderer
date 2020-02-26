#pragma once

#include "common.h"
#include <map>
#include <string>
#include <vector>
#include "Material.h"

namespace uciniti
{
	class MaterialManager
	{
	public:
		static MaterialManager& inst();

		MaterialManager(const MaterialManager&) = delete;
		void operator=(const MaterialManager&) = delete;

		~MaterialManager();

		bool create_material(const char* a_key_name, std::vector<glm::vec3>& a_material_list, const float a_spec_shininess, const float a_alpha);
		void add_loaded_map(Texture* a_loaded_map);
		Material* get_material(const char* a_key_name) { return m_material_map_list[a_key_name]; }

	private:
		std::map<std::string, Material*> m_material_map_list;

		MaterialManager();
		void clean_material();
	};
}