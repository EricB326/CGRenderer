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
		bool bind_texture();
		bool use_texture() { return bind_texture(); }

		uint get_handle();
		inline int get_width() { return m_width; }
		inline int get_height() { return m_height; }

	private:
		uint m_texture_handle;
		int m_width, m_height;
		int m_bit_depth;

		void clear_texture();
	};
}

#endif // !TEXTURE_H