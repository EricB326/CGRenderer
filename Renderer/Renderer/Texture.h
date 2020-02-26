#ifndef TEXTURE_H
#define TEXTURE_H

#include "common.h"

namespace uciniti
{
	class Texture
	{
	public:
		Texture();

		bool load_texture(const char* a_filepath);
		bool bind_texture(int a_index);
		bool use_texture(int a_index) { return bind_texture(a_index); }
		bool bind_multiple_textures(int a_num_of_textures);

		unsigned int get_handle();
		inline int get_width() { return m_width; }
		inline int get_height() { return m_height; }

	private:
		unsigned int m_texture_handle;
		int m_width, m_height;
		int m_bit_depth;

		void clear_texture();
	};
}

#endif // !TEXTURE_H