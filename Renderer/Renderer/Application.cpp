/* Standard lib includes
*/
#include <iostream>

/* User defined includes
*/
#include "Application.h"
#include "Time.h"

namespace uciniti
{
	Application::Application()
		: m_main_window(nullptr), m_renderer(nullptr), m_rendering_concluded(false)
	{}

	Application::~Application()
	{
		shutdown();
	}

	void Application::start(const char* a_window_title, int a_window_width, int a_window_height)
	{
		// Initialize OpenGL and window context.
		if (!initialize(a_window_title, a_window_width, a_window_height))
			return;

		// Set the background colour for the application.
		set_background_colour(glm::vec4(0.2f, 0.2f, 0.2f, 1.0f));

		// Disable the cursor display.
		display_cursor(false);

		// Initialize the rendering.
		m_renderer = new Render();
		m_renderer->start();

		// Being the rendering loop.
		update();
	}

	void Application::set_background_colour(glm::vec4 a_background_rgba)
	{
		glClearColor(a_background_rgba.r, a_background_rgba.g, a_background_rgba.b, a_background_rgba.a);
	}

	void Application::display_cursor(bool a_is_visable)
	{
		glfwSetInputMode(m_main_window, GLFW_CURSOR, ((a_is_visable) ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED));
	}

	bool Application::has_window_closed()
	{
		// If the 'ESC' key, or the 'X' in the window bar is pressed, the window has closed.
		if (glfwGetKey(m_main_window, GLFW_KEY_ESCAPE) == GLFW_PRESS || glfwWindowShouldClose(m_main_window))
			return true;

		return false;
	}

	uint Application::get_window_width() const
	{
		int width = 0, height = 0;
		glfwGetWindowSize(m_main_window, &width, &height);
		return width;
	}

	uint Application::get_window_height() const
	{
		int width = 0, height = 0;
		glfwGetWindowSize(m_main_window, &width, &height);
		return height;
	}

	bool Application::initialize(const char* a_window_title, int a_window_width, int a_window_height)
	{
		// Initialize window.
		if (!glfwInit())
		{
			printf("ERROR: initialize() call. Failed to initialize GLFW.\n");
			return false;
		}

		// Set the main window and verify it exists.
		m_main_window = glfwCreateWindow(a_window_width, a_window_height, a_window_title, nullptr, nullptr);
		if (!m_main_window)
		{
			printf("ERROR: initialize() call. Failed to create glfw window.\n");
			glfwTerminate();
			return false;
		}

		glfwMakeContextCurrent(m_main_window);

		// Initialize openGL
		if (ogl_LoadFunctions() == ogl_LOAD_FAILED)
		{
			printf("ERROR: initialize() call. Failed to load OpenGL functionality.\n");
			glfwDestroyWindow(m_main_window);
			glfwTerminate();
			return false;
		}

		auto major = ogl_GetMajorVersion();
		auto minor = ogl_GetMinorVersion();
		printf("Running OpenGL version: %i.%i\n\n", major, minor);

		return true;
	}

	void Application::update()
	{
		while (!m_rendering_concluded)
		{
			// Ensure time is updated each frame.
			Time::update();

			// Update polled events (input etc).
			glfwPollEvents();

			// Render to the screen.
			m_renderer->update();

			// Push backbuffer to the monitor.
			glfwSwapBuffers(m_main_window);

			// Check if the loop is to exit.
			m_rendering_concluded = has_window_closed();
		}
	}

	void Application::shutdown()
	{
		// Destroy all pointers.
		delete m_renderer;
		m_renderer = nullptr;

		glfwDestroyWindow(m_main_window);
		glfwTerminate();
	}
}