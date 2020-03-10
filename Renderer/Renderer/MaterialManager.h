#ifndef MATERIAL_MANAGER_H
#define MATERIAL_MANAGER_H

/* Standard lib includes
*/
#include <map>
#include <string>
#include <vector>

/* User defined includes
*/
#include "common.h"
#include "Material.h"

namespace uciniti
{
	/* @brief Enum class of the different materials that can
		be stored within the material manager. The data is stored
		as unsigned ints.
	*/
	enum class material_map_type : uint
	{
		ALPHA_MAP,
		AMBIENT_MAP,
		BUMP_MAP,
		DIFFUSE_MAP,
		DISPLACEMENT_MAP,
		SPECULAR_HIGHLIGHT_MAP,
		SPECULAR_MAP
	};

	/* @detail Static MaterialManager class. The class is static so that
		it cannot be instantiated. As the class is required to hold all the
		materials that were created, and should never forget them, only one
		instance of the class is required. The single static class is also
		called within multiple places around the rendering engine. The class
		handles creating materials, adding already loaded maps, retrieving
		material data, etc. The class keeps storage of the created Materials
		within a map, where the Material* is mapped to a string identifier.
	*/
	static class MaterialManager
	{
	public:
		/******************************************************/
		// Functions
		/* @brief Calls delete_all_materials() to delete all allocated map
			pointers.
		*/
		~MaterialManager();

		/* @brief Creates a material with passed data from the user. Verifies
			that the material key passed does not already exist. If not, a new
			Material() is created, set, and stored into the map with the passed
			key name.
		   @param Const char* passing the name.key to give the Material in the map.
		   @param Vector of vec3 passing the different material attributes.
		   @param Specular shine of the material.
		   @param Alpha/Opacity of the material.
		   @return Returns true if the material was successfully created and mapped.
		*/
		static bool create_material(const char* a_key_name, std::vector<glm::vec3>& a_material_list, const float a_spec_shininess, const float a_alpha);

		/* @brief Adds to an already created material a manually loaded texture.
			Verifies that the map key exists, then checks what type of map (texture)
			is being added before passing that data.
		   @param Const char* passing the name of the map key to added the new texture to.
		   @param Texture* holding the data of the manually loaded material map (texture).
		   @param The type of map that will be getting the data (diffuse, bump, spec, etc).
		*/
		static void add_loaded_map(const char* a_key_name, Texture* a_loaded_map, material_map_type a_map_type);

		/* @brief Retrives the material at the map key.
		*/
		static Material* get_material(const char* a_key_name);

		/* @brief Retrives the texture at the map key of a specific material map (texture) type.
		*/
		static Texture* get_material_map(const char* a_key_name, material_map_type a_map_type);

		/* @brief Handles looping through each pair within the map list and deletes
			the Material*.
		*/
		static void delete_all_materials();

		/* @brief Deletes a specific Material* at the given key location in the map.
		*/
		static void delete_material_at(const char* a_key_name);

	private:
		/******************************************************/
		// Variables
		/* @brief Map container that stores all the created materials.
			Each Material is stored as a pointer to that class, using a string
			identifier to locate the desired Material*.
		*/
		static std::map<std::string, Material*> m_material_map_list;

		/******************************************************/
		// Functions
		/* @brief Using the key name passed through, iterates through the
			map container searching for the key.
		   @return True if the key was found.
		*/
		static bool does_key_exist(const char* a_key_name);
	};
}

#endif // !MATERIAL_MANAGER_H