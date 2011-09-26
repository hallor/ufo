#include <SDL/SDL.h>

#include "Application.h"

#include "LogicManager.h"
#include "SceneManager.h"

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
	FpsTimer fps(59);

	if (!init(argc, argv))
		return -1;

	while (!shouldQuit())
	{
		fps.startOfFrame();

		processEvents();
		update();
		render();

		fps.endOfFrame();
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
}

