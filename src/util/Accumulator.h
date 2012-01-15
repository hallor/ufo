#pragma once

template <class T>
class Accumulator
{
public:
    Accumulator()
    {
        m_Current = 0;
        m_ToAccumulate = 0;
    }

    Accumulator(const T& to_accumulate)
    {
        m_Current = 0;
        m_ToAccumulate = to_accumulate;
    }

    void Reset() { m_Current = 0; }

    const T &GetAccumulatedValue() const { return m_Current; }
    
    void Accumulate(const T& delta) { m_Current += delta; }

    bool Check()
    {
        bool overflow = m_Current >= m_ToAccumulate;
        if(overflow)
            m_Current -= m_ToAccumulate;

        return overflow;
    }

private:

    T m_Current;
    T m_ToAccumulate;
};