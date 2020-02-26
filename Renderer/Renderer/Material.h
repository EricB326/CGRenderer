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

		void set_mat_attributes(glm::vec3 a_ambient, glm::vec3 a_diffuse, glm::vec3 a_specular, glm::vec3 a_emissive, float a_spec_shininess, float a_alpha);
		void set_ambient(glm::vec3 a_ambient) { m_ambient = a_ambient; }
		void set_diffuse(glm::vec3 a_diffuse) { m_diffuse = a_diffuse; }
		void set_specular(glm::vec3 a_specular) { m_specular = a_specular; }
		void set_emissive(glm::vec3 a_emissive) { m_emissive = a_emissive; }
		void set_spec_shininess(float a_spec_shininess) { m_specular_intensity = a_spec_shininess; }
		void set_alpha(float a_alpha) { m_alpha = a_alpha; }

		glm::vec3 get_ambient() { return m_ambient; }
		glm::vec3 get_diffuse() { return m_diffuse; }
		glm::vec3 get_specular() { return m_specular; }
		glm::vec3 get_emissive() { return m_emissive; }
		float get_spec_shininess() { return m_specular_intensity; }
		float get_alpha() { return m_alpha; }

		void add_map(Texture* a_map_to_add);

	private:
		glm::vec3 m_ambient;
		glm::vec3 m_diffuse;
		glm::vec3 m_specular;
		glm::vec3 m_emissive;

		float m_specular_intensity;
		float m_alpha;

		Texture m_loaded_maps[8];
	};
}