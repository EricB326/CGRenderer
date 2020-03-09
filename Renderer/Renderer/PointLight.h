#ifndef POINT_LIGHT_H
#define POINT_LIGHT_H

#include "BaseLight.h"

namespace uciniti
{
	class PointLight : public BaseLight
	{
	public:
		PointLight();
		PointLight(glm::vec3 a_light_position, glm::vec3 a_attenuation_values, 
			glm::vec3 a_ambient_rgb, glm::vec3 a_diffuse_rgb, glm::vec3 a_specular_rgb, 
			float a_ambient_intensity, float a_diffuse_intensity, float a_specular_intensity);

		void set_light_position(glm::vec3 a_new_light_pos) { m_position = a_new_light_pos; }
		void set_light_position_x(float a_new_light_x) { m_position.x = a_new_light_x; }
		void set_light_position_y(float a_new_light_y) { m_position.y = a_new_light_y; }
		void set_light_position_z(float a_new_light_z) { m_position.z = a_new_light_z; }

		void set_light_attenuation(glm::vec3 a_new_attenuation_values) { m_constant = a_new_attenuation_values.x;
																		 m_linear = a_new_attenuation_values.y;
																		 m_exponent = a_new_attenuation_values.z; }
		void set_light_constant(float a_new_constant) { m_constant = a_new_constant; }
		void set_light_linear(float a_new_linear) { m_linear = a_new_linear; }
		void set_light_exponent(float a_new_exponent) { m_exponent = a_new_exponent; }

		glm::vec3 get_light_position() { return m_position; }
		float get_light_position_x() { return m_position.x; }
		float get_light_position_y() { return m_position.y; }
		float get_light_position_z() { return m_position.z; }

		glm::vec3 get_light_attenuation() { return glm::vec3(m_constant, m_linear, m_exponent); }
		float get_light_constant() { return m_constant; }
		float get_light_linear() { return m_linear; }
		float get_light_exponent() { return m_exponent; }

	protected:
		glm::vec3 m_position;

		float m_constant, m_linear, m_exponent;
	};

}

#endif // !POINT_LIGHT_H