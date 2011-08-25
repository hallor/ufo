#pragma once

#include <stdint.h>
#include <vector>
#include <iostream>

namespace Importer {

/** Single palette entry */
struct SRGBcolor {
  uint8_t r, g, b;
} __attribute__((packed));

/** Class represeting 8-bit palette */
class cPalette
{
public:
  cPalette();

  /** Loads palette from stream. */
  bool loadFrom(std::istream &file);

  /** Saves palette to stream (raw-data format). */
  bool saveTo(std::ostream & file) const;

  const SRGBcolor & color(uint8_t pos) const;

  bool isValid() const;
  operator bool() const { return isValid(); }

  void clear();

private:
  std::vector<SRGBcolor> m_Data;
  bool m_Valid;
};

};
