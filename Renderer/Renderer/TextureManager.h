#ifndef TEXTURE_MANAGER_H
#define TEXTURE_MANAGER_H

#include "common.h"
#include "Texture.h"
#include <map>
#include <string>

namespace uciniti
{
	class TextureManager
	{
	public:
		static TextureManager& inst();

		TextureManager(const TextureManager&) = delete;
		void operator=(const TextureManager&) = delete;

		~TextureManager();

		bool create_texture(const char* a_key_name, const char* a_filepath);
		Texture* get_texture(const char* a_key_name) { return m_texture_map_list[a_key_name]; }
		void use_texture(const char* a_key_name, int a_index) { m_texture_map_list[a_key_name]->bind_texture(a_index); }
		void use_all_textures();
		bool clear_texture(const char* a_key_name);

		void get_all_texture_names();

	private:
		std::map<std::string, Texture*> m_texture_map_list;

		TextureManager() {}
	};
}

#endif // !TEXTURE_MANAGER_H