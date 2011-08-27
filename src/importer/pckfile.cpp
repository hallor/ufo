#include <vector>
#include <stdexcept>
#include <cstring>

#include "pckfile.h"

#include "tabfile.h"
#include "bitmap.h"


#include "logger.h"

using namespace Importer;

static const int PckLookupTable[10] = {0, 0, 1, 1, 1, 2, 2, 3, 3, 3};

struct sPckHeader
{
  sPckHeader()
  {
    clear();
  }

  void clear()
  {
    memset(this, 0, sizeof(*this));
  }

  bool isValid() const
  {
    return m_Header == 1 && m_Width > 0 && m_Height > 0;
  }

  int m_Header;
  short m_MinSkipLeft;
  short m_Width;
  short m_SkipTop;
  short m_Height;
};

struct sPckEofTag
{
  sPckEofTag()
  {
    clear();
  }

  void clear()
  {
    memset(this, 0, sizeof(*this));
  }

  bool isValid() const { return m_Data[0] == 0xFFFFFFFF; }

  unsigned int m_Data[4];
};

struct sPckLineHeader
{
  sPckLineHeader()
  {
    clear();
  }

  void clear()
  {
    memset(this, 0, sizeof(*this));
  }

  unsigned char m_Checksum;
  unsigned char m_LineIndex;
  unsigned char m_Unknown1[2]; //
  unsigned char m_Skip;
  unsigned char m_DataLength;
  unsigned char m_Unknown2[2];
} __attribute__((packed));

class cPckLine
{
public:
  cPckLine()
  {
    m_Data = NULL;
    m_Valid = false;
  }

  ~cPckLine()
  {
    release();
  }

  void release()
  {
    if(m_Data)
      delete [] m_Data;

    m_Data = NULL;
    m_Header.clear();
    m_Valid = false;
  }

  bool makeFrom(void *data, int data_size);

  bool isValid() const { return m_Valid; }

  int getTotalDataSize() const { return isValid() ? sizeof(m_Header) + m_Header.m_DataLength : 0; }

  const sPckLineHeader &getHeader() const { return m_Header; }
  const unsigned char* getData() const { return m_Data; }

private:

   void createData(int size)
   {
     m_Data = new unsigned char[size];
   }

  sPckLineHeader m_Header;
  unsigned char *m_Data;
  bool m_Valid;
};

bool cPckLine::makeFrom(void *data, int data_size)
{
  release();
  createData(data_size);

  if(!data)
    return false;

  if(data_size < (int)sizeof(m_Header))
    return false;

  int data_done = 0;

  memcpy(&m_Header, data, sizeof(m_Header));
  data_done += sizeof(m_Header);

  if(m_Header.m_DataLength > data_size - data_done)
    return false;

  m_Data = new unsigned char[m_Header.m_DataLength];
  memcpy(m_Data, (unsigned char*)data + data_done, m_Header.m_DataLength);

  m_Valid = true;
  return true;
}

class cPckBitmap
{
public:
  cPckBitmap()
  {
    m_Valid = false;
  }

  ~cPckBitmap()
  {
    release();
  }

  void release()
  {
    for(unsigned int i = 0; i < m_Lines.size(); ++i)
      delete m_Lines[i];

    m_Header.clear();
    m_Lines.clear();
    m_Eof.clear();
    m_Valid = false;
  }

  int width() const { return isValid() ? m_Header.m_Width : 0; }
  int height() const { return isValid() ? m_Header.m_Height : 0; }

  int getLinesCount() const { return isValid() ? m_Lines.size() : 0; }

  bool isValid() const { return m_Valid; }

  bool makeFrom(void *data, int data_size);

  // Returns NULL on error, on success new object returned
  c8bppBitmap * generateBitmap() const;

private:

  sPckHeader m_Header;
  std::vector<cPckLine*> m_Lines;
  sPckEofTag m_Eof;

  bool m_Valid;
};

bool cPckBitmap::makeFrom(void *data, int data_size)
{
  release();

  if(!data)
    return false;

  if(data_size < (int)sizeof(m_Header) + (int)sizeof(m_Eof)) // header + eof size
    return false;

  int data_done = 0;

  memcpy(&m_Header, data, sizeof(m_Header));
  if(!m_Header.isValid())
    return false;

  data_done += sizeof(m_Header);

  while(data_done < data_size)
  {
    if(data_size - data_done < (int)sizeof(m_Eof))
      return false;

    memcpy(&m_Eof, (unsigned char*)data + data_done, sizeof(m_Eof));
    if(m_Eof.isValid())
    {
      data_done += sizeof(m_Eof);
    }
    else
    {
      cPckLine *line = new cPckLine;
      m_Lines.push_back(line);
      if(!line->makeFrom((unsigned char*)data + data_done, data_size - data_done))
      {
        return false;
      }
      else
      {
        data_done += line->getTotalDataSize();
      }
    }
  }

  m_Valid = true;
  return true;
}

