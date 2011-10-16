#pragma once

template <class T>
class cFixedArray 
{
public:
    cFixedArray(unsigned int size)
    {
        m_Array = new T[size];
        m_Size = size;
    }
    ~cFixedArray();

    int GetSize() const { return m_Size; }

    T* GetDataPointer() const { return m_Array; }

    const T& operator [] (int index) const { return m_Array[index]; }
    T& operator [] (int index) { return m_Array[index]; }

protected:

    int m_Size;
    T *m_Array;
};