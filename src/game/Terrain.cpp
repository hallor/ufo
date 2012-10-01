#include "game_pch.h"
#include "Terrain.h"
#include "game/Game.h"

const STerrainElement STerrainElement::s_DummyElement = STerrainElement();

Terrain::Terrain()
: m_Tiles(nullptr)
, m_Loaded(false)
{
    m_Dimensions = vec3(100.0f, 10.0f, 100.0f);
}

Terrain::~Terrain()
{
    Unload();
}

bool Terrain::Load(const std::string& file)
{
    Unload();

    cFile f;
    if(!f.Open(file, FFileOpenFlags::Read | FFileOpenFlags::OpenExisting))
        return false;

    int cells_count = (int)m_Dimensions.Volume();

    if(f.GetSize() < cells_count * sizeof(unsigned short))
        return false;

    if(!CreateTiles(cells_count))
        return false;

    FixedArray<unsigned short> ids(cells_count);

    if(!f.Read(ids.GetDataPointer(), ids.GetDataSize()))
        return false;

    m_TextureSetPrefix = "city";
    m_TextureCache.SetTextureManager(Game::GetSingleton()->GetTextureManager());
    CacheTextures();

    vec3 pos = vec3::ZERO;
    int mx = m_Dimensions.x;
    int mz = m_Dimensions.z;


    for(int i = 0; i < ids.GetSize() && i < (*m_Tiles).GetSize(); ++i)
    {
        pos.y = i / (mx * mz);
        pos.z = i % mz;
        pos.x = (i % (mx * mz) - i % mz) / mz;

        (*m_Tiles)[i].m_Id = ids[i];
        (*m_Tiles)[i].m_Pos = pos;
    }

    m_Loaded = true;

    return true;
}

void Terrain::Unload()
{
    m_Loaded = false;
    DestroyTiles();
    m_TextureSetPrefix = "";
    m_TextureCache.ClearCache();
}

void Terrain::Update(float dt)
{
    // Only touched (damaged etc.) elements should be updated here
}

const STerrainElement& Terrain::GetTileAt(const vec3 &pos) const
{
    if(!IsLoaded())
        return STerrainElement::s_DummyElement;

    if(pos.x < 0.0f || pos.y < 0.0f || pos.z < 0.0f)
        return STerrainElement::s_DummyElement;

    if(pos.x >= m_Dimensions.x || pos.y >= m_Dimensions.y || pos.z >= m_Dimensions.z)
        return STerrainElement::s_DummyElement;

    return (*m_Tiles)[pos.x * m_Dimensions.z + pos.y * m_Dimensions.x * m_Dimensions.z + pos.z];
}

bool Terrain::CreateTiles(unsigned int count)
{
    if(m_Tiles)
        return false;

    m_Tiles = new (std::nothrow) FixedArray<STerrainElement>(count);
    
    return m_Tiles != nullptr;
}

void Terrain::DestroyTiles()
{
    if(!m_Tiles)
        return;
    
    delete m_Tiles;
    m_Tiles = nullptr;
}

void Terrain::CacheTextures()
{
    std::string name = "resources/ufodata/";
    name += m_TextureSetPrefix;
    name += "/";
    name += m_TextureSetPrefix;

    m_TextureCache.CreateCache(name, 1000);
}
