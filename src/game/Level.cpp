#include "Level.h"
#include "Game.h"
#include "TextureManager.h"
#include "LevelTile.h"
#include "File.h"

const vec3 Level::MAP_DIM = vec3(100, 10, 100);

Level::Level()
: m_Tiles(nullptr)
, m_Loaded(false)
{
};

bool Level::Load(const std::string &file)
{
    Unload();

    cFile f;
    if(!f.Open(file, FFileOpenFlags::Read | FFileOpenFlags::OpenExisting))
        return false;

    if(f.GetSize() < MAP_DIM.Volume() * sizeof(unsigned short))
        return false;

    if(!CreateTiles((int)MAP_DIM.Volume()))
        return false;

    FixedArray<unsigned short> ids((int)MAP_DIM.Volume());

    if(!f.Read(ids.GetDataPointer(), ids.GetDataSize()))
        return false;

    m_TextureSetPrefix = "city";
    m_TextureCache.SetTextureManager(Game::GetSingleton()->GetTextureManager());
    CacheTextures();

    int layer = 0;

    for(int i = 0; i < ids.GetSize() && i < (*m_Tiles).GetSize(); ++i)
    {
        layer = i / ((int)MAP_DIM.x * (int)MAP_DIM.z);
        int px = i - layer * (int)(MAP_DIM.x * MAP_DIM.z);
        (*m_Tiles)[i].SetId(ids[i]);
        (*m_Tiles)[i].SetPos(vec3(MAP_DIM.x - (px / ((int)MAP_DIM.x)), layer, px % ((int)MAP_DIM.z)));
    }

    m_Loaded = true;

    return true;
};

void Level::Unload()
{
    m_Loaded = false;
    DestroyTiles();
    m_TextureSetPrefix = "";
};

void Level::Update(float dt)
{
    if(!IsLoaded())
        return;

    for(int i = 0; i < (*m_Tiles).GetSize(); ++i)
        (*m_Tiles)[i].Update(dt);
}

cTexture Level::GetTexture(int id) const
{
    if(id >= m_TextureCache.GetCacheSize())
        return cTexture(NULL);

    return m_TextureCache.GetCachedTexture(id);
}

LevelTile *Level::GetTileAt(const vec3 &pos) const
{
    if(!IsLoaded())
        return NULL;

    if(pos.x < 0.0f || pos.y < 0.0f || pos.z < 0.0f)
        return NULL;

    if(pos.x >= MAP_DIM.x || pos.y >= MAP_DIM.y || pos.z >= MAP_DIM.z)
        return NULL;

    return &(*m_Tiles)[pos.z + pos.y * MAP_DIM.x * MAP_DIM.z + pos.x * MAP_DIM.x];
}

bool Level::CreateTiles(unsigned int count)
{
    if(m_Tiles)
        return false;

    m_Tiles = new FixedArray<LevelTile>(count);

    for(int i = 0; i < m_Tiles->GetSize(); ++i)
        (*m_Tiles)[i].OnCreate(this);

    return true;
}

void Level::DestroyTiles()
{
    if(!m_Tiles)
        return;

    for(int i = 0; i < m_Tiles->GetSize(); ++i)
        (*m_Tiles)[i].OnDestroy();

    delete m_Tiles;
    m_Tiles = nullptr;
}

void Level::CacheTextures()
{
    std::string name = "resources\\ufodata\\";
    name += m_TextureSetPrefix;
    name += "\\";
    name += m_TextureSetPrefix;
    
    m_TextureCache.CreateCache(name, 1000);
}