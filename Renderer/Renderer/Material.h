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

		void set_specular_shininess(float a_specular_shininess) { m_specular_shininess = a_specular_shininess; }
		void set_alpha(float a_alpha) { m_alpha = a_alpha; }

		glm::vec3 get_ambient() { return m_ambient; }
		glm::vec3 get_diffuse() { return m_diffuse; }
		glm::vec3 get_specular() { return m_specular; }
		glm::vec3 get_emissive() { return m_emissive; }

		float get_specular_shininess() { return m_specular_shininess; }
		float get_alpha() { return m_alpha; }

		void add_map(Texture* a_map_to_add);

	private:
		glm::vec3 m_ambient;
		glm::vec3 m_diffuse;
		glm::vec3 m_specular;
		glm::vec3 m_emissive;

		float m_specular_shininess;
		float m_alpha;
	};
}