#ifndef DIRECTIONAL_LIGHT_H
#define DIRECTIONAL_LIGHT_H

#include "BaseLight.h"

namespace uciniti
{
	class DirectionalLight : public BaseLight
	{
	public:
		DirectionalLight();
		DirectionalLight(glm::vec3 a_light_direction, glm::vec3 a_ambient_rgb, glm::vec3 a_diffuse_rgb, glm::vec3 a_specular_rgb, float a_ambient_intensity, float a_diffuse_intensity, float a_specular_intensity);

		void set_light_direction(glm::vec3 a_new_light_xyz) { m_light_direction = a_new_light_xyz; }
		void set_light_direction_x(float a_new_light_x) { m_light_direction.x = a_new_light_x; }
		void set_light_direction_y(float a_new_light_y) { m_light_direction.y = a_new_light_y; }
		void set_light_direction_z(float a_new_light_z) { m_light_direction.z = a_new_light_z; }

		glm::vec3 get_light_direction() { return m_light_direction; }
		float get_light_direction_x() { return m_light_direction.x; }
		float get_light_direction_y() { return m_light_direction.y; }
		float get_light_direction_z() { return m_light_direction.z; }

	private:
		glm::vec3 m_light_direction;
	};
}

#endif // !DIRECTIONAL_LIGHT_H