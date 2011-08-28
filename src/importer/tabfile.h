#pragma once

#include <vector>
#include <iostream>


/** Class responsible for loading, and parsing of .tab files
  Used as a helper by @see cPckFile
*/
class cTabFile
{
public:
  cTabFile();

  //TODO: make this classess accept stream not filename
  /** Loads .tab file. Extension is added automatically.
      If file is not found, it's upper/lower case versions are searched before failing.
  */
  bool loadFrom(std::istream &file);

  /** Returns number of indices loaded from .tab file.*/
  int getOffsetsCount() const { return m_Offsets.size(); }

  /** Returns offset (in bytes) of image in .pck file. */
  int getImageOffset(int image) const;

  /** Returns true if data in .tab file was parsed correctly. */
  bool isValid() const { return m_Valid; }
  operator bool() const { return isValid(); }

  /** Returns size of largest (packed) image (in bytes) */
  int getMaxPackedImageSize() const { return m_maxPackedImageSize; }

  /** Removes indices loaded by @sa loadFrom */
  void clear();

private:
  void performInternalCheck();

  bool m_Valid;
  int m_maxPackedImageSize;
  std::vector<int> m_Offsets;
};

