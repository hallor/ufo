#include "FileEnums.h"


std::string FFileOpenFlags::ToString(DWORD t)
{
	std::string out;

#define PARTIAL_FLAG(p, q) p & q ? std::string(#q) + ":" : ""

	out += PARTIAL_FLAG(t, OpenExisting);
	out += PARTIAL_FLAG(t, OpenAlways);
	out += PARTIAL_FLAG(t, CreateNew);
	out += PARTIAL_FLAG(t, Write);
	out += PARTIAL_FLAG(t, Read);

#undef PARTIAL_FLAG

	if(out.empty())
		out = "NoFlags";

	return out;
};

bool FFileOpenFlags::Valid(DWORD t)
{
	if(!(t & Write) && ! (t& Read))
		return false;

	DWORD prim = t & _MASK_PRIMARY;
	if(!prim || prim == _MASK_PRIMARY || !((prim & OpenExisting) ^ (prim & OpenAlways) ^ (prim & CreateNew)))
		return false;

	return true;
};

DWORD FFileOpenFlags::ToDesiredAccess(DWORD t)
{
	DWORD out = 0;
	out |= (t & Write) ? GENERIC_WRITE : 0;
	out |= (t & Read) ? GENERIC_READ : 0;
	
	return out;
};

DWORD FFileOpenFlags::ToDisposition(DWORD t)
{
	DWORD out = 0;
	if(t & OpenAlways)
	{
		out = OPEN_ALWAYS;
	}
	else
	if(t & OpenExisting)
	{
		out = OPEN_EXISTING;
	}
	else
	if(t & CreateNew)
	{
		out = CREATE_ALWAYS;
	}

	return out;
};

DWORD EFileSeekMethod::ToMoveMethod(EFileSeekMethod::TYPE t)
{
	switch(t)
	{
	case FromBegin:		return FILE_BEGIN;
	case FromEnd:		return FILE_END;
	case Current:		return FILE_CURRENT;
	}

	return (DWORD)(-1);
}