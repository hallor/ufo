#include "Level.h"
#include "LevelTile.h"
#include "File.h"
#include "logger.h"

#include <Windows.h>
#include <iostream>

Level::Level()
: m_Tiles(NULL)
{
};

bool Level::Load(const std::string &file)
{
    Unload();
    //TODO make it work perhaps?
    cFile f;

    if(!f.Open(file, FFileOpenFlags::OpenExisting | FFileOpenFlags::Read))
        return false;
        
    int size = f.GetSize();
    
    m_Dimensions = vec3(MAP_DIM_X, MAP_DIM_Y, MAP_DIM_Z);
    m_Tiles = new LevelTile[size];
    for(int i = 0; i < size; ++i)
        m_Tiles[i].OnCreate();

    for(int i = 0; i < size; ++i)
    {
        unsigned char tid = 0;
        if(!f.Read(&tid, sizeof(tid)))
            return false;

        m_Tiles[i].SetId(tid);
    }
    
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
    unsigned int ts = GetTickCount();

    if(m_Tiles)
    {
        for(int i = 0; i < 200000; ++i)
            m_Tiles[i].OnDestroy();

        delete [] m_Tiles;
    }
        
        
    unsigned int te = GetTickCount();

    std::cout<<(te - ts) / 1000<<std::endl;

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