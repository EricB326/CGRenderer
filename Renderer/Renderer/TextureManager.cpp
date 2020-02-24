#include "TextureManager.h"

/* Graphic includes
*/
#include "gl_core_4_5.h"

/* External lib includes
*/
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

namespace uciniti
{

	TextureManager& TextureManager::inst()
	{
		// If no instance exists, create one.
		static TextureManager inst;

		// Return the instance.
		return inst;
	}

	TextureManager::TextureManager()
		: m_texture_handle(0), m_width(0), m_height(0), m_bit_depth(0)
	{}

	bool TextureManager::load_texture(const char* a_filepath, uciniti::texture_id a_texture_map_key)
	{
		// Load the texture data.
		unsigned char* texture_data = stbi_load(a_filepath, &m_width, &m_height, &m_bit_depth, STBI_default);

		// If there is no texture_data, the file failed to load.
		if (!texture_data)
		{
			printf("ERROR: load_texture() call. Failed to load texture data: '%s'\n", a_filepath);
			return false;
		}

		// Create and bind buffer.
		glGenTextures(1, &m_texture_handle);
		glBindTexture(GL_TEXTURE_2D, m_texture_handle);

		// Specifiy texture parameters.
		// Texture wrapping behaviour.
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		// Texture filtering behaviour.
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// Pass texture data to buffer.
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_width, m_height, 0, GL_RGB, GL_UNSIGNED_BYTE, texture_data);
		// Generate texture mipmaps.
		glGenerateMipmap(GL_TEXTURE_2D);

		// Place the generated texture ID in the map at the correct texture key.
		m_texture_id_list[(uint)a_texture_map_key] = m_texture_handle;

		// Unbind texture.
		glBindTexture(GL_TEXTURE_2D, 0);

		// Free the memory of the texture data.
		stbi_image_free(texture_data);

		// Successfully loaded the texture.
		return true;
	}

	bool TextureManager::bind_texture(uciniti::texture_id a_texture_map_key)
	{
		if (m_texture_id_list.find((uint)a_texture_map_key) != m_texture_id_list.end())
		{
			glActiveTexture(GL_TEXTURE0);
			glBindTexture(GL_TEXTURE_2D, get_handle(a_texture_map_key));

			return true;
		}

		return false;
	}

	uint TextureManager::get_handle(uciniti::texture_id a_texture_key)
	{
		for (std::pair<uint, uint> this_pair : m_texture_id_list)
		{
			if (this_pair.first == (uint)a_texture_key)
				return this_pair.second;
		}

		return 0;
	}

	void TextureManager::clear_texture()
	{
		if (m_texture_handle != 0)
			glDeleteTextures(1, &m_texture_handle);

		m_texture_handle = 0;
		m_width = 0;
		m_height = 0;
		m_bit_depth = 0;
	}
}