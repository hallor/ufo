#pragma once
#include <string>
#include "vec.h"
#include "FixedArray.h"
#include "TextureCache.h"

class LevelTile;
class cTexture;

class Level
{
public:
    Level();

    bool Load(const std::string &file);
    
    void Unload();

    void Update(float dt);

    cTexture GetTexture(int id) const;

    LevelTile *GetTileAt(const vec3 &pos) const;

    bool IsLoaded() const { return m_Loaded; }

protected:

    bool CreateTiles(unsigned int count);
    void DestroyTiles();

    void CacheTextures();

    static const vec3 MAP_DIM;

    FixedArray<LevelTile> *m_Tiles;
    cTextureCache m_TextureCache;

    std::string m_TextureSetPrefix;
    bool m_Loaded;
};