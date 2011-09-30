#pragma once

#ifndef PROPERTY
#define PROPERTY(type, name, default_value) \
    public: \
    const type &Get##name() const { return m_##name; } \
    static type GetDefault##name() { return default_value; } \
    void Set##name(const type &new_val) \
    { m_##name = new_val; } \
    void Default##name() { Set##name(GetDefault##name()); } \
    protected: \
    type m_##name;
#endif

#ifndef STATIC_PROPERTY
#define STATIC_PROPERTY(type, name, default_value) \
    public: \
    static type Get##name() { return Get().m_##name; } \
    static type GetDefault##name() { return default_value; } \
    static void Set##name(const type &new_val) \
    { Get().m_##name = new_val; } \
    static void Default##name() { Get().Set##name(GetDefault##name()); } \
    protected: \
    type m_##name;
#endif