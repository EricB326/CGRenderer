#ifndef DIRECTIONAL_LIGHT_H
#define DIRECTIONAL_LIGHT_H

/* User defined includes
*/
#include "BaseLight.h"

namespace uciniti
{
	/* @brief Class DirectionalLight. Derives from the BaseLight class.
		Handles adding a direction to the light to have the light facing
		a specified location.
	*/
	class DirectionalLight : public BaseLight
	{
	public:
		/******************************************************/
		// Functions
		/* @brief Constructor zeros all values with no params.
		*/
		DirectionalLight();

		/* @brief Constructs the class with user passed data for the
			lighting direction. Also takes constructor data for the BaseLight()
			constructor.
		*/
		DirectionalLight(glm::vec3 a_light_direction, 
			glm::vec3 a_diffuse_rgb, glm::vec3 a_specular_rgb,
			float a_diffuse_intensity, float a_specular_intensity);

		/* @brief Setters for the light direction, able to set all
			data at once or individual coordinates.
		*/
		void set_direction(glm::vec3 a_new_light_xyz) { m_direction = a_new_light_xyz; }
		void set_direction_x(float a_new_light_x) { m_direction.x = a_new_light_x; }
		void set_direction_y(float a_new_light_y) { m_direction.y = a_new_light_y; }
		void set_direction_z(float a_new_light_z) { m_direction.z = a_new_light_z; }

		/* @brief Getters for the light direction, able to retrive all
			data at once or individual coordinates.
		*/
		glm::vec3 get_direction() { return m_direction; }
		float get_direction_x() { return m_direction.x; }
		float get_direction_y() { return m_direction.y; }
		float get_direction_z() { return m_direction.z; }

	private:
		/******************************************************/
		// Variables
		/* @brief Vector3 storing the X, Y, and Z coordinates for the
			lights direction.
		*/
		glm::vec3 m_direction;
	};
}

#endif // !DIRECTIONAL_LIGHT_H