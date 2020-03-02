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

	static class MaterialManager
	{
	public:
		~MaterialManager();

		static bool create_material(const char* a_key_name, std::vector<glm::vec3>& a_material_list, const float a_spec_shininess, const float a_alpha);
		static void add_loaded_map(const char* a_key_name, Texture* a_loaded_map, material_map_type a_map_type);

		static Material* get_material(const char* a_key_name);
		static Texture* get_material_map(const char* a_key_name, material_map_type a_map_type);


	private:
		static std::map<std::string, Material*> m_material_map_list;

		static bool does_key_exist(const char* a_key_name);
		static void clean_material();
	};
}