// TODO: error checking
c8bppBitmap * cPckBitmap::generateBitmap() const
{
  c8bppBitmap * bmp = new c8bppBitmap();

  if (!bmp->create(m_Header.m_Width, m_Header.m_Height))
  {
    delete bmp;
    return NULL;
  }
  for (unsigned int l=0; l<m_Lines.size(); ++l)
  {
    cPckLine *line = m_Lines[l];

    int line_in_bmp = line->getHeader().m_LineIndex;

    line_in_bmp = (line_in_bmp / 10) * 4 + PckLookupTable[line_in_bmp % 10];

    if(line_in_bmp >= m_Header.m_Height)
      continue; //TODO: is it normal condition??

    const sPckLineHeader & hdr = line->getHeader();
    for(int j = hdr.m_Skip; j < hdr.m_DataLength + hdr.m_Skip && j < m_Header.m_Width; ++j)
    {
      int idx = line->getData()[j - hdr.m_Skip];
      bmp->pixel(j, /*m_Header.m_Height - */line_in_bmp) = idx;
    }
  }

  return bmp;
}

class cPckFile::Private
{
public:
  unsigned char *m_Buffer; // Kept here only to do proper destruction if needed
  cPckBitmap *m_Chunks;
  bool m_Valid;

  std::vector<c8bppBitmap*> bitmaps;

  Private()
  {
    m_Chunks = NULL;
    m_Buffer = NULL;
    m_Valid = false;
  }

  ~Private()
  {
    clean();
  }

  void clean()
  {
    m_Valid = false;

    if(m_Chunks)
      delete [] m_Chunks;
    m_Chunks = NULL;

    if(m_Buffer)
      delete [] m_Buffer;
    m_Buffer = NULL;

    for (unsigned int i=0; i<bitmaps.size(); ++i)
      delete bitmaps[i];
    bitmaps.clear();
  }
};

cPckFile::cPckFile()
{
  p = new Private();
}

cPckFile::~cPckFile()
{
  delete p;
}

bool cPckFile::loadPck(const cTabFile & f, std::istream & file)
{
  if (isValid())
    p->clean();

  if (!f.isValid())
    return false;

  if (!file.good())
    return false;

  file.seekg(0, std::ios::end);
  int file_size = file.tellg();
  file.seekg(0, std::ios::beg);

  int images_count = f.getOffsetsCount();
  int read = 0;

  /* Pre-allocate space for bitmaps, they will be created on-demand so for now fill with NULL*/
  p->bitmaps.resize(images_count, NULL);

  p->m_Chunks = new cPckBitmap[images_count];
  int max_data_size = f.getMaxPackedImageSize();

  max_data_size = std::max(max_data_size, file_size - f.getImageOffset(images_count - 1));

  p->m_Buffer = new unsigned char[max_data_size];

  int i=0;
  for (i=0; i<images_count; ++i)
  {
    int data_size = 0;

    if(i == images_count - 1)
      data_size = file_size - f.getImageOffset(images_count - 1);
    else
      data_size = f.getImageOffset(i + 1) - f.getImageOffset(i);

    if(data_size + read > file_size)
      break; //TODO: maybe add error here?? it's unexpected end of file after all.

    if (max_data_size < data_size)
      throw std::logic_error("Invalid size of allocated buffer.");

    file.read((char*)p->m_Buffer, data_size);

    if (!file.good() || file.gcount()!=data_size) // Failed reading
      return false;

    if (!p->m_Chunks[i].makeFrom(p->m_Buffer, data_size))
      LogError("Error loading chunk %i", i);
  }

  LogDebug("Loaded %i chunks from pck file.", i);

  // Free not needed buffer
  delete[] p->m_Buffer;
  p->m_Buffer = NULL;

  p->m_Valid = true;

  return true;
}


const c8bppBitmap *cPckFile::getBitmap(int no) const
{
  if (!isValid() || no >= (int)p->bitmaps.size())
    return NULL;

  if (!p->bitmaps[no])
    p->bitmaps[no] = p->m_Chunks[no].generateBitmap();

  return p->bitmaps[no];
}

int cPckFile::bitmapCount() const
{
  if (isValid())
    return p->bitmaps.size();
  return 0;
}

bool cPckFile::isValid() const
{
  return p->m_Valid;
}
