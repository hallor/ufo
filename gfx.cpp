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
      break;
    SDL_SetColorKey(surf, SDL_SRCCOLORKEY, 0);
    co.push_back(surf);
  }
  resources[filename] = new SpritePack(co);

  std::cout << "Loaded " << resources[filename]->count() << " sprites." << std::endl;

  return resources[filename];
}
