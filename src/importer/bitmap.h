#pragma once

#include <stdint.h>
#include <iostream>
#include <string>

namespace Importer
{

typedef uint8_t tPixel;

/** Class keeping 8-bit raw images - no palette information is attached!*/
class c8bppBitmap
{
public:

  c8bppBitmap();

  ~c8bppBitmap();

  tPixel & pixel(int x, int y);

  bool create(int width, int height);

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
