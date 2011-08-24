#include <SDL/SDL_image.h>
#include <boost/lexical_cast.hpp>
#include <iostream>
#include <cmath>
#include "gfx.h"
using namespace std;

SpritePack::SpritePack(const std::list<SDL_Surface*> & s)
{
  if (s.size())
  {
    int i=0;
    sprite_count = s.size();
    sprites = new Sprite *[sprite_count];
    for (std::list<SDL_Surface*>::const_iterator it = s.begin(); it!=s.end(); ++it)
    {
      if (*it)
      {
        sprites[i] = new Sprite(*it);
        i++;
      }
    }
  }
}

SpritePack::~SpritePack()
{
  for (size_t i=0; i< sprite_count; ++i)
    delete sprites[i];
  delete [] sprites;
}

Sprite * SpritePack::getSprite(size_t no)
{
  if (sprites && no < sprite_count)
    return sprites[no];
  if (sprites)
    return sprites[0];
  return NULL;
}

GfxManager::~GfxManager()
{
  GfxMap::iterator it;
  for (it=resources.begin(); it!=resources.end(); ++it)
    delete (*it).second;
  resources.clear();

  RasterMap::iterator it2;
  for (it2=rasters.begin(); it2!=rasters.end(); ++it2)
    delete (*it2).second;
  rasters.clear();
}

SpritePack * GfxManager::getPack(const std::string & filename)
{
  if (resources.count(filename))
    return resources[filename];

  std::list<SDL_Surface*> co;

  std::cout << "Loading sprite pack from file(s) " << filename << std::endl;

  for (int i=0; i<999; ++i)
  {
    SDL_Surface * surf;

    surf = loadTexture( (filename + boost::lexical_cast<std::string>(i)).c_str(), true);
    if (!surf)
    {
      std::cout << "Failed loading " << filename + boost::lexical_cast<std::string>(i) << std::endl;
      break;
    }
    co.push_back(surf);
  }

  resources[filename] = new SpritePack(co);

  std::cout << "Loaded " << resources[filename]->count() << " sprites." << std::endl;

  return resources[filename];
}

Raster * GfxManager::getRaster(const std::string & filename, bool colorKey)
{
  if (rasters.count(filename))
    return rasters[filename];

  SDL_Surface * surf = loadTexture(filename.c_str(), colorKey);
  if (surf)
  {
    Raster * r = new Raster();
    if (r)
    {
      r->setSurface(surf);
      std::cout << "Loaded raster from " << filename << " Original Size: " << surf->w << "x" << surf->h << std::endl;
    }
    return r;
  }
  return NULL;
}

bool unloadSurfacefromVram(GLuint & tex_id)
{
  glDeleteTextures(1, &tex_id);
  return true;
}

bool loadSurfacetoVram(SDL_Surface * surface, GLuint & tex_id, Rect & tex_rect)
{
  GLenum texture_format = 0;
  GLint  nOfColors;

  // Convert to HW format (32bit etc)
  surface = SDL_DisplayFormat(surface);

  int neww = surface->w;
  int newh = surface->h;

  // Check that the image's width is a power of 2
  if ( (surface->w & (surface->w - 1)) != 0 ) {
    neww = 1 << (int)ceil(log2(surface->w));
  }

  // Also check if the height is a power of 2
  if ( (surface->h & (surface->h - 1)) != 0 ) {
    newh = 1 << (int)ceil(log2(surface->h));
  }

  tex_rect.w = neww;
  tex_rect.h = newh;

  /* Resize if needed */
  if (neww !=surface->w || newh !=surface->h)
  {
    Uint32 rmask, gmask, bmask, amask;
#if SDL_BYTEORDER == SDL_BIG_ENDIAN
    rmask = 0xff000000;
    gmask = 0x00ff0000;
    bmask = 0x0000ff00;
    amask = 0x000000ff;
#else
    rmask = 0x000000ff;
    gmask = 0x0000ff00;
    bmask = 0x00ff0000;
    amask = 0xff000000;
#endif
    SDL_Surface * n = SDL_CreateRGBSurface(SDL_SWSURFACE, neww, newh, 32,rmask,gmask,bmask,amask);
    if (!n)
    {
      SDL_FreeSurface(surface);
      return false;
    }
    // Clear
    //SDL_FillRect(n, NULL, SDL_MapRGBA(n->format,0,0,100,0));
    // Copy
    SDL_BlitSurface(surface, NULL, n, NULL);
    SDL_FreeSurface(surface);
    surface = n;
  }

  // get the number of channels in the SDL surface
  nOfColors = surface->format->BytesPerPixel;
  if (nOfColors == 4)     // contains an alpha channel
  {
    if (surface->format->Rmask == 0x000000ff)
      texture_format = GL_RGBA;
    else
      texture_format = GL_BGRA;
  } else if (nOfColors == 3)     // no alpha channel
  {
    if (surface->format->Rmask == 0x000000ff)
      texture_format = GL_RGB;
    else
      texture_format = GL_BGR;
  } else {
    cout << "warning: the image is not truecolor..  this will probably break\n" << endl;
    // this error should not go unhandled
  }

  // Have OpenGL generate a texture object handle for us
  glGenTextures( 1, &tex_id );
  if (glGetError())
    cout << "GL error" << glGetError() << endl;

  // Bind the texture object
  glActiveTexture(GL_TEXTURE0);
  glBindTexture( GL_TEXTURE_2D, tex_id );

  // Set the texture's stretching properties
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST );
  glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST );

  // Edit the texture object's image data using the information SDL_Surface gives us
  glTexImage2D( GL_TEXTURE_2D, 0, nOfColors, surface->w, surface->h, 0,
               texture_format, GL_UNSIGNED_BYTE, surface->pixels );


  SDL_FreeSurface(surface);
  return true;
}


SDL_Surface * loadTexture(const char * filename, bool colorKey)
{
  SDL_Surface *surface;	// This surface will tell us the details of the image

  surface = IMG_Load(filename);
  if (!surface)
    return NULL;

  if (colorKey)
    SDL_SetColorKey(surface, SDL_SRCCOLORKEY | SDL_RLEACCEL, 0);

  return surface;
}
