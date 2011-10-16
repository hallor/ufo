#pragma once

#include "FileEnums.h"
#include "FileIO.h"

#ifdef _WIN32
class cFileWin32 : public iFile
{
public:
	cFileWin32(int index);
	~cFileWin32();

    virtual bool Open(const std::string &file, DWORD flags);

    std::string GetPath() const;

	virtual int Write(void *data, DWORD length);
	virtual int Read(void *dest, int length);

	virtual bool Seek(int offset, EFileSeekMethod::TYPE t);

    virtual int GetCurrentPos() const;

	virtual DWORD GetSize() const;
	virtual DWORD GetOpenFlags() const;
	virtual bool IsOpen() const;
	virtual bool IsOpenForWrite() const;
	virtual bool IsOpenForRead() const;

	virtual void Close();

	virtual int GetIndex() const;

protected:

	void Clear();

	std::string m_Path;
	HANDLE m_Handle;
	DWORD m_Size;
	DWORD m_Flags;

	int m_Index;
};
#endif