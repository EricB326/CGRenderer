#include "MaterialManager.h"
#include "Material.h"

namespace uciniti
{
	MaterialManager::~MaterialManager()
	{
		delete_all_materials();
	}

	bool MaterialManager::create_material(const char* a_key_name, std::vector<glm::vec3>& a_material_list, const float a_spec_shininess, const float a_alpha)
	{
		if (does_key_exist(a_key_name))
		{
			printf("ERROR: create_material() call. Material key name '%s' already exists.\n", a_key_name);
			return false;
		}

		Material* new_material = new Material();
		new_material->set_mat_attributes(a_material_list[0], a_material_list[1], a_material_list[2], a_material_list[3], a_spec_shininess, a_alpha);

		m_material_map_list[a_key_name] = new_material;

		return true;
	}

	void MaterialManager::add_loaded_map(const char* a_key_name, Texture* a_loaded_map, material_map_type a_map_type)
	{
		// Verify the key actually exists to have a map
		// added to it.
		if (!does_key_exist(a_key_name))
		{
			printf("ERROR: add_loaded_map() call. Could not find any material map key: '%s'.\n", a_key_name);
			return;
		}

		// Add the correct map type.
		switch (a_map_type)
		{
		case material_map_type::ALPHA_MAP:
			m_material_map_list[a_key_name]->set_alpha_map(a_loaded_map);
			break;
		case material_map_type::AMBIENT_MAP:
			m_material_map_list[a_key_name]->set_ambient_map(a_loaded_map);
			break;
		case material_map_type::BUMP_MAP:
			m_material_map_list[a_key_name]->set_bump_map(a_loaded_map);
			break;
		case material_map_type::DIFFUSE_MAP:
			m_material_map_list[a_key_name]->set_diffuse_map(a_loaded_map);
			break;
		case material_map_type::DISPLACEMENT_MAP:
			m_material_map_list[a_key_name]->set_displacement_map(a_loaded_map);
			break;
		case material_map_type::SPECULAR_HIGHLIGHT_MAP:
			m_material_map_list[a_key_name]->set_specular_highlight_map(a_loaded_map);
			break;
		case material_map_type::SPECULAR_MAP:
			m_material_map_list[a_key_name]->set_specular_map(a_loaded_map);
			break;
		default:
			printf("ERROR: add_loaded_map() call. Could not find specified material map type.\n");
			return;
		}
	}

	Material* MaterialManager::get_material(const char* a_key_name)
	{
		return m_material_map_list[a_key_name];
	}

	Texture* MaterialManager::get_material_map(const char* a_key_name, material_map_type a_map_type)
	{
		// Verify the key actually exists to have a map
		// added to it.
		if (!does_key_exist(a_key_name))
		{
			printf("ERROR: get_material_map() call. Could not find any material map key: '%s'.\n", a_key_name);
			return nullptr;
		}

		switch (a_map_type)
		{
		case material_map_type::ALPHA_MAP:
			return m_material_map_list[a_key_name]->get_alpha_map();
		case material_map_type::AMBIENT_MAP:
			return m_material_map_list[a_key_name]->get_ambient_map();
		case material_map_type::BUMP_MAP:
			return m_material_map_list[a_key_name]->get_bump_map();
		case material_map_type::DIFFUSE_MAP:
			return m_material_map_list[a_key_name]->get_diffuse_map();
		case material_map_type::DISPLACEMENT_MAP:
			return m_material_map_list[a_key_name]->get_displacement_map();
		case material_map_type::SPECULAR_HIGHLIGHT_MAP:
			return m_material_map_list[a_key_name]->get_specular_highlight_map();
		case material_map_type::SPECULAR_MAP:
			return m_material_map_list[a_key_name]->get_specular_map();
		default:
			printf("ERROR: get_material_map() call. Could not find specified material map type.\n");
			return nullptr;
		}
	}

	bool MaterialManager::does_key_exist(const char* a_key_name)
	{
		// Using the .find() search for the key passed through.
		// The iterator stores the address of the key value pair.
		auto map_iterator = m_material_map_list.find(a_key_name);

		// If we reached the end of the m_material_map_list, the key was
		// not found in the map.
		if (map_iterator == m_material_map_list.end())
			return false; // Return false, key doesn't exist.
		else
			return true; // Return true, key was found.
	}

	void MaterialManager::delete_all_materials()
	{
		// Check there is data to delete.
		if (m_material_map_list.size() > 0)
		{
			// Delete all Material* pointers
			for (std::pair<std::string, Material*> this_pair : m_material_map_list)
			{
				delete this_pair.second;
				this_pair.second = nullptr;
			}
		}
	}

	void MaterialManager::delete_material_at(const char* a_key_name)
	{
		if (!does_key_exist(a_key_name))
		{
			printf("ERROR: delete_material_at() call. Cannot find key '%s' in material map.\n", a_key_name);
			return;
		}

		// Locate the key in the map
		auto map_iterator = m_material_map_list.find(a_key_name);
		// Key located, delete it. No point doing an if .end() check.
		// Already know the key does exist from above.
		delete map_iterator->second;
		map_iterator->second = nullptr;
		m_material_map_list.erase(map_iterator);
	}

	std::map<std::string, Material*> MaterialManager::m_material_map_list;
}