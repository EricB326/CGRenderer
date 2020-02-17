#ifndef TEXTURE_H
#define TEXTURE_H

#include "common.h"

namespace uciniti
{
	class Texture
	{
	public:
		Texture();
		Texture(const char* a_filepath);
		~Texture() { clear_texture(); }

		bool load_texture(const char* a_filepath);

		void bind_texture();
		void use_texture() { bind_texture(); }

		inline uint get_handle() { return m_texture_handle; }
		inline int get_width() { return m_width; }
		inline int get_height() { return m_height; }

		void clear_texture();

	private:
		uint m_texture_handle;
		int m_width, m_height;
		int m_bit_depth;
	};
}

#endif // !TEXTURE_H