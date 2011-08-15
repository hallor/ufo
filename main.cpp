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
    for (int tz = 0; tz<10; tz++)
    {
      int sx=camera.x, sy=camera.y;
      int ftx, fty, ltx, lty;

      // First tiles to start drawing from
      ftx = (sx + 2*sy + 16 * tz)/64 -1;
      fty = (2*sy - sx + 16 * tz)/64 -1;

      sx = camera.x + WIDTH;
      sy = camera.y + HEIGHT;

      // Last tiles to end drawing at
      ltx = (sx + 2*sy + 16 * tz)/64 +1;
      lty = (2*sy - sx + 16 * tz)/64 +1;
      ltx = lty = 100;
      ftx=fty=0;
      //cout << "Frame" << ftx << "x" << fty << "   " << ltx << "x" << lty << endl;

      //s.x=camera.x;//-64;
      //s.y=camera.y;//-64;

      for (int tx=ftx; tx<ltx; tx++)
      {
        s.x = camera.x + (tx * 32) + (tz*0);
        s.y = camera.y + (tx * 16) - (tz*16);
        for (int ty=fty; ty<lty; ty++)
        {
          if (tx >= 0 && tx < 100 && ty >= 0 && ty < 100)
          {
            Tile * t = &cm.map[tx][ty][tz];
            if (s.x> 0 && s.y > 0 && s.x<WIDTH && s.y<HEIGHT)
            {
              if (t->tile && t->tile < city->count())
                SDL_BlitSurface(city->getSprite(t->tile)->img, NULL, screen, &s);
            }
          }
          s.x-=32;
          s.y+=16;
        }
      }
    }
#if 0
      while (sx < camera.x+WIDTH || sy < camera.y + HEIGHT)
      {
        tx = (sx + 2*sy + 16 * sz)/64;
        ty = (2*sy - sx + 16 * sz)/64;
        if (tx > 0 && tx < 100 && ty > 0 && ty < 100)
        {
          Tile * t = &cm.map[tx][ty][sz];
          s.x=sx-camera.x; s.y=sy-camera.y;
          if (t->tile)
            SDL_BlitSurface(city->getSprite(t->tile)->img, NULL, screen, &s);
        }
        sx += 63;
        if (sx > camera.x + WIDTH)
        {
          sy += 31;
            if (sy > camera.y + HEIGHT)
              break;
          sx = 0;
        }
//        sx-=32;
//        sy+=16;
//        sx = (tx+1)*32;
//        sy = (ty+1)*32;
      }
    }
#endif
#if 0
    for (int sx=camera.x-50; sx<camera.x+WIDTH; sx++)
      for (int sy=camera.y-50; sy<camera.y+HEIGHT; sy++)
      {
        int tx, ty;
        tx = (sx + 2*sy + 16 * sz)/64;
        ty = (2*sy - sx + 16 * sz)/64;
        if (tx > 0 && tx < 100 && ty > 0 && ty < 100)
        {
          Tile * t = &cm.map[tx][ty][sz];
          s.x=sx-camera.x; s.y=sy-camera.y;
          //if (t->tile)
            SDL_BlitSurface(city->getSprite(t->tile)->img, NULL, screen, &s);
        }
      }
#endif
#if 0
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
#endif
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
    int speed = 5;

    if (keystates[SDLK_LSHIFT])
      speed = speed * 5;
    if (keystates[SDLK_UP])
      camera.y+=speed;
    if (keystates[SDLK_DOWN])
      camera.y-=speed;
    if (keystates[SDLK_LEFT])
      camera.x+=speed;
    if (keystates[SDLK_RIGHT])
      camera.x-=speed;

    SDL_Delay( (1000/30) - (SDL_GetTicks() - ticks) );
    ticks = 0;
  }

  //Quit
  SDL_Quit();

  return 0;
}
