#include "FileIO.h"
#include "logger.h"

#ifdef _WIN32
#include "FileWin32.h"
#else
#include "filelinux.h"
#endif

int FilesCount = 0;

iFile *CreateFileIO()
{
    iFile *ret = NULL;
	LogDebug("Creating file %i", FilesCount);
#ifdef _WIN32
	ret = new (std::nothrow) cFileWin32(FilesCount);
#else
    ret = new (std::nothrow) FileLinux();
#endif
	++FilesCount;

	return ret;
};

void ReleaseFileIO(iFile*& f)
{
	if(!f)
		return;
#ifdef _WIN32
    LogDebug("Releasing file %i", (cFileWin32*)f->GetIndex());
#endif
	delete f;
	f = NULL;
};
