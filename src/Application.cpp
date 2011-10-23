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
#include "LogicTimer.h"
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
    LogicTimer ti;
	FpsTimer fps(AppSettings::GetFPSLimit());
    cSoundStreamRenderer ren;
    ObjectPool<char> asd;

	if (!init(argc, argv))
        return -1;

	cSoundBufferManager man;

    cSoundSourceManager srcman;


    cMusicPlayer playa;
    playa.Initialize();
    playa.PlayMusic("xcoma/music");
    cSoundStreamRenderer renderer;
    renderer.Initialize(&man, &srcman);

	while (!shouldQuit())
	{
		fps.startOfFrame();

		processEvents();
		update();
		render();

        playa.Update(0.0f);

        for(unsigned int i = 0; i < playa.GetStreams().size(); ++i)
            renderer.Render(*playa.GetStreams()[i]);

        renderer.OnFrame(0.1f);

		fps.endOfFrame();
	}

    renderer.Release();
        
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

