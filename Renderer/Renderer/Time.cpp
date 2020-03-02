#include "Time.h"

#include "glfw3.h"

namespace uciniti
{
	void Time::update()
	{
		double current_time = (double)glfwGetTime();

		delta_time = current_time - last_time;

		last_time = current_time;

		total_time += delta_time;
	}

	double Time::delta_time = 0.0;
	double Time::total_time = 0.0;
	double Time::last_time = 0.0;
}