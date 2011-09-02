#include "palette.h"
#include <string>
#include <exception>
#include <stdexcept>

using namespace std;

#include "logger.h"
#include "exceptions.h"

cPalette::cPalette() :  m_Valid(false)
{
    sRGBA x;
    x.r = 0xFF;
    x.g = 0xFF;
    x.b = 0xFF;
    x.a = 0xFF;
    m_Invalid = *((tRGBA*)&x);
}

tRGBA cPalette::colorRGBA(uint8_t pos) const
{
    if (pos < m_Data.size())
      return m_Data[pos];
    return m_Invalid;
}

void cPalette::loadFrom(std::istream &file, int colorKey, int num_colors) throw()
{
  int rr = 0;
  clear();

  m_Data.reserve(num_colors);

  sRGBA c = {0,0,0,0};
  while (file.good())
  {
    // Read entry
    file.read((char*)&c, 3);
    c.a = 0xff; //x[0] == 0xCC || x[1] == 0xCC || x[2] == 0xC4 ? 0x00 : 0xff;

    if (file.gcount() == 3)
    {
      m_Data.push_back(*(tRGBA*)&c);
      ++rr;
    }
    else
			throw exceptions::load_resource();

    if (num_colors && rr == num_colors)
      break;
  }

  m_Valid = m_Data.size() == 256 || m_Data.size() == 16;

  if (m_Valid && colorKey >=0 && colorKey < (int)m_Data.size())
    m_Data[colorKey] = 0;

	if (!m_Valid)
		throw exceptions::load_resource();
	LogDebug("Loaded palette with %i colors.", m_Data.size());
}

void cPalette::saveTo(std::ostream & file) const throw()
{
  if (!isValid())
		throw exceptions::invalid_resource();

  size_t s = m_Data.size();
  size_t i=0;

  sRGBA q = {0,0,0,0};
  for (i=0; i<s; ++i)
  {
    q = *((sRGBA*)&m_Data[i]);

		file.write((char*)&q, 3);
		if (!file.good())
			throw exceptions::io();
  }
}

bool cPalette::isValid() const
{
  return m_Valid && (m_Data.size() == 256 || m_Data.size() == 16);
}

void cPalette::clear()
{
  m_Data.clear();
  m_Valid = false;
}
