#ifndef POINT_LIGHT_H
#define POINT_LIGHT_H

/* User defined includes
*/
#include "BaseLight.h"

namespace uciniti
{
	/* @brief Class PointLight. Derives from the BaseLight class.
		Handles adding a position and quadratic fall off (attenuation)
		to the light.
	*/
	class PointLight : public BaseLight
	{
	public:
		/******************************************************/
		// Functions
		/* @brief Constructor zeros all values with no params.
		*/
		PointLight();

		/* @brief Constructs the class with user passed data for the
			lights position, as well as attenuation data. Also takes 
			constructor data for the BaseLight().
		*/
		PointLight(glm::vec3 a_light_position, glm::vec3 a_attenuation_values, 
			glm::vec3 a_diffuse_rgb, glm::vec3 a_specular_rgb, 
			float a_diffuse_intensity, float a_specular_intensity);

		/* @brief Setters for the light position, able to set all
			data at once or individual coordinates.
		*/
		void set_position(glm::vec3 a_new_light_pos) { m_position = a_new_light_pos; }
		void set_position_x(float a_new_light_x) { m_position.x = a_new_light_x; }
		void set_position_y(float a_new_light_y) { m_position.y = a_new_light_y; }
		void set_position_z(float a_new_light_z) { m_position.z = a_new_light_z; }

		/* @brief Setters for the attenuation calculation data required.
			Can be set all at once or each individual term in the equation.
			Note that constant should typically be left at 1.0f.
		*/
		void set_attenuation(glm::vec3 a_new_attenuation_values) { m_constant = a_new_attenuation_values.x;
																   m_linear = a_new_attenuation_values.y;
																   m_exponent = a_new_attenuation_values.z; }
		void set_constant(float a_new_constant) { m_constant = a_new_constant; }
		void set_linear(float a_new_linear) { m_linear = a_new_linear; }
		void set_exponent(float a_new_exponent) { m_exponent = a_new_exponent; }

		/* @brief Getters for the light position, able to retrive all
			data at once or individual coordinates.
		*/
		glm::vec3 get_position() { return m_position; }
		float get_position_x() { return m_position.x; }
		float get_position_y() { return m_position.y; }
		float get_position_z() { return m_position.z; }

		/* @brief Getters for the attenuation terms, able to retrive all
		data at once or individual terms.
	*/
		glm::vec3 get_attenuation() { return glm::vec3(m_constant, m_linear, m_exponent); }
		float get_constant() { return m_constant; }
		float get_linear() { return m_linear; }
		float get_exponent() { return m_exponent; }

	protected:
		/******************************************************/
		// Variables
		/* @brief Vector3 storing the X, Y, and Z coordinates of the
			lights position.
		*/
		glm::vec3 m_position;

		/* @brief The different terms that are required for calculating
			the attenuation of the light
		*/
		float m_constant, m_linear, m_exponent; // c + lx + ex^2
	};

}

#endif // !POINT_LIGHT_H