#pragma once
#include "SuperClass.h"

#define DECLARE_GAME_OBJECT(type, parent) public: \
DECLARE_SUPER(parent); \
static type* CreateObject() { return new (std::nothrow) type; } \
virtual const char* GetObjectClassName() { return #type; }

#define DECLARE_GAME_OBJECT_ABSTRACT(type, parent) public: \
DECLARE_SUPER(parent); \
virtual const char* GetObjectClassName() { return #type; }
