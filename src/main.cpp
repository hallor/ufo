#include "game_pch.h"
#ifdef _WIN32
#ifdef _DEBUG
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
#endif
#endif
#include "Game.h"

int main( int argc, char* argv[] )
{
    #ifdef _DEBUG
#ifdef _WIN32
	_CrtSetDbgFlag(_CrtSetDbgFlag(_CRTDBG_REPORT_FLAG) | _CRTDBG_LEAK_CHECK_DF);
	#endif
#endif

    int ret_val = Game::GetSingleton()->OnExecute(argc, argv);
    Game::Release();
    return ret_val;
}
