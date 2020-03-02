#pragma once

#include "common.h"
#include "glm.hpp"
#include "Texture.h"

namespace uciniti
{
	class Material
	{
	public:
		Material();

		void set_mat_attributes(glm::vec3 a_ambient, glm::vec3 a_diffuse, glm::vec3 a_specular, glm::vec3 a_emissive, float a_specular_shininess, float a_alpha);

		void set_ambient(glm::vec3 a_ambient) { m_ambient = a_ambient; }
		void set_diffuse(glm::vec3 a_diffuse) { m_diffuse = a_diffuse; }
		void set_specular(glm::vec3 a_specular) { m_specular = a_specular; }
		void set_emissive(glm::vec3 a_emissive) { m_emissive = a_emissive; }

		void set_alpha_map(Texture* a_new_map) { m_alpha_map = a_new_map; }
		void set_ambient_map(Texture* a_new_map) { m_ambient_map = a_new_map; }
		void set_bump_map(Texture* a_new_map) { m_bump_map = a_new_map; }
		void set_diffuse_map(Texture* a_new_map) { m_diffuse_map = a_new_map; }
		void set_displacement_map(Texture* a_new_map) { m_displacement_map = a_new_map; }
		void set_specular_highlight_map(Texture* a_new_map) { m_specular_highlight_map = a_new_map; }
		void set_specular_map(Texture* a_new_map) { m_specular_map = a_new_map; }

		void set_specular_shininess(float a_specular_shininess) { m_specular_shininess = a_specular_shininess; }
		void set_alpha(float a_alpha) { m_alpha = a_alpha; }

		glm::vec3 get_ambient() { return m_ambient; }
		glm::vec3 get_diffuse() { return m_diffuse; }
		glm::vec3 get_specular() { return m_specular; }
		glm::vec3 get_emissive() { return m_emissive; }

		Texture* get_alpha_map() { return m_alpha_map; }
		Texture* get_ambient_map() { return m_ambient_map; }
		Texture* get_bump_map() { return m_bump_map; }
		Texture* get_diffuse_map() { return m_diffuse_map; }
		Texture* get_displacement_map() { return m_displacement_map; }
		Texture* get_specular_highlight_map() { return m_specular_highlight_map; }
		Texture* get_specular_map() { return m_specular_map; }

		float get_specular_shininess() { return m_specular_shininess; }
		float get_alpha() { return m_alpha; }

	private:
		glm::vec3 m_ambient;
		glm::vec3 m_diffuse;
		glm::vec3 m_specular;
		glm::vec3 m_emissive;

		Texture* m_alpha_map;
		Texture* m_ambient_map;
		Texture* m_bump_map;
		Texture* m_diffuse_map;
		Texture* m_displacement_map;
		Texture* m_specular_highlight_map;
		Texture* m_specular_map;

		float m_specular_shininess;
		float m_alpha;
	};
}