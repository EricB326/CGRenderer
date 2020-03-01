#ifndef BASE_LIGHT_H
#define BASE_LIGHT_H

#include "gl_core_4_5.h"
#include "glm.hpp"

namespace uciniti
{
	class BaseLight
	{
	public:
		BaseLight();
		BaseLight(glm::vec3 a_ambient_rgb, glm::vec3 a_diffuse_rgb, glm::vec3 a_specular_rgb, float a_ambient_intensity, float a_specular_intensity, float a_diffuse_intensity);

		void set_ambient_colour(glm::vec3 a_new_light_rgb) { m_ambient_colour = a_new_light_rgb; }
		void set_ambient_r(float a_new_light_r) { m_ambient_colour.r = a_new_light_r; }
		void set_ambient_g(float a_new_light_g) { m_ambient_colour.g = a_new_light_g; }
		void set_ambient_b(float a_new_light_b) { m_ambient_colour.b = a_new_light_b; }

		void set_diffuse_colour(glm::vec3 a_new_diffuse_rgb) { m_diffuse_colour = a_new_diffuse_rgb; }
		void set_diffuse_r(float a_new_diffuse_r) { m_diffuse_colour.r = a_new_diffuse_r; }
		void set_diffuse_g(float a_new_diffuse_g) { m_diffuse_colour.g = a_new_diffuse_g; }
		void set_diffuse_b(float a_new_diffuse_b) { m_diffuse_colour.b = a_new_diffuse_b; }

		void set_specular_colour(glm::vec3 a_new_specular_rgb) { m_specular_colour = a_new_specular_rgb; }
		void set_specular_r(float a_new_specular_r) { m_specular_colour.r = a_new_specular_r; }
		void set_specular_g(float a_new_specular_g) { m_specular_colour.g = a_new_specular_g; }
		void set_specular_b(float a_new_specular_b) { m_specular_colour.b = a_new_specular_b; }

		void set_ambient_intensity(float a_new_ambient_intensity) { m_ambient_intensity = a_new_ambient_intensity; }
		void set_diffuse_intensity(float a_new_diffuse_intensity) { m_diffuse_intensity = a_new_diffuse_intensity; }
		void set_specular_intensity(float a_new_specular_intensity) { m_specular_intensity = a_new_specular_intensity; }

		glm::vec3 get_ambient_colour() { return m_ambient_colour; }
		glm::vec3 get_diffuse_colour() { return m_diffuse_colour; }
		glm::vec3 get_specular_colour() { return m_specular_colour; }
				
		float get_ambient_intensity() { return m_ambient_intensity; }
		float get_diffuse_intensity() { return m_diffuse_intensity; }
		float get_specular_intensity() { return m_specular_intensity; }

	private:
		glm::vec3 m_ambient_colour;
		glm::vec3 m_diffuse_colour;
		glm::vec3 m_specular_colour;

		float m_ambient_intensity;
		float m_diffuse_intensity;
		float m_specular_intensity;
	};
}

#endif // !BASE_LIGHT_H