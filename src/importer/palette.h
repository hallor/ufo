#pragma once

#include <stdint.h>
#include <vector>
#include <iostream>

namespace Importer {

typedef uint32_t tRGBA;
typedef struct {
  uint8_t r;
  uint8_t g;
  uint8_t b;
  uint8_t a;
} __attribute__((packed)) sRGBA;

/** Class represeting 8-bit palette */
class cPalette
{
public:
  cPalette();

  /** Loads palette from stream.
  @warning Palette is loaded from "old-school" 24-bit format. Alpha is zeroed for entry colorKeyNo (if >0)
  */
  bool loadFrom(std::istream &file, int colorKeyNo=0, int num_colors=256);

  /** Saves palette to stream (raw-data format).
  @warning Palette is saved in "old-school" 24-bit format (alpha is ignored)
  */
  bool saveTo(std::ostream & file) const;

  /** Returns color asociated with position pos. If color is not valid exception is thrown. */
  tRGBA colorRGBA(uint8_t pos) const
  {
      if (pos < m_Data.size())
        return m_Data[pos];
      return 0;
  }

  bool isValid() const;
  operator bool() const { return isValid(); }

  void clear();

private:
  std::vector<tRGBA> m_Data;
  bool m_Valid;
};

};
