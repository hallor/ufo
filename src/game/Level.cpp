#include "Level.h"
#include "LevelTile.h"

Level::Level()
: m_Tiles(NULL)
{
};

bool Level::Load(const std::string &file)
{
    //TODO make it work perhaps?

    m_Dimensions = vec3(MAP_DIM_X, MAP_DIM_Y, MAP_DIM_Z);
    m_Tiles = new LevelTile[(int)m_Dimensions.Volume()];
    for(int i = 0; i < (int)m_Dimensions.Volume(); ++i)
        m_Tiles[i].OnCreate();

    return true;
};

const LevelTile* Level::GetTile(const vec3 &pos) const
{
    if(!CheckTilePositionBounds(pos))
        return NULL;

    int x = pos.x;
    int y = pos.y;
    int z = pos.z;

    return &m_Tiles[IndexFromPos(pos)];
};

void Level::Unload()
{
    if(m_Tiles)
    {
        for(int i = 0; i < (int)m_Dimensions.Volume(); ++i)
            m_Tiles[i].OnDestroy();

        delete [] m_Tiles;
    }
        
    m_Tiles = NULL;
    m_Dimensions = vec3::ZERO;
};

bool Level::CheckTilePositionBounds(const vec3 &pos) const
{
    return pos.x >= 0.0f && pos.y >= 0.0f && pos.z >= 0.0f &&
           pos.x < m_Dimensions.x && pos.y < m_Dimensions.y && pos.z < m_Dimensions.z;
};

int Level::IndexFromPos(const vec3 &pos) const
{
    return (int)(pos.x + pos.y * m_Dimensions.x * m_Dimensions.z + pos.z * m_Dimensions.x);
}