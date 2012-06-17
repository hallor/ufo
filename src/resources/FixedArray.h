#pragma once

template <class T>
class FixedArray 
{
public:
    FixedArray(unsigned int size)
    {
        m_Array = new T[size];
        m_Size = size;
    }
    ~FixedArray()
    {
        if(m_Array)
            delete [] m_Array;
    };

    int GetSize() const { return m_Size; }
    int GetDataSize() const { return m_Size * sizeof(T); }

    T* GetDataPointer() const { return m_Array; }

    const T& operator [] (int index) const { return m_Array[index]; }
    T& operator [] (int index) { return m_Array[index]; }

protected:

    int m_Size;
    T *m_Array;
};