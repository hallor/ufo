#include "game_pch.h"
#ifdef __linux__
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "filelinux.h"
#include "logger.h"

using namespace std;

FileLinux::FileLinux() : fd(-1), flags(0)
{
    atEOF = true;
}

FileLinux::~FileLinux()
{
    if (fd>=0)
        close(fd);
    fd=-1;
}

bool FileLinux::Open(const std::string &file, DWORD f)
{
    if (fd>=0)
        close(fd);

    this->flags = f;
    this->posix_flags = 0;

    if (f & FFileOpenFlags::Read)
        this->posix_flags = O_RDONLY;
    if (f & FFileOpenFlags::Write)
    {
        if (this->posix_flags == O_RDONLY)
            this->posix_flags = O_RDWR;
        else
            this->posix_flags = O_WRONLY;
    }
    if (f & FFileOpenFlags::CreateNew)
        this->posix_flags |= O_CREAT | O_EXCL;
    if (f & FFileOpenFlags::OpenAlways)
        this->posix_flags |= O_CREAT;

    fd = open(file.c_str(), this->posix_flags, 644);
    name = file;
    atEOF = false;

    return fd >= 0;
}

int FileLinux::Write(void *data, DWORD length)
{
    int ret = write(fd, data, length);
    if (ret<0) // Total hack but ateof interface is stupid ;)
        atEOF = true;
    return ret;
}

int FileLinux::Read(void *dest, int length)
{
    int ret = read(fd, dest, length);
    if (ret<0) // Total hack but ateof interface is stupid ;)
        atEOF = true;
    return ret;
}

bool FileLinux::Seek(int offset, EFileSeekMethod::TYPE t)
{
    switch(t)
    {
    case EFileSeekMethod::Current: return lseek(fd, offset, SEEK_CUR) >=0; break;
    case EFileSeekMethod::FromBegin: return lseek(fd, offset, SEEK_SET) >=0; break;
    case EFileSeekMethod::FromEnd: return lseek(fd, offset, SEEK_END) >=0; break;
    default: return false; break;
    }
}

DWORD FileLinux::GetSize() const
{
    struct stat b;
    if (!fstat(fd, &b))
    {
        return b.st_size;
    }
    return 0;
}

bool FileLinux::IsOpen() const
{
    return lseek(fd, 0, SEEK_CUR) != (off_t)-1;
}

bool FileLinux::IsOpenForWrite() const
{
    return IsOpen() && flags & (FFileOpenFlags::Write);
}

bool FileLinux::IsOpenForRead() const
{
    return IsOpen() && flags & (FFileOpenFlags::Read);
}

void FileLinux::Close()
{
    close(fd);
    flags = 0;
    fd = -1;
    name = string();
    atEOF = false;
}

int FileLinux::GetCurrentPos() const
{
    if (fd <0)
        return 0;
    return lseek(fd, 0, SEEK_CUR);
}

std::string FileLinux::GetPath() const
{
    return name;
}

bool FileLinux::AtEnd() const
{
    return atEOF;
} //todo
#endif
