#include "palette.h"
#include <string>
#include <exception>
#include <stdexcept>

using namespace std;

#include "logger.h"

cPalette::cPalette() : m_Valid(false)
{
}


bool cPalette::loadFrom(std::istream &file, int colorKey, int num_colors)
{
  clear();

  m_Data.reserve(num_colors);
  int rr = 0;

  while (file.good())
  {
    uint8_t x[3];
    sRGBA c;

    // Read entry
    file.read((char*)x, sizeof(x));
    c.r = x[0];
    c.g = x[1]; 
    c.b = x[2]; 
    c.a = x[0] == 0xCC || x[1] == 0xCC || x[2] == 0xC4 ? 0x00 : 0xff;
    //c = (tRGBA)x[0] << 24 | x[1] << 16 | x[2] << 8;

    if (file.gcount() == sizeof(x))
    {
      m_Data.push_back(*(tRGBA*)&c);
      ++rr;
    }
    else
      break;

    if (num_colors && rr == num_colors)
      break;
  }

  m_Valid = m_Data.size() == 256 || m_Data.size() == 16;


  if (m_Valid && colorKey >=0 && colorKey < (int)m_Data.size())
    m_Data[colorKey] = 0;


  LogDebug("Loaded palette with %i colors.", m_Data.size());

  return m_Valid;
}

bool cPalette::saveTo(std::ostream & file) const
{
  if (!isValid())
    return false;
  size_t s = m_Data.size();
  size_t i=0;
  uint8_t x[3];

  for (i=0; i<s; ++i)
  {
    tRGBA q = m_Data[i];

    x[0] = (q >> 24);
    x[1] = (q >> 16) & 0xFF;
    x[2] = (q >> 8) & 0xFF;

    if (file.good())
      file.write((char*)x, sizeof(x));
    else break;
  }

  return i == m_Data.size();
}

bool cPalette::isValid() const
{
  return m_Valid && (m_Data.size() == 256 || m_Data.size() == 16);
}

void cPalette::clear()
{
  m_Data.clear();
  m_Valid = false;
};
