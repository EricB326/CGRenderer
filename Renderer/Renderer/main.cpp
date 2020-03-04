/* User defined includes
*/
#include "Application.h"

/* @brief Beginning of the rendering software, allows the user to create
	a new application. With the new application they can start the rendering
	loop which will take care of calling appropriate functions throught the 
	programs runtime. Classes within this application are typically held within
	the "uciniti" namesapce.
*/
int main()
{
	// Check for memory leaks.
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	// Create a new rendering application.
	auto rendering_app = new uciniti::Application();

	// Initialize rendering application and being rendering loop.
	rendering_app->start("Uciniti", 1280, 720);

	// Delete the rendering application when exited.
	delete rendering_app;
	rendering_app = nullptr;

	return 0;
}