#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_gfxPrimitives.h>
#include <SDL/SDL_rotozoom.h>
#include <iostream>
#include <stdlib.h>
#include "gfx.h"
#include "citymap.h"
#include "cityitem.h"

using namespace std;
#define WIDTH (640*2)
#define HEIGHT (480*2)
int main( int argc, char* argv[] )

{
  GfxManager gfx;
  srand(42);
  //Start
  SDL_Init( SDL_INIT_EVERYTHING );

  SDL_Surface * screen;
  SDL_Surface * gui, *t;

  t = IMG_Load("ufodata/buybase2.pcx");
  gui = zoomSurface(t, 2, 2, 0);
  SDL_FreeSurface(t);

  screen = SDL_SetVideoMode(WIDTH, HEIGHT, 32, SDL_SWSURFACE);

  SpritePack *city = gfx.getPack("ufodata/CITY");

  SpritePack *ptang = gfx.getPack("ufodata/PTANG");

  SpritePack *ufos = gfx.getPack("ufodata/SAUCER");

  CityMap cm(100,100,10);

  cout << "Loading city map..." << std::flush;

  bool res;
if (argc <2)
  res = cm.load("ufodata/citymap1");
else
 res = cm.load(argv[1]);

  cout << res << std::endl;

  SDL_Rect s;
  SDL_Rect camera;

  s.x = s.y = 0;
  camera.x = 0;
  camera.y = 0;

  bool quit = false;

  CityItem * ci1, *ci2, *ufo;
  ci1 = new DimensionGate();
  ci2 = new DimensionGate();
  ufo = new Ufo(ci1, ci2);

  ci1->images = ptang;
  ci2->images = ptang;
  ci2->frame = ci1->frame = ci1->start_frame = ci2->start_frame = 42;
  ci1->end_frame = ci2->end_frame = 73;

  ufo->images = ufos;
  ufo->start_frame = ufo->frame = 66;
  ufo->frame *=2;
  ufo->end_frame = 77;


  while (!quit)
  {
    SDL_Event event;
    int ticks = SDL_GetTicks();

    SDL_FillRect(screen, NULL, 0);
    ci1->update();
    ci2->update();
    ufo->update();

    for (int tz = 0; tz<10; tz++)
    {
      int sx=camera.x, sy=camera.y;
      int ftx, fty, ltx, lty, a, b;
#define XX ((sx/*-camera.x*/ + 2 * sy/* - 2*camera.y */+ 32*tz)/64)
#define YY ((2*sy /*- 2*camera.y */+ 32*tz - sx /*+ camera.x*/)/64)

      // Find tile ranges
      sx = camera.x + 0;
      sy = camera.y + 0;
      ftx = XX;
      fty = YY;
      ltx = a = ftx;
      lty = b = fty;

      sx = camera.x + WIDTH;
      a = XX;
      b = YY;
      ftx = min(a, ftx); fty = min(b, fty); ltx = max(a, ltx); lty = max(b, lty);

      sy = camera.y + HEIGHT;
      a = XX;
      b = YY;
      ftx = min(a, ftx); fty = min(b, fty); ltx = max(a, ltx); lty = max(b, lty);

      sx = camera.x + 0;
      a = XX;
      b = YY;
      ftx = min(a, ftx); fty = min(b, fty); ltx = max(a, ltx); lty = max(b, lty);

      // Last tiles to end drawing at
//      ltx = (sx-camera.x + 2 * sy - 2*camera.y + 32*tz)/64;//(sx + 2*sy + 16 * tz)/64 +1;
//      lty = (2*sy - 2*camera.y + 32*tz - sx + camera.x)/64;//(2*sy - sx + 16 * tz)/64 +1;


      //ltx++; lty++; ftx--;fty--;
      ltx = lty = 100;
      ftx=fty=0;

      //s.x=camera.x;//-64;
      //s.y=camera.y;//-64;

      // Clip map
      ftx = max(0, ftx); fty = max(0, fty); ltx=min(100, ltx); lty = min(100, lty);

      //cout << "Frame" << ftx << "x" << fty << "   " << ltx << "x" << lty << " CAM " << camera.x << "x" << camera.y << endl;

      for (int tx=ftx; tx<ltx; tx++)
      {
        s.x = camera.x + (tx * 32) + (tz*0);
        s.y = camera.y + (tx * 16) - (tz*16);
        for (int ty=fty; ty<lty; ty++)
        {
          Tile * t = &cm.map[tx][ty][tz];
          if (s.x> 0 && s.y > 0 && s.x+1<WIDTH && s.y+1<HEIGHT)
          {
            if (t->tile && t->tile < city->count())
              SDL_BlitSurface(city->getSprite(t->tile)->img, NULL, screen, &s);
          }

          if (tx == ci1->tx && ty == ci1->ty && tz == ci1->tz)
            ci1->blit(screen, &s);

          if (tx == ci2->tx && ty == ci2->ty && tz == ci2->tz)
            ci2->blit(screen, &s);
          if (tx == ufo->tx && ty == ufo->ty && tz == ufo->tz)
            ufo->blit(screen, &s);

          s.x-=32;
          s.y+=16;
        }
      }
    }

    SDL_BlitSurface(gui, NULL, screen, NULL);
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
    int speed = -5;

    if (keystates[SDLK_LSHIFT])
      speed = speed * 5;
    if (keystates[SDLK_UP])
      camera.y-=speed;
    if (keystates[SDLK_DOWN])
      camera.y+=speed;
    if (keystates[SDLK_LEFT])
      camera.x-=speed;
    if (keystates[SDLK_RIGHT])
      camera.x+=speed;

    ticks = SDL_GetTicks() - ticks;
    if (1000/30 > ticks)
      SDL_Delay( (1000/30) - ticks);
    ticks = 0;
  }

  //Quit
  SDL_Quit();

  return 0;
}
