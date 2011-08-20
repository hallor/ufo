#include <SDL/SDL.h>
#include <SDL/SDL_image.h>
#include <SDL/SDL_gfxPrimitives.h>
#include <iostream>
#include <algorithm>
#include <stdlib.h>
#include <boost/foreach.hpp>
#include "gfx.h"
#include "citymap.h"
#include "cityitem.h"
#include "timer.h"

using namespace std;
#define WIDTH (640*2)
#define HEIGHT (480*2)

#define TILE_WIDTH 64
#define TILE_HEIGHT 32

/* Must supply also tile_z */
inline void screen_to_tile(int sx, int sy, float & tile_x, float & tile_y, float tile_z)
{
  tile_x = (float)sx/TILE_WIDTH + (float)sy/TILE_HEIGHT + tile_z/2;
  tile_y = (float)sy/TILE_HEIGHT + tile_z/2 -  (float)sx/TILE_WIDTH;
}

inline void tile_to_screen(float tile_x, float tile_y, float tile_z, int & sx, int & sy)
{
  sx = (tile_x - tile_y) * TILE_WIDTH/2;
  sy = (tile_x + tile_y - tile_z) * TILE_HEIGHT/2;
}

class Mouse : public CityItem
{
public:
  Mouse() : CityItem()
  {
    tz = 9;
  }

  virtual void update()
  {
    SDL_GetMouseState(&sx, &sy);
    screen_to_tile(sx+camera.x, sy+camera.y, tx, ty, tz);
//    cout << "Mouse pos " << sx << "x" << sy << "->" << tx << "x" << ty << "x" << tz << endl;
  }

  virtual void draw(Raster * to)
  {
    Rect t;
    t.x = sx; t.y = sy;
    image->blit(NULL, to, &t);
  }

  int sx, sy;
  Rect camera;
  Raster * image;
};


