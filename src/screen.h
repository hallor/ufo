#ifndef SCREEN_H
#define SCREEN_H

struct SDL_Surface;

class Screen
{
public:
  Screen(int width, int height, const char * window_title="ufo");

  void flip();

  void clear();

private:
  SDL_Surface * surface;
};
#endif // SCREEN_H
