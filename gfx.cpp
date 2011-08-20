#include <SDL/SDL_image.h>
#include <boost/lexical_cast.hpp>
#include <iostream>

#include "gfx.h"

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

    surf = IMG_Load( (filename + boost::lexical_cast<std::string>(i)).c_str() );
    if (!surf)
    {
      std::cout << "Failed loading " << filename + boost::lexical_cast<std::string>(i) << std::endl;
      break;
    }
    SDL_SetColorKey(surf, SDL_SRCCOLORKEY | SDL_RLEACCEL, surf->format->colorkey);
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

  SDL_Surface * surf = IMG_Load(filename.c_str());
  if (surf)
  {
    if (colorKey)
      SDL_SetColorKey(surf, SDL_SRCCOLORKEY | SDL_RLEACCEL, surf->format->colorkey);
    else
      SDL_SetColorKey(surf, 0, 0);
    Raster * r = new Raster();
    if (r)
      r->setSurface(surf);
    std::cout << "Loaded raster from " << filename << " Size: " << surf->w << "x" << surf->h << std::endl;
    return r;
  }
  return NULL;
}
