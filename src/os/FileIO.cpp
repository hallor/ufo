#include "FileIO.h"
#include "logger.h"

#ifdef _WIN32
#include "FileWin32.h"
#endif

int FilesCount = 0;

iFile *CreateFileIO()
{
	LogDebug("Creating file %i", FilesCount);
	iFile *ret = NULL;
#ifdef _WIN32
	ret = new (std::nothrow) cFileWin32(FilesCount);
#else
	ret = NULL;
#endif
	++FilesCount;

	return ret;
};

void ReleaseFileIO(iFile*& f)
{
	if(!f)
		return;

	LogDebug("Releasing file %i", f->GetIndex());
	delete f;
	f = NULL;
};