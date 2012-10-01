#pragma once

namespace Math
{
    template <typename T>
    T Clamp(const T& val, const T& min, const T& max)
    {
        return val < min ? min : (val > max ? max : val);
    };
};