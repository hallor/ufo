#include <SDL/SDL.h>
#include <GL/glew.h>

#include "Application.h"
#include "OpenAL.h"

#include "screen.h"
#include "utils.h"

#include "SceneManager.h"
#include "LogicManager.h"
#include "AppSettings.h"
#include "logger.h"

bool Application::init(int argc, char* argv[])
{
	if (SDL_Init( SDL_INIT_EVERYTHING ) < 0)
		return false;

	SDL_GL_SetAttribute(SDL_GL_RED_SIZE,    	    8);
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE,  	    8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE,   	    8);
	SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE,  	    8);

//  SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE,  	    16);
	SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE,		    32);

	SDL_GL_SetAttribute(SDL_GL_ACCUM_RED_SIZE,	    8);
	SDL_GL_SetAttribute(SDL_GL_ACCUM_GREEN_SIZE,	8);
	SDL_GL_SetAttribute(SDL_GL_ACCUM_BLUE_SIZE,	    8);
	SDL_GL_SetAttribute(SDL_GL_ACCUM_ALPHA_SIZE,	8);

	OpenAL::Get().Initialize();

	m_logic->init();
	m_scene->init();

	// restart game timer
	m_timer.restart();

	return true;
}
