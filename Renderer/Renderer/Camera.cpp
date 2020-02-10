#include "Camera.h"

void Camera::set_persepctive(float a_field_of_view,
							 float a_aspect_ratio,
							 float a_near, float a_far)
{
	m_projection_transform = glm::perspective(a_field_of_view, a_aspect_ratio, a_near, a_far);
}

void Camera::set_look_at(glm::vec3 a_world_space, glm::vec3 a_look_direction, glm::vec3 a_up)
{
	m_view_transform = glm::lookAt(a_world_space, a_look_direction, a_up);
	m_world_transform = glm::inverse(m_view_transform);
	update_matrices();
}

void Camera::set_position(glm::vec3 a_position)
{
	m_world_transform[3] = glm::vec4(a_position, 1.0f);
	m_view_transform = glm::inverse(m_world_transform);
	update_matrices();
}

void Camera::update_matrices()
{
	m_projection_view_transform = m_projection_transform * m_view_transform;
}
