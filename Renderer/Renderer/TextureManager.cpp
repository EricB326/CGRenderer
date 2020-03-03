#include "TextureManager.h"

/* Graphic includes
*/
#include "gl_core_4_5.h"

namespace uciniti
{
	TextureManager::~TextureManager()
	{
		delete_all_textures();
	}

	bool TextureManager::create_texture(const char* a_key_name, const char* a_filepath)
	{
		Texture* new_texture = new Texture();

		if (!new_texture->load_texture(a_filepath))
		{
			printf("ERROR: create_texture() call. Failed to load texture data: '%s'\n", a_filepath);
			return false;
		}

		m_texture_map_list[a_key_name] = new_texture;

		return true;
	}

	void TextureManager::use_texture(const char* a_key_name, int a_index)
	{
		m_texture_map_list[a_key_name]->bind_texture(a_index);
	}

	Texture* TextureManager::get_texture(const char* a_key_name)
	{
		return m_texture_map_list[a_key_name];;
	}

	void TextureManager::get_all_texture_names()
	{
		int index = 0;
		for (std::pair<std::string, Texture*> this_pair : m_texture_map_list)
		{
			printf("\nTexture%i map name/key: %s", index, this_pair.first.c_str());
			index++;
		}
	}

	void TextureManager::delete_all_textures()
	{
		// Check there is data to delete.
		if (m_texture_map_list.size() > 0)
		{
			// Delete all Material* pointers
			for (std::pair<std::string, Texture*> this_pair : m_texture_map_list)
			{
				delete this_pair.second;
				this_pair.second = nullptr;
			}
		}
	}

	void TextureManager::delete_texture_at(const char* a_key_name)
	{
		if (!does_key_exist(a_key_name))
		{
			printf("ERROR: delete_material_at() call. Cannot find key '%s' in material map.\n", a_key_name);
			return;
		}

		// Locate the key in the map
		auto map_iterator = m_texture_map_list.find(a_key_name);
		// Key located, delete it. No point doing an if .end() check.
		// Already know the key does exist from above.
		delete map_iterator->second;
		map_iterator->second = nullptr;
		m_texture_map_list.erase(map_iterator);
	}

	bool TextureManager::does_key_exist(const char* a_key_name)
	{
		// Using the .find() search for the key passed through.
		// The iterator stores the address of the key value pair.
		auto map_iterator = m_texture_map_list.find(a_key_name);

		// If we reached the end of the m_material_map_list, the key was
		// not found in the map.
		if (map_iterator == m_texture_map_list.end())
			return false; // Return false, key doesn't exist.
		else
			return true; // Return true, key was found.
	}

	std::map<std::string, Texture*> TextureManager::m_texture_map_list;
}