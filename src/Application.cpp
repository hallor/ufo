#include <SDL/SDL.h>

#include "Application.h"

#include "LogicManager.h"
#include "SceneManager.h"
#include "SoundBufferManager.h"
#include "SoundBuffer.h"
#include "SoundSourceManager.h"
#include "SoundSource.h"
#include "OpenAL.h"
#include "AppSettings.h"
#include "FileIO.h"
#include "MusicPlayer.h"
#include "EngineSettings.h"
#include "SoundStreamRenderer.h"
#include "ObjectPool.h"

Application::Application()
{
	m_do_quit = false;
	m_logic = new LogicManager();
	m_scene = new SceneManager();
}

Application::~Application()
{
	delete m_logic;
	m_logic = NULL;
	delete m_scene;
	m_scene = NULL;
}

int Application::execute(int argc, char* argv[])
{
	if (!init(argc, argv))
        return -1;

	while (!shouldQuit())
	{
		processEvents();
		update();
		render();
	}
        
	exit();
	return 0;
}


bool Application::shouldQuit() const
{
	return m_do_quit;
}

void Application::exit()
{
	SDL_Quit();
	OpenAL::Free();

	AppSettings::Free();
    EngineSettings::Free();
}

