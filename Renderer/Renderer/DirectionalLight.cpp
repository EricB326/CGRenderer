#include "DirectionalLight.h"

namespace uciniti
{
	DirectionalLight::DirectionalLight()
		: m_direction(glm::vec3(0.0f)), 
		  BaseLight()
	{}

	DirectionalLight::DirectionalLight(glm::vec3 a_light_direction, glm::vec3 a_diffuse_rgb, glm::vec3 a_specular_rgb, float a_diffuse_intensity, float a_specular_intensity)
		: m_direction(a_light_direction),
		  BaseLight(a_diffuse_rgb, a_specular_rgb, a_diffuse_intensity, a_specular_intensity)
	{}
}