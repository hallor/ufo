#ifndef MOUSE_H
#define MOUSE_H

#include "utils.h"
#include "cityitem.h"

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
    Utils::screen_to_tile(sx+camera.x, sy+camera.y, tx, ty, tz);
  }

  virtual void renderAt(Rect & /*pos*/)
  {
    Rect t;
    t.x = sx; t.y = sy;
    image->renderAt(t);
  }

  int sx, sy;
  Rect camera;
  Raster * image;
};

#endif // MOUSE_H
