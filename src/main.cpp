#ifdef _WIN32
#ifdef _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#endif
#endif
#include "Application.h"

int main( int argc, char* argv[] )
{
    #ifdef _DEBUG
#ifdef _WIN32
	_CrtSetDbgFlag(_CrtSetDbgFlag(_CRTDBG_REPORT_FLAG) | _CRTDBG_LEAK_CHECK_DF);
	#endif
#endif

	Application * app = new Application;
	int ret_val = app->execute(argc, argv);
    delete app;
    return ret_val;
}
