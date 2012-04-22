#pragma once
#include <string>
#include "FixedArray.h"
#include "File.h"

class cFile;

class cRawFile
{
public:
    bool Open(const std::string &path);

    bool Read(cFixedArray<char> * data, bool loop = false);

    int GetSize() const;

    void Close();

protected:
    cFile m_File;
};
