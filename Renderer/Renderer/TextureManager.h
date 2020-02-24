#ifndef TEXTURE_H
#define TEXTURE_H

#include "common.h"
#include <map>

namespace uciniti
{
	enum class texture_id
	{
		BACKGROUND_TEXTURE_ID,
		GROUND_TEXTURE_ID,
		CRATE_TEXTURE_ID,
		BRICK_TEXTURE_ID
	};

	class TextureManager
	{

	public:
		static TextureManager& inst();

		TextureManager(const TextureManager&) = delete;
		void operator=(const TextureManager&) = delete;


		~TextureManager() { clear_texture(); }

		bool load_texture(const char* a_filepath, uciniti::texture_id a_texture_map_key);

		bool bind_texture(uciniti::texture_id a_texture_map_key);
		bool use_texture(uciniti::texture_id a_texture_map_key) { return bind_texture(a_texture_map_key); }

		inline uint get_handle() { return m_texture_handle; }
		inline int get_width() { return m_width; }
		inline int get_height() { return m_height; }


	protected:
		std::map<uint, uint> m_texture_id_list;

		uint m_texture_handle;
		int m_width, m_height;
		int m_bit_depth;

		TextureManager();

		void clear_texture();
		void reset_instance();
	};
}

#endif // !TEXTURE_H