#pragma once

#include <stdint.h>
#include <iostream>
#include <string>

#include "palette.h"
namespace Importer
{
/** Class keeping 8-bit raw images - no palette information is attached!*/
class c8bppBitmap
{
public:

  c8bppBitmap();

  ~c8bppBitmap();

  /** Returns _write'able_ pixel of bitmap, in case of invalid coordinates exception is thrown. */
  tPixel & pixel(int x, int y);

  /** Returns copy of pixel (for rendering). In case of invalid coordinates 0 is returned. */
  tPixel pixel(int x, int y) const;

  /** Renders image using palette pal - image is then appropriate to upload as texture.
    Image format - RGBA. Color-key is converted to alpha channel (controlled by palette).
    Resulting image is owned by caller.
    */
  tRGBA * render(const cPalette & pal) const;

  bool create(int width, int height);

  /** Class takes ownership of pixel data */
  bool createFrom(tPixel * data, int width, int height)
  {
    free();
    m_PixelData = data;
    m_Width = width;
    m_Height = height;
    return data && width > 0 && height > 0;
  }

  void clear(const tPixel clear_color = 0);

  int width() const { return m_Width; };
  int height() const { return m_Height; };

  bool saveTo(std::ostream &file);

  void free();

protected:
  int m_Width, m_Height;
  tPixel *m_PixelData;
};

}
