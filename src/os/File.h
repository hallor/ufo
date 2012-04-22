#pragma once
#ifdef _WIN32

#include "FileWin32.h"

class cFile : public cFileWin32
{
};

#else

#include "filelinux.h"

class cFile : public FileLinux
{
};

#endif