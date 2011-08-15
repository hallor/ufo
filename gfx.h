#ifndef GFX_H
#define GFX_H

#include <string>
#include <map>
#include <list>
#include <SDL/SDL.h>

class Sprite
{
public:
  Sprite(SDL_Surface *s) : img(s) {}
  ~Sprite() { SDL_FreeSurface(img); img = NULL; }

  SDL_Surface * img;
};

class SpritePack
{
public:

  SpritePack(const std::list<SDL_Surface*> & s);

  ~SpritePack();

  Sprite * getSprite(size_t no);

  size_t count() const { return sprite_count; }

private:
  Sprite ** sprites;
  size_t sprite_count;
  std::string origin;
};

class GfxManager
{
  typedef std::map<std::string, SpritePack *> GfxMap;

public:

  ~GfxManager();

  /* TODO: rewrite */
  SpritePack * getPack(const std::string & filename);
private:
  GfxMap resources;
};

#endif
