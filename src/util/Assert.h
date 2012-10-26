#pragma once
#include <crtdbg.h>

#define CHECK(cond) _ASSERT(cond)
#define CHECKI(cond, str) _ASSERT((str, cond))