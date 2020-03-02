#ifndef APPLICATION_H
#define APPLICATION_H

/* Graphic includes
*/
#include "gl_core_4_5.h"
#include "glfw3.h"

/* Math includes
*/
#include "glm.hpp"

/* User defined includes
*/
#include "common.h"
#include "Render.h"

namespace uciniti
{
	class Application
	{
	public:
		Application();
		virtual ~Application();

		virtual void start(const char* a_window_title, int a_window_width, int a_window_height);

		void set_background_colour(glm::vec4 a_background_rgba = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));

		void display_cursor(bool a_is_visable = true);

		bool has_window_closed();

		uint get_window_width() const;
		uint get_window_height() const;

	private:
		GLFWwindow* m_main_window;
		Render* m_renderer;
		bool m_rendering_concluded;

		bool initialize(const char* a_window_title, int a_window_width, int a_window_height);
		virtual void update();
		void shutdown();
	};
};

#endif // !APPLICATION_H