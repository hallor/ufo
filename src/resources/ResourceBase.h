#pragma once
#include <string>
#include "ResourceEnums.h"
#include "ManagerBase.h"

template <class T>
class vResource
{
public:
    vResource()
    {
        m_Resource = NULL;
        m_StorageIndex = -1;
        m_Type = EResourceType::None;
        m_ResourceID = "UNKNOWN RESOURCE";
        m_State = EResourceState::NotLoaded;
        m_Parent = NULL;
    }

    virtual ~vResource(){};

    T Get() const { return m_Resource; }

    std::string          GetID()    const { return m_ResourceID; }
    EResourceState::TYPE GetState() const { return m_State; }
    EResourceType::TYPE  GetType()  const { return m_Type; }

    iManagerBase<T>*     GetParent()       const { return m_Parent; }
    unsigned int         GetStorageIndex() const { return m_StorageIndex; }

    static vResource<T>* Create() { return new (std::nothrow) vResource<T>; };

    friend class iManagerBase<T>;

protected:

    virtual void SetParent(iManagerBase<T> * parent)
    {
        if(GetParent() == parent)
            return;

        if(GetParent())
            GetParent()->NotifyResourceParentChanged(this);

        m_Parent = parent;
    }

    virtual void SetID(const std::string &id) { m_ResourceID = id; };
    virtual void SetStorageIndex(unsigned int index) { m_StorageIndex = index; };

    T m_Resource;
    EResourceState::TYPE m_State;
    EResourceType::TYPE m_Type;

    unsigned int m_StorageIndex;
    iManagerBase<T> *m_Parent;

    std::string m_ResourceID;
};
