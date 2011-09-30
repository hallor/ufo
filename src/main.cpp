#include "Application.h"

int main( int argc, char* argv[] )
{
	Application * app = new Application();
	return app->execute(argc, argv);
}
