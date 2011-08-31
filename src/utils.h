#ifndef UTILS_H
#define UTILS_H

#define WIDTH (640*2)
#define HEIGHT (480*2)

#define TILE_WIDTH 64
#define TILE_HEIGHT 32

namespace Utils
{
  inline void tile_to_screen(float tile_x, float tile_y, float tile_z, int & sx, int & sy)
  {
    sx = (int)((tile_x - tile_y) * TILE_WIDTH/2);
    sy = (int)((tile_x + tile_y - tile_z) * TILE_HEIGHT/2);
  }

  /* Must supply also tile_z */
  inline void screen_to_tile(int sx, int sy, float & tile_x, float & tile_y, float tile_z)
  {
    tile_x = (float)sx/TILE_WIDTH + (float)sy/TILE_HEIGHT + tile_z/2;
    tile_y = (float)sy/TILE_HEIGHT + tile_z/2 -  (float)sx/TILE_WIDTH;
  }
};

#endif // UTILS_H
