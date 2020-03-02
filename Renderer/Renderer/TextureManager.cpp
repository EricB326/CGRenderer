#include "TextureManager.h"

/* Graphic includes
*/
#include "gl_core_4_5.h"

namespace uciniti
{
	TextureManager::~TextureManager()
	{
		clean_texture();
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

	void TextureManager::clean_texture()
	{
		// Delete Texture* pointers
		for (std::pair<std::string, Texture*> this_pair : m_texture_map_list)
			delete this_pair.second;
	}

	std::map<std::string, Texture*> TextureManager::m_texture_map_list;
}