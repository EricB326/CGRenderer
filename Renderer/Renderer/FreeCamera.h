#ifndef FREE_CAMERA_H
#define FREE_CAMERA_H
#include "Camera.h"

class FreeCamera : public Camera
{
public:
	FreeCamera();
	FreeCamera(
		const glm::vec3 a_position,
		const glm::vec3 a_look_at_point,
		const float a_aspect_ratio = 16.0 / 9.0f,
		const float a_FOV_Y = 3.14159f * 0.5f, // 90 on Y, roughly 120 on X
		const float a_near_distance = 0.01f,
		const float a_far_distance = 100.0f);

	void update(float a_delta_time) override;

	void set_speed(float a_speed);

private:
	float m_camera_speed;
	float m_rotate_speed;

};

#endif // !FREE_CAMERA_H


