/* Graphic includes
*/
#include "gl_core_4_5.h"

/* Texture loading lib includes
*/
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

/* User defined includes
*/
#include "Texture.h"

namespace uciniti
{
	Texture::Texture()
		: m_texture_handle(0), m_width(0), m_height(0), m_bit_depth(0)
	{}

	bool Texture::load_texture(const char* a_filepath)
	{
		// Load the texture data.
		unsigned char* texture_data = stbi_load(a_filepath, &m_width, &m_height, &m_bit_depth, STBI_default);
		
		// If there is no texture_data, the file failed to load.
		if (!texture_data)
			return false;

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

		// Unbind texture.
		glBindTexture(GL_TEXTURE_2D, 0);

		// Free the memory of the texture data.
		stbi_image_free(texture_data);

		// Successfully loaded the texture.
		return true;
	}

	bool Texture::bind_texture(int a_index)
	{
		glActiveTexture(GL_TEXTURE0 + a_index);
		glBindTexture(GL_TEXTURE_2D, m_texture_handle);

		return true;
	}

	uint Texture::get_handle()
	{
		return m_texture_handle;
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