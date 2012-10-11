#include "game_pch.h"
#include "GameObject.h"

iGameObject::iGameObject()
{
    m_IsDestroying = false;
}

iGameObject::~iGameObject()
{
}

void iGameObject::Delete()
{
    if (m_IsDestroying)
        return;

    g_GOFactory.DestroyObject(this);
    m_IsDestroying = true;
}

void iGameObject::RegisterHandler(EEngineHandler::TYPE handler)
{
    g_GOFactory.RegisterHandler(this, handler);
}

void iGameObject::UnregisterHandler(EEngineHandler::TYPE handler)
{
    g_GOFactory.UnregisterHandler(this, handler);
}
