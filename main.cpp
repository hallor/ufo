#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_gfxPrimitives.h>
#include <iostream>
#include "gfx.h"
#include "citymap.h"

using namespace std;
#define WIDTH 1024
#define HEIGHT 768
int main( int /*argc*/, char** /*args[]*/ )

{
  GfxManager gfx;
  //Start
  SDL_Init( SDL_INIT_EVERYTHING );

  SDL_Surface * screen;

  screen = SDL_SetVideoMode(WIDTH, HEIGHT, 32, SDL_SWSURFACE);

  SpritePack *city = gfx.getPack("ufodata/CITY");

  CityMap cm(100,100,10);

  cout << "Loading city map..." << std::flush;

  bool res;
  res = cm.load("ufodata/citymap1");
  cout << res << std::endl;

  SDL_Rect s;
  SDL_Rect camera;

  s.x = s.y = 0;
  camera.x = 0;
  camera.y = 0;

//  for (int tz=0; tz<10; ++tz)
//  for (int tx=0; tx<100; tx++)
//  {
//    for (int ty=0; ty<100; ty++)
//    {
//      Tile * t = &cm.map[tx][ty][tz];
//      if (t->tile < city->count())
//        if (s.x>-100 && s.y >-100 && s.x<WIDTH && s.y<HEIGHT)
//        {
//          if (t->tile)
//            SDL_BlitSurface(city->getSprite(t->tile)->img, NULL, screen, &s);
//        }
//      s.y+=16;
//      s.x-=32;
//    }
//    s.x = (tx * 32) + (tz*0);
//    s.y = (tx * 16) - (tz*16);
//  }

//  SDL_Flip(screen);

  bool quit = false;

  while (!quit)
  {
    SDL_Event event;
    int ticks = SDL_GetTicks();

    SDL_FillRect(screen, NULL, 0);

    for (int tz=0; tz<10; ++tz)
    for (int tx=0; tx<100; tx++)
    {
      for (int ty=0; ty<100; ty++)
      {
        Tile * t = &cm.map[tx][ty][tz];
        if (t->tile < city->count())
          if (s.x>-100 && s.y >-100 && s.x<WIDTH && s.y<HEIGHT)
          {
            if (t->tile)
              SDL_BlitSurface(city->getSprite(t->tile)->img, NULL, screen, &s);
          }
        s.x-=32;
        s.y+=16;
      }
      s.x = (tx * 32) + (tz*0);
      s.y = (tx * 16) - (tz*16);
    }

    SDL_Flip(screen);

    while (SDL_PollEvent(&event))
    {
      if (event.type == SDL_QUIT)
        quit = true;
      if (event.type == SDL_KEYDOWN)
        switch (event.key.keysym.sym)
        {
          case SDLK_q:
          case SDLK_ESCAPE: quit = true; break;
        default: break;
        }
    }

    Uint8 *keystates = SDL_GetKeyState(NULL);
#define SPEED 1
    if (keystates[SDLK_UP])
      camera.y-=SPEED;
    if (keystates[SDLK_DOWN])
      camera.y+=SPEED;
    if (keystates[SDLK_LEFT])
      camera.x-=SPEED;
    if (keystates[SDLK_RIGHT])
      camera.x+=SPEED;

    SDL_Delay( (1000/15) - (SDL_GetTicks() - ticks) );
    ticks = 0;
  }

  //Quit
  SDL_Quit();

  return 0;
}
