#ifndef MATERIAL_H
#define MATERIAL_H

/* Math includes
*/
#include "glm.hpp"

/* User defined includes
*/
#include "common.h"
#include "Texture.h"

namespace uciniti
{
	/* @brief Class Material. Handles creating a material with the data retrieved
		from the loaded .mtl file. Can store none, one, or multiple material maps
		(textures) such as diffuse, bump, specular, etc. Also handles the retriving
		the different material colours within the .mtl.
	*/
	class Material
	{
	public:
		/******************************************************/
		// Functions
		/* @brief Constructor zeros all values with no params.
		*/
		Material();

		/* @brief Allows the user to set all the differnt material atrributes
			at once. Starting with the different colours, followed by specular
			shiniess, and lastly the aplha/opacity.
		*/
		void set_mat_attributes(glm::vec3 a_ambient, glm::vec3 a_diffuse, glm::vec3 a_specular, glm::vec3 a_emissive, float a_specular_shininess, float a_alpha);

		/* @brief Setters for the different material colours.
		*/
		void set_ambient(glm::vec3 a_ambient) { m_ambient = a_ambient; }
		void set_diffuse(glm::vec3 a_diffuse) { m_diffuse = a_diffuse; }
		void set_specular(glm::vec3 a_specular) { m_specular = a_specular; }
		void set_emissive(glm::vec3 a_emissive) { m_emissive = a_emissive; }

		/* @brief Setters for the specular power and alpha.
		*/
		void set_specular_shininess(float a_specular_shininess) { m_specular_shininess = a_specular_shininess; }
		void set_alpha(float a_alpha) { m_alpha = a_alpha; }

		/* @brief Setters for the different material maps (textures).
		*/
		void set_alpha_map(Texture* a_new_map) { m_alpha_map = a_new_map; }
		void set_ambient_map(Texture* a_new_map) { m_ambient_map = a_new_map; }
		void set_bump_map(Texture* a_new_map) { m_bump_map = a_new_map; }
		void set_diffuse_map(Texture* a_new_map) { m_diffuse_map = a_new_map; }
		void set_displacement_map(Texture* a_new_map) { m_displacement_map = a_new_map; }
		void set_specular_highlight_map(Texture* a_new_map) { m_specular_highlight_map = a_new_map; }
		void set_specular_map(Texture* a_new_map) { m_specular_map = a_new_map; }

		/* @brief Getters for the different material colours.
		*/
		glm::vec3 get_ambient() { return m_ambient; }
		glm::vec3 get_diffuse() { return m_diffuse; }
		glm::vec3 get_specular() { return m_specular; }
		glm::vec3 get_emissive() { return m_emissive; }

		/* @brief Getters for the specular power and alpha.
		*/
		float get_specular_shininess() { return m_specular_shininess; }
		float get_alpha() { return m_alpha; }

		/* @brief Getters for the different material maps (textures).
		*/
		Texture* get_alpha_map() { return m_alpha_map; }
		Texture* get_ambient_map() { return m_ambient_map; }
		Texture* get_bump_map() { return m_bump_map; }
		Texture* get_diffuse_map() { return m_diffuse_map; }
		Texture* get_displacement_map() { return m_displacement_map; }
		Texture* get_specular_highlight_map() { return m_specular_highlight_map; }
		Texture* get_specular_map() { return m_specular_map; }

	private:
		/******************************************************/
		// Variables
		/* @brief Vector3 storing the R, G, and B colour channels for the
			ambient colour.
		*/
		glm::vec3 m_ambient;

		/* @brief Vector3 storing the R, G, and B colour channels for the
			diffuse colour.
		*/
		glm::vec3 m_diffuse;

		/* @brief Vector3 storing the R, G, and B colour channels for the
			specular colour.
		*/
		glm::vec3 m_specular;

		/* @brief Vector3 storing the R, G, and B colour channels for the
			emissive colour.
		*/
		glm::vec3 m_emissive;

		/* @brief Float value controlling the "shininess" of the specularity.
		*/
		float m_specular_shininess;

		/* @brief Float value controlling the alpha/opacity.
		*/
		float m_alpha;

		/* @brief The different material maps (textures) that can be used, or not,
			on a material.
		*/
		Texture* m_alpha_map;
		Texture* m_ambient_map;
		Texture* m_bump_map;
		Texture* m_diffuse_map;
		Texture* m_displacement_map;
		Texture* m_specular_highlight_map;
		Texture* m_specular_map;
	};
}

#endif // !MATERIAL_H