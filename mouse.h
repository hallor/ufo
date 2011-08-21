#ifndef MOUSE_H
#define MOUSE_H

#include "cityitem.h"

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
