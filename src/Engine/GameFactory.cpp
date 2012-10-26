#include "game_pch.h"
#include "GameFactory.h"

cGameFactory::cGameFactory()
{

}

cGameFactory::~cGameFactory()
{
    CheckNonReleasedObjects();
    CleanupObjects();
}

cGameFactory g_GOFactory;

void cGameFactory::CallHandler(EEngineHandler::TYPE handler)
{
    if (EEngineHandler::IsValid(handler))
    {
        TObjectsList& objects = m_RegisteredHandlers[handler];

        switch(handler)
        {
        case EEngineHandler::HandlerOnLogicUpdate:
            CallOnLogicUpdate(objects);
            break;
        case EEngineHandler::HandlerOnRenderFrame:
            CallOnRenderFrame(objects);
            break;
            
        default:
            CHECKI(false, "FATAL: INVALID HANDLER PASSED TO [cGameFactory::CallHandler]");
        }
    }
    else
        CHECKI(false, "FATAL: INVALID HANDLER PASSED TO [cGameFactory::CallHandler]");
}

void cGameFactory::DestroyObject(iGameObject* obj)
{
    CHECKI(obj && !obj->IsDestroying(), "");

    if (!obj || obj->IsDestroying())
        return;

    bool created = RemoveObjectFromList(m_CreatedObjects, obj);
    CHECKI(created, ("Object of class %s was not created properly!", obj->GetObjectClassName()));

    for (int i = 0; i < EEngineHandler::_COUNT; ++i)
        RemoveObjectFromList(m_RegisteredHandlers[i], obj);

    obj->OnDestroy();
    m_ObjectsToDelete.push_back(obj);
}

void cGameFactory::RegisterHandler(iGameObject* obj, EEngineHandler::TYPE handler)
{
    CHECKI(obj, "Trying to register invalid handler!");

    if (!obj)
        return;

    TObjectsList& list = m_RegisteredHandlers[handler];

    if (!FindObjectOnList(list, obj))
        list.push_back(obj);
}

void cGameFactory::UnregisterHandler(iGameObject* obj, EEngineHandler::TYPE handler)
{
    CHECKI(obj, "");
    
    TObjectsList& list = m_RegisteredHandlers[handler];

    RemoveObjectFromList(list, obj);
}

void cGameFactory::CleanupObjects()
{
    for (UINT i = 0; i < m_ObjectsToDelete.size(); ++i)
        delete m_ObjectsToDelete[i];

    m_ObjectsToDelete.clear();
}

void cGameFactory::CallOnLogicUpdate(TObjectsList& objects)
{
    for (UINT i = 0; i < objects.size(); ++i)
        objects[i]->OnLogicUpdate();
};

void cGameFactory::CallOnRenderFrame(TObjectsList& objects)
{
    for (UINT i = 0; i < objects.size(); ++i)
        objects[i]->OnRenderFrame();
};

void cGameFactory::CheckNonReleasedObjects() const
{
    int nonreleased = 0;

    nonreleased += CheckNonReleasedObjects(m_CreatedObjects);

    if (nonreleased > 0)
        LogError("%i objects were not released properly!", nonreleased);
    CHECKI(nonreleased == 0, "[cGameFactory::CheckNonReleasedObjects] Some objects were not released properly!");
};

int cGameFactory::CheckNonReleasedObjects(const TObjectsList& list) const
{
    for (UINT i = 0; i < list.size(); ++i)
        LogError("Object of class %s was not destroyed properly!", list[i]->GetObjectClassName());
    
    return (int)list.size();
};

bool cGameFactory::RemoveObjectFromList(TObjectsList& list, iGameObject* obj)
{   
    if (!obj)
        return false;

    for (UINT i = 0; i < list.size(); ++i)
    {
        if (list[i] == obj)
        {
            list[i] = list.back();
            list.pop_back();
            return true;
        }
    }

    return false;
};

bool cGameFactory::FindObjectOnList(const TObjectsList& list, iGameObject* obj) const
{   
    if (!obj)
        return false;

    for (UINT i = 0; i < list.size(); ++i)
    {
        if (list[i] == obj)
            return true;
    }

    return false;
};