#pragma once
#include <iostream>
#include "importer/palette.h"
#include "surface.h"

/** Class keeping 8-bit raw images - no palette information is attached!*/
class c8bppBitmap
{
public:
  c8bppBitmap();
  ~c8bppBitmap();

  /** Returns _write'able_ pixel of bitmap, in case of invalid coordinates exception is thrown. */
	tPixel & pixel(int x, int y) throw();

  /** Returns copy of pixel (for rendering). In case of invalid coordinates 0 is returned. */
	tPixel pixel(int x, int y) const;

  /** Renders image using palette pal - image is then appropriate to upload as texture.
    Image format - RGBA. Color-key is converted to alpha channel (controlled by palette).
    Resulting image is owned by caller.
    */
	Surface render(const cPalette & pal) const throw();

  /** Creates empty bitmap with specified width/height */
	void create(int width, int height) throw();

  /** Class takes ownership of pixel data */
	void createFrom(tPixel * data, int width, int height) throw();

  /** Fills bitmap with pixels of selected color */
  void clear(const tPixel clear_color = 0);

  int width() const { return m_Width; };
  int height() const { return m_Height; };

  /** Saves bitmap to stream */
	void saveTo(std::ostream &file) throw();

  /** Removes resources allocated by bitmap */
  void free();

	bool isValid() const { return m_PixelData && m_Width > 0 && m_Height > 0; }

private:
  /** Disable copying for now */
  c8bppBitmap(const c8bppBitmap &);
  c8bppBitmap & operator=(const c8bppBitmap &);

  int m_Width, m_Height;
  tPixel *m_PixelData;
};

