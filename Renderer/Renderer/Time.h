#ifndef TIME_H
#define TIME_H

namespace uciniti
{
	static class Time
	{
	public:
		static double delta_time;
		static double total_time;

		static void update();

	private:
		static double last_time;
	};
};

#endif // !TIME_H