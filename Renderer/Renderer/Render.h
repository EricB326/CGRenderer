#ifndef RENDER_H
#define RENDER_H

/* User defined includes
*/
#include "common.h"
#include "Mesh.h"
#include "ShaderManager.h"
#include "FreeCamera.h"
#include "TextureManager.h"
#include "MaterialManager.h"
#include "DirectionalLight.h"
#include "PointLight.h"

namespace uciniti
{
	/* @detail Class Render. Handles setting up each of piece of data to
		be rendered to the window (lights, geometry, etc). Also prepares shaders
		to be used through the rendering loop. Updates data shown within the 
		window each frame, including geometry, lights, camera, etc. Finally, handles
		settings any bit of data being rendered, passing data to the GPU.
	*/
	class Render
	{
	public:
		/******************************************************/
		// Functions
		/* @brief Constructor zeros all values with no params.
		*/
		Render();

		/* @brief Calls clean_memory() to delete all allocated pointers.
		*/
		~Render();

		/* @brief Start of the renderer. Calls out to the
			correct functions to start and prepare the data to be
			rendered on the window.
		*/
		virtual void start();

		/* @brief Called once per frame. Handles updating all real-time info such as;
			geometry positions, shader in use, lighting, etc.
		*/
		virtual void update();


	private:
		/******************************************************/
		// Variables
		/* @brief FreeCamera* which acts as the main camera within
			the window. Used to look around the rendered scene.
		*/
		FreeCamera* m_main_camera;

		/* @brief Vector containers storing all the different Mesh*
			that have been created, ready for rendering.
		*/
		std::vector<Mesh*> m_mesh_list;

		/* @brief ShaderManager* handling the different loaded shaders
			as well as shader programs to allow the use easy use of shaders.
		*/
		ShaderManager* m_shaders;

		/* @brief The main source of light in the scene, typically seen 
			as the Sun.
		*/
		DirectionalLight* m_main_light;

		/* @brief Up to 20 PointLight* that can emit light in the scene.
		*/
		PointLight* m_point_lights[MAX_POINT_LIGHTS];

		/* @brief The number of point lights in the scene being used,
			this variable is used for know how many point lights to render
			as well as how many to send to the GPU.
		*/
		int m_point_light_count;

		/* @brief The model matrix is the geometry/lights position, scale, 
			and rotation in the world. This data is passed to the GPU so that
			it know where the vertices are to be rendered, at what rotation,
			and how large.
		*/
		glm::mat4 m_model_matrix;

		/* @brief Sets the uniform data of the object being passed to the GPU.
		*/
		void set_lights(glm::mat4 a_model_matrix, double a_current_time);
		void set_red_lamp(glm::mat4 a_model_matrix, double a_current_time, glm::vec3 a_position);
		void set_blue_lamp(glm::mat4 a_model_matrix, double a_current_time, glm::vec3 a_position);
		void set_earth(glm::mat4 a_model_matrix, double a_current_time);
		void set_asteroid(glm::mat4 a_model_matrix, double a_current_time);

		/* @brief Creates the different meshs, textures, shaders, and lights when
			the application is launched.
		   @return True if the data was successfuly created.
		*/
		bool create_geometry();
		bool create_textures();
		bool create_shaders();
		bool create_lights();

		/* @brief Clears the colour, depth, and stencil buffer bits for the next
			buffer swap.
		*/
		void clear_screen();

		/* @brief Specifies what tests should be enabled, ie DEPTH tests.
		*/
		void enable_test_types();

		/* @brief Handles deleting all pointers allocated.
		*/
		void clean_memory();
	};
};

#endif // !RENDER_H