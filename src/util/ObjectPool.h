#pragma once
#include <list>
#include <vector>
#include <algorithm>

template <class T>
class ObjectPool
{
public:
    void Add(const T &object, bool free = true)
    {
        if(std::find(m_All.begin(), m_All.end(), object) == m_All.end())
        {
            m_All.push_back(object);
            (free ? m_Free : m_Used).push_back(object); 
        }
    }

    void Remove(const T &object)
    {
        m_All.remove(object);
        // TODO optimize
        m_Free.erase(object);
        m_Used.erase(object);
    }

    void Clear()
    {
        m_All.clear();
        m_Free.clear();
        m_Used.clear();
    }

    void FreeAll()
    {
        for(unsigned int i = 0; i < m_Used.size(); ++i)
            m_Free.push_back(m_Used[i]);
        
        m_Used.clear();
    }

    bool HasFree() const { return FreeCount() > 0; }

    int Size() const { return m_All.size(); }

    int UsedCount() const { return m_Used.size(); }

    int FreeCount() const { return m_Free.size(); }

    bool Get(T &out)
    {
        if(HasFree())
        {
            T tmp = m_Free.back();
            m_Free.pop_back();
            m_Used.push_back(tmp);
            out = tmp;
            return true;
        }

        return false;
    }

	 void Return(const T &object)
    {
       typename std::vector<T>::iterator it = std::find(m_Used.begin(), m_Used.end(), object);
       if(it != m_Used.end())
       {
           m_Used.erase(it);
           m_Free.push_back(object);
       }
   }

protected:

    std::vector<T> m_Free;
    std::vector<T> m_Used;
    std::list<T> m_All;
};
