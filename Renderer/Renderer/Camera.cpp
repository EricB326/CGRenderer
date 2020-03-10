/* User defined includes
*/
#include "Camera.h"

namespace uciniti
{
	void Camera::set_perspective(float a_field_of_view, float a_aspect_ratio, float a_near, float a_far)
	{
		// Set the projection matrix to persepctive "mode".
		m_projection_transform = glm::perspective(a_field_of_view, a_aspect_ratio, a_near, a_far);
	}

	void Camera::set_look_at(glm::vec3 a_world_space, glm::vec3 a_look_direction, glm::vec3 a_up)
	{
		// Set the view transforms new look at.
		m_view_transform = glm::lookAt(a_world_space, a_look_direction, a_up);

		// Set the cameras world matrix to the inverse of the view matrix.
		m_world_transform = glm::inverse(m_view_transform);

		// Update the projectiong * view matrix.
		update_matrices();
	}

	void Camera::set_position(glm::vec3 a_position)
	{
		// Sets the transform coloum of the world matrix to the passed position.
		m_world_transform[3] = glm::vec4(a_position, 1.0f);

		// Set the view matrix to the inverse of the world matrix.
		m_view_transform = glm::inverse(m_world_transform);

		// Update the projectiong * view matrix.
		update_matrices();
	}

	void Camera::update_matrices()
	{
		// Calculate the new projection view matrix
		m_projection_view_transform = m_projection_transform * m_view_transform;
	}
}
