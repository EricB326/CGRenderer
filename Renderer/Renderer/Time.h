#ifndef TIME_H
#define TIME_H

namespace uciniti
{
	/* @detail Static Time class. The class is static so that it cannot be 
		instantiated. As the class is required to hold all the time stamps, 
		and should never forget them, only one instance of the class is required. 
		The single static class is also called within multiple places around 
		the rendering engine. The class handles updating all the different time
		stamp types (total run time, delta time, etc) as well as retrieving that
		data.
	*/
	static class Time
	{
	public:
		/******************************************************/
		// Functions
		/* @brief Called once per frame. Handles updating all time stamps.
		*/
		static void update();

		/* @brief Getters for the different time stamps.
		*/
		static double get_delta_time();
		static double get_total_time();
		static double get_current_time();
		static double get_time_elapsed();

	private:
		/******************************************************/
		// Variables
		/* @brief Stores the change in time since the last frames
			time stamp.
		*/
		static double delta_time;

		/* @brief Stores the total time the application has been running.
		*/
		static double total_time;

		/* @brief Stores the last frames time stamp. Used for calculating the
			delta time.
		*/
		static double last_time;
	};
};

#endif // !TIME_H