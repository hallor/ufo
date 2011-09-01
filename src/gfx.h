#ifndef GFX_H
#define GFX_H

#include <string>
#include <map>
#include <list>
#include <GL/glew.h>
#include <GL/gl.h>
#include <SDL/SDL.h>
#include "shaderprogram.h"
#include "importer/palette.h"
#include "surface.h"

typedef SDL_Rect Rect;

//SDL_Surface * loadTexture(const char * filename, bool colorKey);
//bool loadSurfacetoVram(SDL_Surface * surface, GLuint & tex_id, Rect & tex_rect);
bool unloadSurfacefromVram(GLuint & tex_id);

bool loadSurfacetoVram(Surface & surface, GLuint & tex_id, Rect & tex_rect);

// Base class for all drawable items
class Raster
{
public:
  Raster() : surface(), in_vram(false) { zoom=1;}

  ~Raster()
  {
    surface.clean();
    if (in_vram)
      unloadSurfacefromVram(tex_id);
  }

  const Rect rect() const
  {
    Rect r;
    r.w = surface.w;
    r.h = surface.h;
    return r;
  }

  inline void renderAt(const Rect & pos, const ShaderProgram & sp, float z)
  {
    z=z/20;
    if (in_vram)
    {
      glActiveTexture(GL_TEXTURE0); // We use texture 0 for images
      glBindTexture(GL_TEXTURE_2D, tex_id);

      glBegin(GL_QUADS);
        glTexCoord2f( 0, 0 ); glVertex3f(pos.x*zoom, pos.y*zoom, z);
        glTexCoord2f( sx, 0 ); glVertex3f(pos.x*zoom+surface.w*zoom, pos.y*zoom, z);
        glTexCoord2f( sx, sy); glVertex3f(pos.x*zoom+surface.w*zoom, pos.y*zoom+ surface.h*zoom, z);
        glTexCoord2f( 0, sy); glVertex3f(pos.x, pos.y*zoom+surface.h*zoom, z);
      glEnd();
    } else
      std::cout << "Surface not in vram!" << std::endl;
  }

  void setSurface(Surface surf)
  {
    surface.clean();

    if (in_vram)
    {
      unloadSurfacefromVram(tex_id);
      in_vram = false;
    }

    surface = surf;

    if (loadSurfacetoVram(surface, tex_id, gl_tex_size))
    {
      in_vram = true;
      sx = (float)surface.w/gl_tex_size.w;
      sy = (float)surface.h/gl_tex_size.h;
    }
  }

  void setZoom(float n) { zoom = n; }

  GLuint tex_id;
private:
  Surface surface;
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
  Sprite(Surface s) : Raster()
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
  SpritePack(const std::list<Surface> & s);

  Sprite ** sprites;
  size_t sprite_count;
  std::string origin;
};

class Texture
{
public:
  float dx, dy, w, h; //coords on global texture
  GLuint tex_id; //Texture ID in opengl
};

class TextureLibrary
{
public:
  // Works best if textures are of similar sizes
  TextureLibrary(const std::list<SDL_Surface*> & pck)
  {
    GLint max_size;
    glGetIntegerv(GL_MAX_TEXTURE_SIZE, &max_size);
    int row_height=0, row_width=0;

	for(std::list<SDL_Surface*>::const_iterator it = pck.begin(); it != pck.end(); ++it)
	{
      row_height = std::max(row_height, (*it)->h);
      row_width = std::max(row_width, (*it)->w);
    }

  }


  Texture * getTexture(size_t no)
  {
      return NULL;
  }

  Texture ** textures;
  size_t count_per_pack;
  size_t count;
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