int main( int argc, char* argv[] )
{
  GfxManager gfx;
  srand(42);
  //Start
  SDL_Init( SDL_INIT_EVERYTHING );

  Screen * screen;

  screen = new Screen(WIDTH, HEIGHT, "X-Com 42");

  Raster * menu = gfx.getRaster("ufodata/buybase2.pcx", false);
  menu->zoom2x();

  SpritePack *city = gfx.getPack("ufodata/CITY");

  SpritePack *ptang = gfx.getPack("ufodata/PTANG");

  SpritePack *ufos = gfx.getPack("ufodata/SAUCER");

  SpritePack *invalid = gfx.getPack("ufodata/DESCURS");

  Raster *mouse_img = gfx.getRaster("cursor.png");

  CityMap cm(100,100,10);

  cout << "Loading city map..." << std::flush;

  bool res;

  if (argc <2)
    res = cm.load("ufodata/citymap1");
  else
    res = cm.load(argv[1]);

  cout << res << std::endl;

  Rect s;
  Rect camera;

  s.x = s.y = 0;
  camera.x = 0;
  camera.y = 0;
  camera.w = WIDTH;
  camera.h = HEIGHT * 0.76;

  bool quit = false;

  CityItem * ci1, *ci2, *ufo;
  Mouse * mouse;
  std::list<CityItem *> items;
  std::list<CityItem *> temp_items;
  ci1 = new DimensionGate();
  ci2 = new DimensionGate();
  ufo = new Ufo(ci1, ci2);
  mouse = new Mouse();

  ci1->images = ptang;
  ci2->images = ptang;
  ci2->frame = ci1->frame = ci1->start_frame = ci2->start_frame = 42;
  ci1->end_frame = ci2->end_frame = 73;

  ufo->images = ufos;
  ufo->start_frame = ufo->frame = 92;
  ufo->end_frame = 94;
  ufo->anim_speed = 0.3;

  //mouse->images = mouse_img;
  mouse->start_frame = mouse->end_frame = mouse->frame = 0;
  mouse->tx = 10; mouse->ty = 10; mouse->tz = 9;
  mouse->image = mouse_img;

  items.push_back(ci1);
  items.push_back(ci2);
  items.push_back(ufo);
//  items.push_back(mouse);

  FpsTimer timer(30);

  while (!quit)
  {
    timer.startOfFrame();


    /* Update animations etc */
    list<CityItem*> to_remove;

    BOOST_FOREACH(CityItem *i, items)
    {
      i->update();
    }

    BOOST_FOREACH(CityItem *i, temp_items)
    {
      i->update();
      if (i->garbage())
        to_remove.push_back(i);
    }

    BOOST_FOREACH(CityItem *i, to_remove)
    {
      temp_items.remove(i);
      delete i;
    }

    mouse->update();

    /* Drawing */
    screen->clear();

    for (int tz = 0; tz<10; tz++)
    {
      int ftx = 0, ltx=100, fty=0, lty=100;

      for (int tx=ftx; tx<ltx; tx++)
      {
        for (int ty=fty; ty<lty; ty++)
        {
          Tile * t = &cm.map[tx][ty][tz];
          int sx, sy;
          tile_to_screen(tx, ty, tz, sx, sy);
          if (sx+TILE_WIDTH > camera.x && sy+TILE_HEIGHT+15> camera.y && sx - camera.x <camera.w && sy - camera.y <camera.h)
          {
            sx -= camera.x;
            sy -= camera.y;
            s.x = sx;
            s.y = sy;
            if (t->tile)
            {
              if (t->tile < city->count())
                city->getSprite(t->tile)->blit(NULL, screen, &s);
              else
                invalid->getSprite(1)->blit(NULL, screen, &s);
            }
          }
        }
      }

      BOOST_FOREACH(CityItem *i, temp_items)
      {
        int sx, sy;
        if ((int)i->tz == tz)
        {
          tile_to_screen(i->tx, i->ty, i->tz, sx, sy);
          sx -= camera.x;
          sy -= camera.y;
          s.x = sx;
          s.y = sy;
          // Don't clip for now - SDL should do the clipping anyway
          i->blit(screen, &s);
        }
      }

      BOOST_FOREACH(CityItem *i, items)
      {
        int sx, sy;
        if ((int)i->tz == tz)
        {
          tile_to_screen(i->tx, i->ty, i->tz, sx, sy);
          sx -= camera.x;
          sy -= camera.y;
          s.x = sx;
          s.y = sy;
          // Don't clip for now - SDL should do the clipping anyway
          i->blit(screen, &s);
        }
      }
    }

    menu->blit(NULL, screen, NULL);
    mouse->draw(screen);

    screen->flip();

    /* EVENT processing */

    SDL_Event event;
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
      if (event.type == SDL_MOUSEBUTTONUP)
      {
        SingleShotItem * shot = new SingleShotItem();

        shot->tx = mouse->tx;
        shot->ty = mouse->ty;
        shot->tz = mouse->tz;
        shot->anim_speed=1;
        shot->images = ptang;
        switch(event.button.button)
        {
        case SDL_BUTTON_RIGHT:
          shot->start_frame = 8; shot->end_frame = 25;
          break;

        case SDL_BUTTON_MIDDLE:
          shot->start_frame =74; shot->end_frame=83;
          break;

        case SDL_BUTTON_LEFT:
          shot->start_frame =0; shot->end_frame=7;
          break;

        default:
          shot->start_frame =29; shot->end_frame=41;
          break;
          break;
        }
        shot->frame = shot->start_frame - shot->anim_speed;
        temp_items.push_back(shot);
      }
    }

    Uint8 *keystates = SDL_GetKeyState(NULL);
    int speed = 5;

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
    if (keystates[SDLK_HOME])
    {
      int cx, cy;
      tile_to_screen(ufo->tx, ufo->ty, ufo->tz, cx, cy);
      camera.x = cx - WIDTH/2;
      camera.y = cy - HEIGHT/2;
    }
    mouse->camera = camera;

    timer.endOfFrame();
  }

  //Quit
  SDL_Quit();

  return 0;
}
