#ifndef RENDER_H
#define RENDER_H

/* User defined includes
*/
#include "Mesh.h"
#include "ShaderManager.h"
#include "FreeCamera.h"
#include "TextureManager.h"
#include "MaterialManager.h"
#include "DirectionalLight.h"


namespace uciniti
{
	class Render
	{
	public:
		Render();
		~Render();

		virtual void start();

		virtual void update();


	private:
		FreeCamera* m_main_camera;
		std::vector<Mesh*> m_mesh_list;
		ShaderManager* m_shaders;
		DirectionalLight* m_main_light;
		glm::mat4 m_model_matrix;

		void set_soulspear(glm::mat4 a_model_matrix, double a_current_time);

		bool create_geometry();
		bool create_textures();
		bool create_shaders();
		bool create_lights();

		void clear_screen();
		void enable_test_types();

		void clean_memory();
	};
};

#endif // !RENDER_H