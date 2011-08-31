#ifndef SURFACE_H
#define SURFACE_H
#include "importer/palette.h"

//typedef uint32_t tRGBA;

//typedef struct {
//  uint8_t r;
//  uint8_t g;
//  uint8_t b;
//  uint8_t a;
//} __attribute__((packed)) sRGBA;

//typedef union {
//  tRGBA t;
//  sRGBA s;
//} uRGBA;

//typedef uint8_t tPixel;


struct Surface
{
  Surface() : pixels(0), w(0), h(0) {}

  bool isValid() const { return pixels && w>0 && h >0; }

  bool set(tRGBA *p, int w, int h)
  {
    clean();
    pixels = p;
    this->w = w; this->h = h;
    return isValid();
  }

  void clean()
  {
    delete [] pixels;
    pixels = 0;
    w = h = 0;
  }

  tRGBA * pixels;
  int w, h;
};

#endif // SURFACE_H
