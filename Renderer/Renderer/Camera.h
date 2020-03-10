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

/* User defined includes
*/
#include "common.h"

namespace uciniti
{
	/* @brief Class Camera. Base data for different camera types
		that may be made. Handles setting if the camera is perspective
		or ortho, where the camera is looking, as well as any world, view,
		and projection matrix data.
	*/
	class Camera
	{
	public:
		/******************************************************/
		// Functions
		/* @brief Constructor sets each matrix type to an
			identity matrix4.
		*/
		Camera() : m_world_transform(glm::mat4(1.0f)), m_view_transform(glm::mat4(1.0f)), m_projection_transform(glm::mat4(1.0f)), m_projection_view_transform(glm::mat4(1.0f)) {};

		/* TODO: Add something to blank update.
		*/
		virtual void update(double a_delta_time = 0) {};

		/* @brief Sets the camera to a perspective type.
		   @param Field of view of the camera (in radians).
		   @param Aspect ratio of the window.
		   @param Cameras near plane.
		   @param Cameras far plane.
		*/
		void set_perspective(float a_field_of_view = glm::radians(75.0f),
			float a_aspect_ratio = ((float)WIDTH / (float)HEIGHT),
			float a_near = 0.01f, float a_far = 100.0f);

		/* @brief Sets the cameras look at using glm. Updates
			the world transform matrix to the inverse of the view.
			Finally updates the PV matrix.
		   @param Vec3 of the cameras position.
		   @param Vec3 of the look direction/center.
		   @param Vec3 worlds up vector.
		*/
		void set_look_at(glm::vec3 a_world_space, glm::vec3 a_look_direction, glm::vec3 a_up);

		/* @brief Sets the position of the camera.
		   @param The new position for the camera.
		*/
		void set_position(glm::vec3 a_position);

		/* @brief Update the projection view (PV) matrix with the new
			projection * view calculation.
		*/
		void update_matrices();

		/* @brief Retrives the world matrix.
		   @param Returns a mat4 of the world transform matrix.
		*/
		inline glm::mat4 get_world_matrix() { return m_world_transform; }

		/* @brief Retrives the world right vector.
		   @param Returns vec3 of worlds transform matrix index 0 (right vector coloum).
		*/
		inline glm::vec3 get_world_right() { return m_world_transform[0]; }

		/* @brief Retrives the world up vector.
		   @param Returns vec3 of worlds transform matrix index 1 (up vector coloum).
		*/
		inline glm::vec3 get_world_up() { return m_world_transform[1]; }

		/* @brief Retrives the world foward vector.
		   @param Returns vec3 of worlds transform matrix index 2 (foward vector coloum).
		*/
		inline glm::vec3 get_world_foward() { return m_world_transform[2]; }

		/* @brief Retrives the world transform vector.
		   @param Returns vec3 of worlds transform matrix index 3 (transform vector coloum).
		*/
		inline glm::vec3 get_world_transform() { return m_world_transform[3]; }

		/* @brief Retrives the view matrix.
		   @param Returns a mat4 of the view transform matrix.
		*/
		inline glm::mat4 get_view_matrix() { return m_view_transform; }

		/* @brief Retrives the view right vector.
		   @param Returns vec3 of views transform matrix index 0 (right vector coloum).
		*/
		inline glm::vec3 get_view_right() { return m_view_transform[0]; }

		/* @brief Retrives the view up vector.
		   @param Returns vec3 of views transform matrix index 0 (up vector coloum).
		*/
		inline glm::vec3 get_view_up() { return m_view_transform[1]; }

		/* @brief Retrives the view forward vector.
		   @param Returns vec3 of views transform matrix index 0 (forward vector coloum).
		*/
		inline glm::vec3 get_view_foward() { return m_view_transform[2]; }

		/* @brief Retrives the view transform vector.
		   @param Returns vec3 of views transform matrix index 3 (transform vector coloum).
		*/
		inline glm::vec3 get_view_transform() { return m_view_transform[3]; }

		/* @brief Retrives the projection matrix.
		   @param Returns a mat4 of the projection transform matrix.
		*/
		inline glm::mat4 get_projection() { return m_projection_transform; }

		/* @brief Retrives the projection view (p * v) matrix.
		   @param Returns a mat4 of the projection view transform matrix.
		*/
		inline glm::mat4 get_projection_view() { return m_projection_view_transform; }

	protected:
		/******************************************************/
		// Variables
		/* @brief The cameras world transform matrix.
		*/
		glm::mat4 m_world_transform;

		/* @brief The cameras view transform matrix.
		*/
		glm::mat4 m_view_transform;

		/* @brief The projection transform matrix.
		*/
		glm::mat4 m_projection_transform;

		/* @brief The projection view transform matrix.
		*/
		glm::mat4 m_projection_view_transform;
	};
}

#endif // !CAMERA_H