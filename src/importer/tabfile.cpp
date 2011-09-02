#include "tabfile.h"
#include "logger.h"
#include "exceptions.h"

cTabFile::cTabFile() : m_Valid(false)
{
}

void cTabFile::loadFrom(std::istream &file)  throw()
{
  clear();

  if (!file.good())
		throw exceptions::load_resource();

  m_Offsets.reserve(100);

  while(file.good())
  {
    int offset = 0;
    file.read((char*)&offset, sizeof(offset));

    if (file.good())
        m_Offsets.push_back(offset * 4);
  }

  performInternalCheck();

  LogDebug("Loaded %i offsets.", getOffsetsCount());
}

int cTabFile::getImageOffset(int img) const
{
  if(img < 0 || img >= getOffsetsCount())
    return -1;

  return m_Offsets[img];
}

void cTabFile::performInternalCheck() throw()
{
  if(!getOffsetsCount())
  {
    m_Valid = false;
		throw exceptions::invalid_resource();
  }

  int prev = -1;
  m_maxPackedImageSize = 0;

  for(int i = 0; i < getOffsetsCount(); ++i)
  {
    int next = getImageOffset(i);
    if(next <= prev)
    {
      clear();
			throw exceptions::invalid_resource();
    }
    if (i>0)
      m_maxPackedImageSize = std::max(m_maxPackedImageSize, next - prev);
    prev = next;
  }

  m_Valid = true;
}

void cTabFile::clear()
{
  m_Valid = false;
  m_Offsets.clear();
  m_maxPackedImageSize = 0;
}
