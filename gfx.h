#ifndef GFX_H
#define GFX_H

#include <string>
#include <map>
#include <list>
#include <SDL/SDL.h>
#include <SDL/SDL_rotozoom.h>


typedef SDL_Rect Rect;

// Base class for all drawable items
class Raster
{
  friend class GfxManager;
public:
  Raster() : surface(NULL) {}

  ~Raster()
  {
    SDL_FreeSurface(surface);
  }

  const Rect & rect() const { return surface_rect; }

  inline void blit(Rect * src_rect, Raster * to, Rect * to_rect)
  {
    SDL_BlitSurface(surface, src_rect, to->surface, to_rect);
  }

  void zoom(float zoom_x, float zoom_y)
  {
    SDL_Surface * x = zoomSurface(surface, zoom_x, zoom_y, 0);
    SDL_FreeSurface(surface);
    surface = x;
  }

protected:

  void setSurface(SDL_Surface * surf)
  {
    if (surface)
      SDL_FreeSurface(surface);
    surface = surf;
    surface_rect.w = surface->w;
    surface_rect.h = surface->h;
  }

  SDL_Surface * surface;
  Rect surface_rect;
};

class Screen : public Raster
{
public:
  Screen(int width, int height, std::string window_title="ufo") : Raster()
  {
    surface = SDL_SetVideoMode(width, height, 32, SDL_SWSURFACE);
    SDL_WM_SetCaption(window_title.c_str(), NULL);
    SDL_ShowCursor(false);
    SDL_FillRect(surface, NULL, 0);
  }

  void flip()
  {
    SDL_Flip(surface);
  }

  void clear()
  {
    SDL_FillRect(surface, NULL, 1);
  }

};

class Sprite : public Raster
{
  friend class SpritePack;
public:
  ~Sprite()
  {
  }
protected:
  Sprite(SDL_Surface *s) : Raster()
  {
    setSurface(s);
  }
};

class SpritePack
{
  friend class GfxManager;
public:
  Sprite * getSprite(size_t no);
  ~SpritePack();
  size_t count() const { return sprite_count; }

private:
  SpritePack(const std::list<SDL_Surface*> & s);

  Sprite ** sprites;
  size_t sprite_count;
  std::string origin;
};

class GfxManager
{
  typedef std::map<std::string, SpritePack *> GfxMap;
  typedef std::map<std::string, Raster *> RasterMap;
public:

  ~GfxManager();

  /* TODO: rewrite */
  SpritePack * getPack(const std::string & filename);
  Raster * getRaster(const std::string & filename, bool colorKey = true);

private:
  GfxMap resources;
  RasterMap rasters;
};

#endif
