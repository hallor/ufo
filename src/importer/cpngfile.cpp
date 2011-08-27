#include <png.h>
#include <png++/png.hpp>

#include "cpngfile.h"

using namespace Importer;

cPNGFile::cPNGFile()
{
  m_Raster = NULL;
}

cPNGFile::~cPNGFile()
{
  clean();
}

bool cPNGFile::setRaster(tRGBA * raster, int w, int h)
{
  clean();
  if (raster && w > 0 && h > 0)
  {
    m_Raster = raster;
    m_Width = w;
    m_Height = h;
  }
  return isValid();
}

bool cPNGFile::load(std::istream & file)
{
  clean();

  png::image<png::rgba_pixel> image;
  image.read_stream<std::istream>(file);

  if (!image.get_width() || !image.get_height())
    return false;

  m_Raster = new tRGBA [image.get_width() * image.get_height()];
  sRGBA * s = (sRGBA *) m_Raster;

  for (size_t y = 0; y < image.get_height(); ++y)
  {
      for (size_t x = 0; x < image.get_width(); ++x)
      {
        png::rgba_pixel & px = image[y][x];
        s->r = px.red;
        s->g = px.green;
        s->b = px.blue;
        s->a = px.alpha;
        s++;
      }
  }
  return true;
}

bool cPNGFile::save(std::ostream & file) const
{
  if (!isValid())
    return false;

  sRGBA * s = (sRGBA *) m_Raster;

  png::image< png::rgba_pixel > image(m_Width, m_Height);

  for (size_t y = 0; y < image.get_height(); ++y)
  {
      for (size_t x = 0; x < image.get_width(); ++x)
      {
          image[y][x] = png::rgba_pixel(s->r, s->g, s->b, s->a);
          s++;
      }
  }
  image.write_stream<std::ostream>(file);
  return true;
}
