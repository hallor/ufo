#include <stdexcept>
#include <cstring>

#include "bitmap.h"

using namespace Importer;

c8bppBitmap::c8bppBitmap()
{
  m_PixelData = NULL;
  m_Width = m_Height = 0;
};

c8bppBitmap::~c8bppBitmap()
{
  free();
};

tPixel & c8bppBitmap::pixel(int x, int y)
{
  if(x < 0 || x >= m_Width)
    throw std::out_of_range("Invalid color requested.");

  if(y < 0 || y >= m_Height)
    throw std::out_of_range("Invalid color requested.");

  return m_PixelData[x + m_Width * y];
}

bool c8bppBitmap::create(int width, int height)
{
  free();

  if(width <= 0 || height <= 0)
    return false;

  m_PixelData = new tPixel[width * height];

  m_Width = width;
  m_Height = height;

  return m_PixelData;
};

void c8bppBitmap::clear(const tPixel clear_color)
{
  if (m_PixelData)
    memset(m_PixelData, clear_color, m_Width*m_Height);
}

bool c8bppBitmap::saveTo(std::ostream &file)
{
  if (!m_PixelData)
    return false;

  if (file.good())
    file.write((char*)m_PixelData, m_Width * m_Height);

  return !file.bad();
}

void c8bppBitmap::free()
{
  if(m_PixelData)
    delete [] m_PixelData;

  m_Width = 0;
  m_Height = 0;
  m_PixelData = NULL;
}
