#include "palette.h"
#include <string>
#include <exception>
#include <stdexcept>

using namespace std;
using namespace Importer;

cPalette::cPalette() : m_Valid(false)
{
}


bool cPalette::loadFrom(std::istream &file)
{
  clear();

  m_Data.reserve(256);

  while (file.good())
  {
    SRGBcolor c;

    file.read((char*)&c, sizeof(c));

    if (file.gcount() == sizeof(c))
      m_Data.push_back(c);
    else
      break;
  }

  return m_Data.size() == 256;
}

bool cPalette::saveTo(std::ostream & file) const
{
  if (!isValid())
    return false;
  size_t s = m_Data.size();
  size_t i=0;

  for (i=0; i<s; ++i)
  {
    if (file.good())
      file.write((char*)&m_Data[i], sizeof(SRGBcolor));
    else break;
  }

  return i == m_Data.size();
}

const SRGBcolor & cPalette::color(uint8_t pos) const
{
  if(pos >= m_Data.size())
    throw std::out_of_range("Invalid color requested.");

  return m_Data[pos];
};

bool cPalette::isValid() const
{
  return m_Valid && m_Data.size() == 256;
}

void cPalette::clear()
{
  m_Data.clear();
  m_Valid = false;
};
