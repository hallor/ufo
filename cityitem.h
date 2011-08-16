#ifndef CITYITEM_H
#define CITYITEM_H

#include <SDL/SDL.h>
#include <stdlib.h>
#include <iostream>
#include "gfx.h"


class CityItem
{
public:
    CityItem()
    {
      tx = 0; ty = 0; tz = 8;
      frame = 0;
    }

    /* Called in the beginnig on every frame */
    virtual void update()
    {
      frame++;
      if (frame/2 > end_frame)
        frame = start_frame*2;
    }

    void blit(SDL_Surface * to, SDL_Rect * rect)
    {
      SDL_BlitSurface(images->getSprite(frame/2)->img, NULL, to, rect);
    }

    SpritePack * images;
    int start_frame, end_frame, frame;
    int tx, ty, tz;
};

class DimensionGate : public CityItem
{
public:
  DimensionGate() : CityItem()
  {
    tx = 10 + rand() % 90; ty = 10 + rand() % 90; tz = 8;
    std::cout << "Generated gate item at" << tx << "x" << ty << std::endl;
  }

  void update()
  {
    CityItem::update();
    if (rand() % 30 == 0)
    {
      tx += (rand() % 3) - 1;
      ty += (rand() % 3) - 1;
      if (tx > 100) tx = 100;
      if (tx < 0) tx = 0;
      if (ty > 100) ty = 100;
      if (ty < 0) ty = 0;
    }
  }
};

class Ufo : public CityItem
{
public:
  Ufo(CityItem * source,CityItem * dest) : CityItem(), g_source(source), g_dest(dest)
  {
    tz = source->tz +1;
    tx = source->tx;
    ty = source->ty;
    speedaccum=0;
    std::cout << "Generated ufo at" << tx << "x" << ty << std::endl;
  }

  void update()
  {
    CityItem::update();
    if (tx != g_dest->tx || ty != g_dest->ty)
    {
      speedaccum++;
      if (speedaccum > 10)
      {
        speedaccum=0;
        if (tx != g_dest->tx)
          tx += (g_dest->tx - tx) / abs((g_dest->tx - tx));
        if (ty != g_dest->ty)
          ty += (g_dest->ty - ty) / abs((g_dest->ty - ty));
      }
    } else // swap gates
    {
      CityItem * d = g_source;
      g_source = g_dest;
      g_dest = d;
    }
  }

  CityItem * g_source, *g_dest;
  int speedaccum;
};

#endif // CITYITEM_H
