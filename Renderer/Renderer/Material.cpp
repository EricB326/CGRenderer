#include "Material.h"

namespace uciniti
{
	Material::Material()
		: m_ambient(glm::vec3(1.0f)), m_diffuse(glm::vec3(1.0f)), m_specular(glm::vec3(1.0f)), m_emissive(glm::vec3(1.0f)), m_specular_intensity(32.0f), m_alpha(1.0f)
	{}

	void Material::set_mat_attributes(glm::vec3 a_ambient, glm::vec3 a_diffuse, glm::vec3 a_specular, glm::vec3 a_emissive, float a_spec_shininess, float a_alpha)
	{
		m_ambient = a_ambient;
		m_diffuse = a_diffuse;
		m_specular = a_specular;
		m_emissive = a_emissive;
		m_specular_intensity = a_spec_shininess;
		m_alpha = a_alpha;
	}
}