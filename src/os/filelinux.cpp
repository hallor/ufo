#ifdef __linux__
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>

#include "filelinux.h"
#include "logger.h"

using namespace std;

FileLinux::FileLinux() : fd(-1), flags(0)
{
}

FileLinux::~FileLinux()
{
    if (fd>=0)
        close(fd);
    fd=-1;
}

bool FileLinux::Open(const std::string &file, DWORD flags)
{
    if (fd>=0)
        close(fd);

    this->flags = 0;

    if (flags & FFileOpenFlags::Read)
        this->flags = O_RDONLY;
    if (flags & FFileOpenFlags::Write)
    {
        if (this->flags == O_RDONLY)
            this->flags = O_RDWR;
        else
            this->flags = O_WRONLY;
    }
    if (flags & FFileOpenFlags::CreateNew)
        this->flags |= O_CREAT | O_EXCL;
    if (flags & FFileOpenFlags::OpenAlways)
        this->flags |= O_CREAT;

    fd = open(file.c_str(), this->flags, 644);

    return fd >= 0;
}

int FileLinux::Write(void *data, DWORD length)
{
    return write(fd, data, length);
}

int FileLinux::Read(void *dest, int length)
{
    return read(fd, dest, length);
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
    return lseek(fd, 0, SEEK_CUR) >=0;
}

bool FileLinux::IsOpenForWrite() const
{
    return IsOpen() && flags & (O_RDWR | O_WRONLY);
}

bool FileLinux::IsOpenForRead() const
{
    return IsOpen() && flags & (O_RDWR | O_RDONLY);
}

void FileLinux::Close()
{
    close(fd);
    flags = 0;
    fd = -1;
}

int FileLinux::GetIndex() const
{
    return -1;
}
#endif
