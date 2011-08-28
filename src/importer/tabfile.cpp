#include "tabfile.h"
#include "..\logger.h"

using namespace Importer;

cTabFile::cTabFile() : m_Valid(false)
{
}

bool cTabFile::loadFrom(std::istream &file)
//bool cTabFile::loadFrom(const std::string &base_name)
{
  clear();

//  std::string fn(base_name+".tab");

//  if (!filesystem::exists(fn))
//    std::transform(fn.begin(), fn.end(), fn.begin(), (int(*)(int))std::toupper);
//  if (!filesystem::exists(fn))
//    std::transform(fn.begin(), fn.end(), fn.begin(), (int(*)(int))std::tolower);

//  if (!filesystem::exists(fn) && !filesystem::is_regular_file(fn))
//    return false;

//  std::ifstream file(fn.c_str());


  //Warning - code not safe (race conditions if file is deleted/written to).

//  LogDebug("Opened file %s, size %i.", fn.c_str(), filesystem::file_size(fn));

  if (!file.good())
    return false;

  // Number of records in file
//  int num_records = filesystem::file_size(fn) / sizeof(int);

//  if (num_records < 1)
//    return false;

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

  return isValid();
}

int cTabFile::getImageOffset(int img) const
{
  if(img < 0 || img >= getOffsetsCount())
    return -1;

  return m_Offsets[img];
}

void cTabFile::performInternalCheck()
{
  if(!getOffsetsCount())
  {
    m_Valid = false;
    return;
  }

  int prev = -1;
  m_maxPackedImageSize = 0;

  for(int i = 0; i < getOffsetsCount(); ++i)
  {
    int next = getImageOffset(i);
    if(next <= prev)
    {
      clear();
      return;
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
