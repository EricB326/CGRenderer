#include "DirectionalLight.h"

uciniti::DirectionalLight::DirectionalLight() 
	: m_light_direction(glm::vec3(0.0f)), BaseLight()
{}

uciniti::DirectionalLight::DirectionalLight(glm::vec3 a_light_direction, glm::vec3 a_light_rgb, glm::vec3 a_diffuse_rgb, glm::vec3 a_specular_rgb, float a_specular_intensity, float a_diffuse_intensity)
	: m_light_direction(a_light_direction), BaseLight(a_light_rgb, a_diffuse_rgb, a_specular_rgb, a_specular_intensity, a_diffuse_intensity)
{}
