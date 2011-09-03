#include <stdexcept>
#include <cstring>
#include "bitmap.h"

#include "exceptions.h"

c8bppBitmap::c8bppBitmap() : m_Width(0), m_Height(0), m_PixelData(0)
{
}

c8bppBitmap::~c8bppBitmap()
{
  free();
}

void c8bppBitmap::createFrom(tPixel * data, int width, int height) throw()
{
  free();
	if (!data || width<=0 || height <=0)
		throw exceptions::invalid_arguments();

  m_PixelData = data;
  m_Width = width;
  m_Height = height;
}


tPixel & c8bppBitmap::pixel(int x, int y) throw()
{
  if(x < 0 || x >= m_Width)
    throw std::out_of_range("Invalid width requested.");

  if(y < 0 || y >= m_Height)
    throw std::out_of_range("Invalid height requested.");

  return m_PixelData[x + m_Width * y];
}

tPixel c8bppBitmap::pixel(int x, int y) const
{
  if(x < 0 || x >= m_Width)
    return 0;

  if(y < 0 || y >= m_Height)
    return 0;

  return m_PixelData[x + m_Width * y];
}

Surface c8bppBitmap::render(const cPalette & pal) const throw()
{
  int size = m_Width * m_Height;

  if (size < 1)
		throw exceptions::invalid_resource();

	if (!pal.isValid())
		throw exceptions::invalid_arguments();

  tRGBA * x = new tRGBA[size];

  for (int i=0; i<size; ++i)
    x[i] = pal.colorRGBA(m_PixelData[i]);
	Surface q;
	q.set(x, m_Width, m_Height);
	return q;
}


void c8bppBitmap::create(int width, int height) throw()
{
  free();

  if(width <= 0 || height <= 0)
		throw exceptions::invalid_arguments();

  m_PixelData = new tPixel[width * height];
  clear();

  m_Width = width;
  m_Height = height;
}

void c8bppBitmap::clear(const tPixel clear_color)
{
  if (m_PixelData)
    memset(m_PixelData, clear_color, m_Width*m_Height);
}

void c8bppBitmap::saveTo(std::ostream &file) throw()
{
	if (!isValid())
		throw exceptions::invalid_resource();
	if (!file.good())
		throw exceptions::invalid_arguments();

	file.write((char*)m_PixelData, m_Width * m_Height);

	if (file.bad())
		throw exceptions::io();
}

void c8bppBitmap::free()
{
  if(m_PixelData)
    delete [] m_PixelData;

  m_Width = 0;
  m_Height = 0;
  m_PixelData = NULL;
}
