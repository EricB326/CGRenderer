#include "FreeCamera.h"


FreeCamera::FreeCamera()
	: m_camera_speed(5.0f), m_rotate_speed(0.1f)
{
}

FreeCamera::FreeCamera(const glm::vec3 a_position, const glm::vec3 a_look_at_point, const float a_aspect_ratio, const float a_FOV_Y, const float a_near_distance, const float a_far_distance)
{
	m_camera_speed = 5.0f;
	m_rotate_speed = 0.1f;

	set_persepctive(a_FOV_Y, a_aspect_ratio, a_near_distance, a_far_distance);
	set_look_at(a_position, a_look_at_point, glm::vec3(0.0f, 1.0f, 0.0f));
}

void FreeCamera::update(float a_delta_time)
{
	// Grab window context.
	auto glfw_window = glfwGetCurrentContext();
	// Only update camera if input is received.
	bool input_flag = false;
	// The build translation vector.
	glm::vec3 displacement = glm::vec3(0.0f);
	
	/*** KEYBOARD CONTROL ***/
	// Forward.
	if (glfwGetKey(glfw_window, GLFW_KEY_W))
	{
		displacement -= get_world_foward();
		input_flag = true;
	}
	// Backward.
	if (glfwGetKey(glfw_window, GLFW_KEY_S))
	{
		displacement += get_world_foward();
		input_flag = true;
	}
	// Left.
	if (glfwGetKey(glfw_window, GLFW_KEY_A))
	{
		displacement -= get_world_right();
		input_flag = true;
	}
	// Right.
	if (glfwGetKey(glfw_window, GLFW_KEY_D))
	{
		displacement += get_world_right();
		input_flag = true;
	}
	// Up.
	if (glfwGetKey(glfw_window, GLFW_KEY_LEFT_CONTROL))
	{
		displacement -= glm::vec3(0.0f, 1.0f, 0.0f);
		input_flag = true;
	}
	// Down.
	if (glfwGetKey(glfw_window, GLFW_KEY_SPACE))
	{
		displacement += glm::vec3(0.0f, 1.0f, 0.0f);
		input_flag = true;
	}

	// Apply the scaled translation to the cameras position, if there was input.
	if (input_flag)
	{
		set_position(glm::vec4(get_world_transform(), 0.0f) + (glm::vec4(glm::normalize(displacement), 0.0f) * m_camera_speed * a_delta_time));
		update_matrices();
	}

	/*** MOUSE CONTROL ***/
	double cursor_position_x;
	double cursor_position_y;

	// Aquire the current cursor position.
	glfwGetCursorPos(glfw_window, &cursor_position_x, &cursor_position_y);

	// Calculate the offset from the screens centre this frame.
	float delta_x = cursor_position_x - (WIDTH * 0.5);
	float delta_y = cursor_position_y - (HEIGHT * 0.5);

	// Move the cursor back to the centre of the render / window.
	glfwSetCursorPos(glfw_window, WIDTH * 0.5, HEIGHT * 0.5);

	// If either input is non-zero, apply the rotation.
	if (delta_x || delta_y)
	{
		// Identity matrix to accumulate rotation.
		auto rotation = glm::mat4(1.0f);
		// Left / Right rotation.
		rotation = glm::rotate(rotation, float(m_rotate_speed * a_delta_time * -delta_x), glm::vec3(get_view_up()));
		// Up / Down rotation.
		rotation = glm::rotate(rotation, float(m_rotate_speed * a_delta_time * -delta_y), glm::vec3(1.0f, 0.0f, 0.0f));

		// Apply the rotation to the camera.
		m_world_transform = m_world_transform * rotation;
		// Preserve the inverse.
		m_view_transform = glm::inverse(m_world_transform);
		// Update PxV.
		update_matrices();
	}
}

void FreeCamera::set_speed(float a_speed)
{
	m_camera_speed = a_speed;
}
