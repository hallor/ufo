#include <SDL/SDL.h>
#include "Application.h"

void Application::processEvents()
{
	SDL_Event event;

	while (SDL_PollEvent(&event))
	{
		if (event.type == SDL_QUIT)
			m_do_quit = true;

		if (event.type == SDL_KEYDOWN)
			switch (event.key.keysym.sym)
			{
			case SDLK_q:
			case SDLK_ESCAPE: m_do_quit = true; break;
			default:
				break;
			}
		}

	Uint8 *keystates = SDL_GetKeyState(NULL);

	//if (keystates[SDLK_LSHIFT])
}
