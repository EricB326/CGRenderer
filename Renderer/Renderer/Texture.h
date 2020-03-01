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

		uint get_handle();
		uint get_sampler_id();
		inline int get_width() { return m_width; }
		inline int get_height() { return m_height; }

	private:
		uint m_texture_handle;
		uint m_texture_sampler_id;
		int m_width, m_height;
		int m_bit_depth;

		void clear_texture();
	};
}

#endif // !TEXTURE_H