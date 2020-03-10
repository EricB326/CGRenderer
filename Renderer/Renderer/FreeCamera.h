#ifndef FREE_CAMERA_H
#define FREE_CAMERA_H

/* User defined includes
*/
#include "Camera.h"

namespace uciniti
{
	/* @detail Class FreeCamera. Derives from the Camera class.
		Handles creating a camera that can more and look in all
		directions. Allows for changing the mouse sensitivity and
		speed of the camera. Finally, handles checking for user
		input to modify the facing and position of the camera.
	*/
	class FreeCamera : public Camera
	{
	public:
		/******************************************************/
		// Functions
		/* @brief Constructor zeros all values.
		*/
		FreeCamera();

		/* @brief Constructor zeros all values. Also has
			enough data to setup persepective and lookat content.
		   @param Position for the camera to start at.
		   @param Point of where teh camera will look towards.
		   @param Aspect ratio of the window.
		   @param FOV of the camera.
		   @param Near plane.
		   @param Far plane.
		*/
		FreeCamera(
			const glm::vec3 a_position,
			const glm::vec3 a_look_at_point,
			const float a_aspect_ratio = (float)WIDTH / (float)HEIGHT,
			const float a_FOV_Y = glm::radians(90.0f),
			const float a_near_distance = 0.01f,
			const float a_far_distance = 100.0f);

		/* @brief Update called once per frame. Checks user
			input to see if the camera should be adjusted
			accordingly.
		   @param Float holding the change in time (delta time)
			since the last call.
		*/
		virtual void update(double a_delta_time) override;

		/* @brief Allows the user to modify how fast the camera
			moves around the scene.
		   @param New value to be used as movement speed .
		*/
		void set_movement_speed(float a_speed) { m_camera_movement_speed = a_speed; }

		/* @brief Allows the user to modify how sensitive the
			mouse is to movement.
		   @param New value to be used as sensitivity.
		*/
		void set_mouse_sensitivity(float a_speed) { m_mouse_sensitivity = a_speed; }

	private:
		/******************************************************/
		// Variables
		/* @brief The speed the camera moves around the scene.
		*/
		float m_camera_movement_speed;

		/* @brief The sensitivity of the mouse when moved by
			the user.
		*/
		float m_mouse_sensitivity;

		/* @brief Check on if the application is doing its first
			mouse call. Done to prevent the camera starting with
			a rotate and not being zero'd.
		*/
		bool m_first_mouse_call;

		/******************************************************/
		// Functions

		/* @brief Checks keyboard input from the user and stores
			the amount to move by within a displacement vector.
			Calculate the new position by the cameras world transform
			plus the displacement.
		   @param Context of the window the application is connected to.
		   @param Float holding the change in time (delta time)
			since the last call.
		*/
		void keyboard_controls(GLFWwindow* a_window, double a_delta_time);

		/* @brief Checks mouse input from the user and stores the
			delta x and y. Using the aforemention, rotates accordingly.
		   @param Context of the window the application is connected to.
		   @param Float holding the change in time (delta time)
			since the last call.
		*/
		void mouse_controls(GLFWwindow* a_window, double a_delta_time);
	};
}

#endif // !FREE_CAMERA_H