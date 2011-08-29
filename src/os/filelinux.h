#ifndef FILELINUX_H
#define FILELINUX_H

#ifdef __linux__
#include "FileIO.h"

class FileLinux : public iFile
{
public:
    FileLinux();
    ~FileLinux();
    // TODO: support for rest of the flags
    virtual bool Open(const std::string &file, DWORD flags);
    virtual int Write(void *data, DWORD length);
    virtual int Read(void *dest, int length);
    virtual bool Seek(int offset, EFileSeekMethod::TYPE t);
    virtual DWORD GetSize() const;
    virtual bool IsOpen() const;
    virtual bool IsOpenForWrite() const;
    virtual bool IsOpenForRead() const;
    virtual void Close();
    virtual int GetIndex() const;
private:
    FileLinux(const FileLinux & other);
    int fd;
    int flags;
};

#endif

#endif // FILELINUX_H
