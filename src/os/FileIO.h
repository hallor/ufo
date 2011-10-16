#pragma once

/*
	Multiplatform file I/O abstraction layer
*/
#include "FileEnums.h"

/*
  TODO:
  - add a way to do copy-by-value of files (refcounts??)
  - and remove destructor function
  - Maybe create wrapper so file creation will be the same for all platforms
    without need for factory (i.e. is factory needed here?)
  - Remove os-dependent flags/dwords/etc. replace with generic enums
*/

class iFile
{
public:

    virtual ~iFile(){};

	/* Opens file $file using passed flags

		Return value:
		true	 file is ready to be written to/read from
		false	 file cannot be opened due to wrong flags, user not having high enough privilage level, or simply because it doesnt exist

		Flags usage guideline:

		Only one of the following flags may be used:
			OpenExisting
			OpenAlways
			CreateNew
		Remainig flags may be mixed together, but at least one must be present
	
		TODO:
			Change return value so that exact error type can be returned */
	virtual bool Open(const std::string &file, DWORD flags) = 0;

    /* Retrieves $file argument passed to last Open() call
        
        Return value:
        empty string    last call to Open() failed
    */
    virtual std::string GetPath() const = 0; 

	/* Writes $length bytes from $data to previously Open'ed file
	
		Return value:
		-1		file is not open for writing, or previous call to Open has failed
		>0		number of bytes written to file

		Requires file to be open with FFileOpenFlags::Write */
	virtual int Write(void *data, DWORD length) = 0;

	/* Reads $length bytes to $data from previously Open'ed file
	
		Return value:
		-1		file is not open for reading, or previous call to Open has failed
		>0		number of bytes read from file

		Requires file to be open with FFileOpenFlags::Read */
	virtual int Read(void *dest, int length) = 0;

	/* Moves file read/write cursor by $offset bytes from specified location

		Return value:
		true	cursor was succesfully moved to requested position
		false	cursor couldn't be moved because file isn't open, or range was exceeded */
	virtual bool Seek(int offset, EFileSeekMethod::TYPE t) = 0;

    virtual int GetCurrentPos() const = 0;

    virtual bool AtEnd() const = 0;

	/* Retrieves file size
		
		Return value:
		n		length of file in bytes
		0		file is either empty, or it wasn't opened exclusively with FFileOpenFlags::Read */
	virtual DWORD GetSize() const = 0;

	/* Returns true when file is open, false otherwise */
	virtual bool IsOpen() const = 0;
	virtual bool IsOpenForWrite() const = 0;
	virtual bool IsOpenForRead() const = 0;

	/* Closes file */
	virtual void Close() = 0;
};

/* Creates iFile depending on compilation platform 
	May return NULL
	doesnt throw exceptions
*/
iFile* CreateFileIO();

/* Releases file created by CreateFileIO()
	Passing NULL is legal NOP
	Will reset $f to NULL
*/
void ReleaseFileIO(iFile*& f);
