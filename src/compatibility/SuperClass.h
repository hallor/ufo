#pragma once

#ifndef _WIN32
#define DECLARE_SUPER(p) typedef p __super;
#else
#define DECLARE_SUPER(p)
#endif