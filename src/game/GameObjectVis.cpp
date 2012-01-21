#include <cstdlib>
#include "GameObjectVis.h"
#include "GameObject.h"

iGameObjectVis::iGameObjectVis(iGameObject *parent)
{
    m_Parent = parent;
    m_Texture = NULL;
}

iGameObjectVis::~iGameObjectVis()
{
}