#ifndef SURFACE_H
#define SURFACE_H
#include <vector>

#include "importer/palette.h"

/** RGBA image */
struct Surface
{
  Surface() : pixels(0), w(0), h(0) {}

  bool isValid() const { return pixels && w>0 && h >0; }

  bool set(tRGBA *p, int w, int h)
  {
		clear();
    pixels = p;
    this->w = w; this->h = h;
    return isValid();
  }

	void clear()
  {
    delete [] pixels;
    pixels = 0;
    w = h = 0;
  }

  tRGBA * pixels;
  int w, h;
};

/** Collection of RGBA images */
struct AnimatedSurface
{
	bool isValid() const { return frames.size() && frames[0].isValid(); }

	void addFrame(Surface frame)
	{
		frames.push_back(frame);
	}

	bool set(std::vector<Surface> f, float fps = 1.0)
	{
		clear();
		frames = f;
		this->fps = fps;
		return isValid();
	}

	void clear()
	{
		for (int i=0; i<frames.size(); ++i)
			frames[i].clear();
	}

	float fps; // default frames per second
	std::vector<Surface> frames;
};

#endif // SURFACE_H
