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

	if (!init(argc, argv))
        return -1;

	cSoundBufferManager man;
	cSoundBuffer *buf = man.Get();

    cSoundSourceManager srcman;
    cSoundSource *src = srcman.Get();
       iFile *file = CreateFileIO();
		file->Open("xcoma/rawsound/tactical/explosns/explosn1.raw", FFileOpenFlags::OpenExisting | FFileOpenFlags::Read);
    int length = file->GetSize();

    char *data = new char[length];
    file->Read(data, length);

    alBufferData(buf->Get(), AL_FORMAT_MONO8, data, length, 22050);
    
    delete [] data;
    file->Close();
    ReleaseFileIO(file);

    alSourcei(src->Get(), AL_BUFFER, buf->Get());
        alSourcePlay(src->Get());

    cMusicPlayer playa;
    playa.Initialize();
    playa.PlayMusic("xcoma/rawsound/tactical/explosns/explosn1.raw");

	while (!shouldQuit())
	{
		fps.startOfFrame();

		processEvents();
		update();
		render();

        playa.Update(0.0f);

		fps.endOfFrame();
	}

    srcman.ReleaseResource(src);
    man.ReleaseResource(buf);

	buf->Release();
    src->Release();

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

