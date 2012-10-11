#pragma once

#include <vector>
#include "GameObject.h"
#include "HandlerTypes.h"

class cGameFactory;
extern cGameFactory g_GOFactory;

class cGameFactory
{
public:
    cGameFactory();
    ~cGameFactory();
    // Calls handler for proper objects
    void CallHandler(EEngineHandler::TYPE handler);

    template <class T>
    T* CreateObject()
    {
        iGameObject* object = T::CreateObject();

        if (object)
        {        
            object->OnCreate();
            m_CreatedObjects.push_back(object);
        }

        return (T*)object;
    };

    void DestroyObject(iGameObject* obj);

    void RegisterHandler(iGameObject* obj, EEngineHandler::TYPE handler);
    void UnregisterHandler(iGameObject* obj, EEngineHandler::TYPE handler);

    // Frees memory used by objects in destroyed state
    void CleanupObjects();

protected:
    typedef std::vector<iGameObject*> TObjectsList;

    void CallOnLogicUpdate(TObjectsList& objects);
    void CallOnRenderFrame(TObjectsList& objects);
    void CallOnCreate();
    void CallOnDestroy();

    void CheckNonReleasedObjects() const;
    int CheckNonReleasedObjects(const TObjectsList& list) const; 
    
    bool RemoveObjectFromList(TObjectsList& list, iGameObject* obj);
    bool FindObjectOnList(const TObjectsList& list, iGameObject* obj) const;

    TObjectsList m_SheduledOnDestroyObjects;
    TObjectsList m_ObjectsToDelete;

    // All fully constructed and working objects
    TObjectsList m_CreatedObjects;

    TObjectsList m_RegisteredHandlers[EEngineHandler::_COUNT];
};