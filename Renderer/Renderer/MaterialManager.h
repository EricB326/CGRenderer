#pragma once

#include "common.h"
#include <map>
#include <string>
#include <vector>
#include "Material.h"

namespace uciniti
{
	enum class material_map_type : uint
	{
		ALPHA_MAP,
		AMBIENT_MAP,
		BUMP_MAP,
		DIFFUSE_MAP,
		DISPLACEMENT_MAP,
		SPECULAR_HIGHLIGHT_MAP,
		SPECULAR_MAP
	};

	class MaterialManager
	{
	public:
		static MaterialManager& inst();

		MaterialManager(const MaterialManager&) = delete;
		void operator=(const MaterialManager&) = delete;

		~MaterialManager();

		bool create_material(const char* a_key_name, std::vector<glm::vec3>& a_material_list, const float a_spec_shininess, const float a_alpha);
		void add_loaded_map(const char* a_key_name, Texture* a_loaded_map, material_map_type a_map_type);

		Material* get_material(const char* a_key_name) { return m_material_map_list[a_key_name]; }
		Texture* get_material_map(const char* a_key_name, material_map_type a_map_type);


	private:
		std::map<std::string, Material*> m_material_map_list;

		MaterialManager();
		bool does_key_exist(const char* a_key_name);
		void clean_material();
	};
}