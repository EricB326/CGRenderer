#include "BaseLight.h"

namespace uciniti
{
	BaseLight::BaseLight()
		: m_ambient_colour(glm::vec3(0)), m_diffuse_colour(glm::vec3(0)), m_specular_colour(glm::vec3(0)), m_ambient_intensity(0), m_diffuse_intensity(0), m_specular_intensity(0)
	{}

	BaseLight::BaseLight(glm::vec3 a_ambient_rgb, glm::vec3 a_diffuse_rgb, glm::vec3 a_specular_rgb, float a_ambient_intensity, float a_diffuse_intensity, float a_specular_intensity)
		: m_ambient_colour(a_ambient_rgb), m_diffuse_colour(a_diffuse_rgb), m_specular_colour(a_diffuse_rgb), m_ambient_intensity(a_ambient_intensity), m_diffuse_intensity(a_diffuse_intensity), m_specular_intensity(a_specular_intensity)
	{}
}
