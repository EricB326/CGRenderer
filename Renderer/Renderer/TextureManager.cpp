#include "TextureManager.h"

/* Graphic includes
*/
#include "gl_core_4_5.h"

namespace uciniti
{

	TextureManager& TextureManager::inst()
	{
		// If no instance exists, create one.
		static TextureManager inst;

		// Return the instance.
		return inst;
	}

	TextureManager::~TextureManager()
	{
		// Delete Texture* pointers
		for (std::pair<std::string, Texture*> this_pair : m_texture_map_list)
			delete this_pair.second;
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

	void TextureManager::use_all_textures()
	{
		int index = 0;
		for (auto& this_pair : m_texture_map_list)
		{
			this_pair.second->bind_texture(index);
			index++;
		}
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
}