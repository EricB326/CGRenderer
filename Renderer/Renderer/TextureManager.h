#ifndef TEXTURE_MANAGER_H
#define TEXTURE_MANAGER_H

#include "common.h"
#include "Texture.h"
#include <map>
#include <string>

namespace uciniti
{
	static class TextureManager
	{
	public:
		~TextureManager();

		static bool create_texture(const char* a_key_name, const char* a_filepath);

		static void use_texture(const char* a_key_name, int a_index);

		static Texture* get_texture(const char* a_key_name);

		static void clean_texture();

		static void get_all_texture_names();

	private:
		static std::map<std::string, Texture*> m_texture_map_list;
	};
}

#endif // !TEXTURE_MANAGER_H