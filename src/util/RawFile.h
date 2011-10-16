#pragma once
#include <string>
#include "FixedArray.h"

class iFile;

class cRawFile
{
public:
    cRawFile();
    ~cRawFile();

    bool Open(const std::string &path);

    bool Read(cFixedArray<char> * data, bool loop = false);

    int GetSize() const;

    void Release();

protected:
    iFile *m_File;
};
