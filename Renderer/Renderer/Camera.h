#ifndef CAMERA_H
#define CAMERA_H

/* Graphic includes
*/
#include "gl_core_4_5.h"
#include "glfw3.h"

/* Math includes
*/
#include "glm.hpp"
#include "ext.hpp"

#include "common.h"

class Camera
{
public:
	Camera() : m_world_transform(glm::mat4(1.0f)), m_view_transform(glm::mat4(1.0f)), m_projection_transform(glm::mat4(1.0f)), m_projection_view_transform(glm::mat4(1.0f)) {};
	~Camera() {};

	virtual void update(float a_delta_time = 0) {};

	void set_persepctive(float a_field_of_view = glm::radians(75.0f),
					     float a_aspect_ratio = ((float)WIDTH / (float)HEIGHT),
						 float a_near = 0.01f, float a_far = 100.0f);

	void set_look_at(glm::vec3 a_world_space, glm::vec3 a_look_direction, glm::vec3 a_up);

	void set_position(glm::vec3 a_position);

	glm::mat4 get_world_matrix() { return m_world_transform; }

	glm::vec3 get_world_right() { return m_world_transform[0]; }
	
	glm::vec3 get_world_up() { return m_world_transform[1]; }

	glm::vec3 get_world_foward() { return m_world_transform[2]; }

	glm::vec3 get_world_transform() { return m_world_transform[3]; }

	glm::mat4 get_view_matrix() { return m_view_transform; }

	glm::vec3 get_view_right() { return m_view_transform[0]; }

	glm::vec3 get_view_up() { return m_view_transform[1]; }

	glm::vec3 get_view_foward() { return m_view_transform[2]; }

	glm::vec3 get_view_transform() { return m_view_transform[3]; }

	glm::mat4 get_projection() { return m_projection_transform; }

	glm::mat4 get_projection_view() { return m_projection_view_transform; }


	void update_matrices();

protected:
	glm::mat4 m_world_transform;

	glm::mat4 m_view_transform;

	glm::mat4 m_projection_transform;

	glm::mat4 m_projection_view_transform;

};

#endif // !CAMERA_H


