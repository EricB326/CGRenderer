#include "Texture.h"

/* Graphic includes
*/
#include "gl_core_4_5.h"

/* External lib includes
*/
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

namespace uciniti
{
	Texture::Texture()
		: m_texture_handle(0), m_width(0), m_height(0), m_bit_depth(0)
	{}

	Texture::Texture(const char* a_filepath)
		: m_texture_handle(0), m_width(0), m_height(0), m_bit_depth(0)
	{
		load_texture(a_filepath);
	}

	bool Texture::load_texture(const char* a_filepath)
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
		// Texture warpping behaviour.
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		// Texture filtering behaviour.
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// Pass texture data to buffer.
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_width, m_height, 0, GL_RGB, GL_UNSIGNED_BYTE, texture_data);
		// Generate texture mipmaps.
		//glGenerateMipmap(GL_TEXTURE_2D);

		// Unbind texture.
		glBindTexture(GL_TEXTURE_2D, 0);

		// Successfully loaded the texture.
		return true;
	}

	void Texture::bind_texture()
	{
		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_texture_handle);
	}

	void Texture::clear_texture()
	{
		if (m_texture_handle != 0)
			glDeleteTextures(1, &m_texture_handle);

		m_texture_handle = 0;
		m_width = 0;
		m_height = 0;
		m_bit_depth = 0;
	}
}