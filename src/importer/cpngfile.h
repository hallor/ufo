#ifndef CPNGFILE_H
#define CPNGFILE_H

#include <iostream>
#include <string>
#include "palette.h"

namespace Importer {

#ifdef HAS_LIBPNG
class cPNGFile
{
public:
    cPNGFile();
    ~cPNGFile();

    bool load(std::istream & file);
    bool save(std::ostream & file) const;

    const tRGBA * getRaster() const { return m_Raster; }
    /** Sets raster, pngfile becomes owner of it */
    bool setRaster(tRGBA * raster, int w, int h);

    int width() const { return m_Width; };
    int height() const { return m_Height; };

    bool isValid() const { return m_Raster && m_Width > 0 && m_Height > 0; }

    void clean() { delete [] m_Raster; m_Raster = 0; }

private:
    tRGBA * m_Raster;
    int m_Width;
    int m_Height;
};
#endif

};
#endif // CPNGFILE_H
