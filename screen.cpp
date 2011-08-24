#include <SDL/SDL.h>
#include <GL/glew.h>
#include <GL/gl.h>
#include "screen.h"

Screen::Screen(int width, int height, const char * window_title)
{
  SDL_GL_SetAttribute( SDL_GL_DOUBLEBUFFER, 1 );
  SDL_GL_SetAttribute( SDL_GL_SWAP_CONTROL, 1 );

  surface = SDL_SetVideoMode(width, height, 32, SDL_HWSURFACE | SDL_GL_DOUBLEBUFFER | SDL_OPENGL);
  SDL_WM_SetCaption(window_title, NULL);
  SDL_ShowCursor(false);

  glClearColor(0, 0, 0, 0);

  glViewport(0, 0, width, height);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  glOrtho(0, width, height, 0, 10, -1);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  glEnable(GL_TEXTURE_2D);
  glClearColor(0,0,0,0);
}

void Screen::flip()
{
  SDL_GL_SwapBuffers();
}

void Screen::clear()
{
  glClear(GL_COLOR_BUFFER_BIT);
}
