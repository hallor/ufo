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
		type m_##name
#endif

#ifndef PTR_PROPERTY
#define PTR_PROPERTY(type, name, default_value) \
    public: \
    type *Get##name() const { return m_##name; } \
    static type *GetDefault##name() { return default_value; } \
    void Set##name(type *new_val) \
    { m_##name = new_val; } \
    void Default##name() { Set##name(GetDefault##name()); } \
    protected: \
		type *m_##name
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
		type m_##name
#endif

#ifndef MEMBER
#define MEMBER(type, name) \
		public: \
		const type & Get##name() const	{ return m_##name; } \
		void Set##name(const type &new_val) { m_##name = new_val; } \
		protected: \
		type m_##name
#endif


// member returned by value
#ifndef MEMBERV
#define MEMBERV(type, name) \
    public: \
    const type get##name() const	{ return m_##name; } \
    void set##name(type new_val) { m_##name = new_val; } \
    protected: \
    type m_##name
#endif

#ifndef CONST_MEMBER
#define CONST_MEMBER(type, name) \
		public: \
		const type & get##name() const	{ return m_##name; } \
		void set##name(const type &new_val) { m_##name = new_val; } \
		protected: \
		const type m_##name
#endif
