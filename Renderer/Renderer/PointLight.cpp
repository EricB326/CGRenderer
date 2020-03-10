/* User defined includes
*/
#include "PointLight.h"

namespace uciniti
{
	PointLight::PointLight()
		: m_position(glm::vec3(0.0f)), m_constant(1.0), m_linear(0.09), m_exponent(0.032),
		  BaseLight()
	{}

	PointLight::PointLight(glm::vec3 a_light_position, glm::vec3 a_attenuation_values, glm::vec3 a_diffuse_rgb, glm::vec3 a_specular_rgb, float a_diffuse_intensity, float a_specular_intensity)
		: m_position(a_light_position), m_constant(a_attenuation_values.x), m_linear(a_attenuation_values.y), m_exponent(a_attenuation_values.z),
		BaseLight(a_diffuse_rgb, a_specular_rgb, a_diffuse_intensity, a_specular_intensity)
	{}
}