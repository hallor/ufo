#ifndef COMPAT_SDL_H
#define COMPAT_SDL_H
// Compat sdl headers

#ifdef _WIN32
#include <SDL\SDL.h>
#else
#include <SDL/SDL.h>
#endif

#endif // COMPAT_SDL_H
