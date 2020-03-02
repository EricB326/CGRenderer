#include "Material.h"
#include "TextureManager.h"

namespace uciniti
{
	Material::Material()
		: m_ambient(glm::vec3(1.0f)), m_diffuse(glm::vec3(1.0f)), m_specular(glm::vec3(1.0f)), m_emissive(glm::vec3(1.0f)), m_specular_shininess(32.0f), m_alpha(1.0f),
		m_alpha_map(nullptr), m_ambient_map(nullptr), m_bump_map(nullptr), m_diffuse_map(nullptr), m_displacement_map(nullptr), m_specular_highlight_map(nullptr), m_specular_map(nullptr)
	{}

	void Material::set_mat_attributes(glm::vec3 a_ambient, glm::vec3 a_diffuse, glm::vec3 a_specular, glm::vec3 a_emissive, float a_specular_shininess, float a_alpha)
	{
		m_ambient = a_ambient;
		m_diffuse = a_diffuse;
		m_specular = a_specular;
		m_emissive = a_emissive;
		m_specular_shininess = a_specular_shininess;
		m_alpha = a_alpha;
	}
}