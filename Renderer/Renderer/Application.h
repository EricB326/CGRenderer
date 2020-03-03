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
	/* @detail Class Application. Handles starting up the rendering 
		software. Initializing core functionality of OpenGL, creating
		a visable window to display renders onto, and other changeable
		settings for the user to adjust. Contains the core "rendering"
		loop which handles updating data+ throughout the applications life.
		Also contains a pointer to the renderer to handle rendering to the
		main window context.
	*/
	class Application
	{
	public:
		/******************************************************/
		// Functions
		/* @brief Constructor zeros all values with no params.
		*/
		Application();

		/* @brief See: application.h lines
		*/
		virtual ~Application();

		/* @brief Start of the application. Calls out to the 
			correct functions to start and prepare the program
			for rendering.
		   @param Title of the window in which the rendering is displayed.
		   @param The width of the window being created.
		   @param The height of the window being created.
		*/
		virtual void start(const char* a_window_title, int a_window_width, int a_window_height);

		/* @brief Sets the colour of the renders display "background."
		   @param Vec4 with each element signifying the RGBA values respectively.
		*/
		void set_background_colour(glm::vec4 a_background_rgba = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f));

		/* @brief Sets if the mouse cursor should display within the renderer.
		   @param Bool on if the mouse is visable or not.
		*/
		void display_cursor(bool a_is_visable = true);

		/* @brief Checks if the application has closed which will end the
			rendering loop.
		   @return True if closed, false if not.
		*/
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