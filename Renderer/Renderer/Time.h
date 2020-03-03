#ifndef TIME_H
#define TIME_H

namespace uciniti
{
	static class Time
	{
	public:
		static void update();

		static double get_delta_time();
		static double get_total_time();
		static double get_current_time();
		static double get_time_elapsed();

	private:
		static double delta_time;
		static double total_time;
		static double last_time;
	};
};

#endif // !TIME_H