#pragma once
#include "FileWin32.h"
#include "filelinux.h"

#ifdef _WIN32
class cFile : public cFileWin32
{
};
#else
class cFile : public FileLinux
{
};
#endif