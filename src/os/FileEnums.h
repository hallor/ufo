#pragma once

#ifdef _WIN32
#include <Windows.h>
#else
#include <stdint.h>
#define DWORD uint32_t
#endif

#include <string>

namespace FFileOpenFlags
{
	enum TYPE
	{
		// Opens file only when it exists
		OpenExisting = 1,
		// Opens file if it exists, creates it otherwise
		OpenAlways = 2,
		// Creates file when it doesnt exist, doesn't open otherwise
		CreateNew = 4,
		// Opens file for reading
		Read = 8,
		// Opens file for writing
		Write = 16,

		_MASK_PRIMARY = 0x07,
		_MASK_SECONDARY = ~_MASK_PRIMARY
	};

	// For debugging purposes
	std::string ToString(DWORD t);

	bool Valid(DWORD t);

	DWORD ToDesiredAccess(DWORD t);
	DWORD ToDisposition(DWORD t);
};

namespace EFileSeekMethod
{
	enum TYPE
	{
		FromBegin = 0,
		FromEnd,
		Current
	};

	DWORD ToMoveMethod(TYPE t);
};
