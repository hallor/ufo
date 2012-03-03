#include "FileIO.h"
#include "logger.h"

#ifdef _WIN32
#include "FileWin32.h"
#else
#include "filelinux.h"
#endif

iFile *CreateFileIO()
{
    iFile *ret = NULL;

#ifdef _WIN32
	ret = new (std::nothrow) cFileWin32();
#else
    ret = new (std::nothrow) FileLinux();
#endif

	return ret;
}

void ReleaseFileIO(iFile*& f)
{
	if(!f)
		return;
	delete f;
	f = NULL;
}
