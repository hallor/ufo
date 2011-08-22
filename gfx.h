#ifndef GFX_H
#define GFX_H

#include <string>
#include <map>
#include <list>
#include <SDL/SDL.h>
#include <SDL/SDL_rotozoom.h>
#include <GL/glew.h>
#include "shaderprogram.h"

typedef SDL_Rect Rect;

SDL_Surface * loadTexture(const char * filename, bool colorKey);
bool loadSurfacetoVram(SDL_Surface * surface, GLuint & tex_id, Rect & tex_rect);
bool unloadSurfacefromVram(GLuint & tex_id);

// Base class for all drawable items
class Raster
{
public:
  Raster() : surface(NULL), in_vram(false) { zoom=1;}

  ~Raster()
  {
    SDL_FreeSurface(surface);
    if (in_vram)
      unloadSurfacefromVram(tex_id);
  }

  const Rect rect() const
  {
    Rect r;
    r.w = surface->w;
    r.h = surface->h;
    return r;
  }

  inline void renderAt(const Rect & pos, const ShaderProgram & sp)
  {
    if (in_vram)
    {
//      int my_sampler_uniform_location = glGetUniformLocation(sp.get_sp(), "tex");
      glBindTexture(GL_TEXTURE_2D, tex_id);
//      glUniform1iARB(my_sampler_uniform_location, GL_TEXTURE0);

      glMatrixMode(GL_TEXTURE);
      glLoadIdentity();
      glScalef(sx, sy, 1);
      glBegin(GL_QUADS);
        glTexCoord2i( 0, 0 ); glVertex3f(pos.x*zoom, pos.y*zoom, 0);
        glTexCoord2i( (float)surface->w, 0 ); glVertex3f(pos.x*zoom+surface->w*zoom, pos.y*zoom, 0);
        glTexCoord2i( (float)surface->w, (float)surface->h); glVertex3f(pos.x*zoom+surface->w*zoom, pos.y*zoom+ surface->h*zoom, 0);
        glTexCoord2i( 0, (float)surface->h); glVertex3f(pos.x, pos.y*zoom+surface->h*zoom, 0);
      glEnd();
    } else
      std::cout << "Surface not in vram!" << std::endl;
  }

  void setSurface(SDL_Surface * surf)
  {
    if (surface)
      SDL_FreeSurface(surface);
    if (in_vram)
    {
      unloadSurfacefromVram(tex_id);
      in_vram = false;
    }

    surface = surf;
    if (loadSurfacetoVram(surface, tex_id, gl_tex_size))
    {
      in_vram = true;
      sx = 1.0/gl_tex_size.w;
      sy = 1.0/gl_tex_size.h;
    }
  }

  void setZoom(int n) { zoom = n; }

  GLuint tex_id;
private:
  SDL_Surface * surface; // Copy of texture - usually stored in vram - this is texture with original size/bpp (To be converted)
  float zoom;
  Rect gl_tex_size;
  float sx, sy;
  bool in_vram; // if set - tex_id can be used, else we must copy surface to vram
};

class Sprite : public Raster
{
  friend class SpritePack;
public:
  ~Sprite()
  {
  }
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
