#ifndef CURSOR_H
#define CURSOR_H
#include <iostream>
#include <cstring>
#include "surface.h"
#include "logger.h"
#include "bitmap.h"
#include "utils.h"

/** Class used to load MOUSE.DAT-like cursors */
class cCursor
{
public:
    cCursor() : raster_count(0) { }

    ~cCursor()
    {
      clear();
    }

    void clear()
    {
			raster.clear();
      raster_count = 0;
    }

    /* Does seek on stream to figure out number of images */
    bool loadFrom(std::istream & file, const cPalette & pal)
    {
      file.seekg(0, std::ios::end);
      int size = (int)file.tellg();
      file.seekg(0, std::ios::beg);

      if (!file.good() || !pal.isValid())
        return false;

      if (size % (24*24))
        LogWarning("Mouse cursor file has junk at the end.");

      raster_count = size / (24*24);

      c8bppBitmap bmp; //bitmap will be power-of-2 from the beginning

      int height = Utils::normalizeResolution(24*raster_count);

      tPixel * pixels = new tPixel[height*32];
      memset(pixels, 0, height* 32 * sizeof(tPixel));

      // have to read line-by-line
      tPixel *px = pixels;
      for (int i=0; i<24*raster_count && file.good(); i++)
      {
        file.read((char*)px, 24*sizeof(tPixel));
        px += 32;
      }

      bmp.createFrom(pixels, 32, height);

      /* We can return here as bmp owns pixels now */
      if (!file.good())
        return false;

			raster = bmp.render(pal);

      LogInfo("Loaded %i mouse cursors.", raster_count);

      return raster.isValid();
    }

    Surface * getSurface()
    {
      if (!isValid())
        return NULL;
      return &raster;
    }

    bool isValid() { return raster.isValid() && raster_count > 0; }

private:
    Surface raster;
    int raster_count;
};

#endif // CURSOR_H
