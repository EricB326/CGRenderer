#ifndef TEXTURE_MANAGER_H
#define TEXTURE_MANAGER_H

/* Standard lib includes
*/
#include <map>
#include <string>

/* User defined includes
*/
#include "common.h"
#include "Texture.h"

namespace uciniti
{
	/* @detail Static TextureManager class. The class is static so that
		it cannot be instantiated. As the class is required to hold all the
		textures that were created, and should never forget them, only one
		instance of the class is required. The single static class is also
		called within multiple places around the rendering engine. The class
		handles creating textures, using textures, retrieving textures data, 
		etc. The class keeps storage of the created Textures within a map, 
		where the Texture* is mapped to a string identifier.
	*/
	static class TextureManager
	{
	public:
		/******************************************************/
		// Functions
		/* @brief Calls delete_all_textures() to delete all allocated map
			pointers.
		*/
		~TextureManager();

		/* @brief Creates a texture with passed data from the user. Verifies
			that the texture key passed does not already exist. If not, a new
			Texture() is created, set, and stored into the map with the passed
			key name.
		   @param Const char* passing the name/key to give the Texture in the map.
		   @param Filepath of the texture image being loaded.
		*/
		static bool create_texture(const char* a_key_name, const char* a_filepath);

		/* @brief Uses the texture at the specified map key, index passed is the
			position of the texture that is wanting to be used.
		*/
		static void use_texture(const char* a_key_name, int a_index);

		/* @brief Retrives the texture at the map key.
		*/
		static Texture* get_texture(const char* a_key_name);

		/* @brief Prints all the names of each texture within the map container.
			Primarily used for debugging.
		*/
		static void get_all_texture_names();

		/* @brief Handles looping through each pair within the map list and deletes
			the Texture*.
		*/
		static void delete_all_textures();

		/* @brief Deletes a specific Texture* at the given key location in the map.
		*/
		static void delete_texture_at(const char* a_key_name);

	private:
		/******************************************************/
		// Variables
		/* @brief Map container that stores all the created textures.
			Each texture is stored as a pointer to that class, using a string
			identifier to locate the desired Texture*.
		*/
		static std::map<std::string, Texture*> m_texture_map_list;

		/******************************************************/
		// Functions
		/* @brief Using the key name passed through, iterates through the
			map container searching for the key.
		   @return True if the key was found.
		*/
		static bool does_key_exist(const char* a_key_name);
	};
}

#endif // !TEXTURE_MANAGER_H