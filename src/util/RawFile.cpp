#include <cstring>
#include <algorithm>
#include "RawFile.h"

using namespace std;

bool cRawFile::Open(const std::string &path)
{
    if(m_File.GetPath() == path)
        return m_File.Seek(0, EFileSeekMethod::FromBegin);
        
    return m_File.Open(path, FFileOpenFlags::OpenExisting | FFileOpenFlags::Read);
};

bool cRawFile::Read(cFixedArray<char> *data, bool loop /* = false */)
{
    if(!data)
        return false;

    if(!m_File.IsOpenForRead())
        return false;

    if(loop && m_File.AtEnd())
        m_File.Seek(0, EFileSeekMethod::FromBegin);

    int data_size = data->GetSize();
    int file_size = m_File.GetSize();
    int cur_pos = m_File.GetCurrentPos();
		int size_to_read = loop ? data_size : min(file_size - cur_pos, data_size);

    if(data_size <= file_size - cur_pos)
    {
        m_File.Read(data->GetDataPointer(), data_size);
    }
    else
    {        
        if(loop)
        {
            int count_read = 0;
            while(count_read < data_size)
            {
                if(m_File.AtEnd())
                    m_File.Seek(0, EFileSeekMethod::FromBegin);

                int tmp = m_File.Read(data->GetDataPointer() + count_read, data_size - count_read);
                if(!tmp)
                    break;

                count_read += tmp;
            }
        }
        else
        {
            m_File.Read(data->GetDataPointer(), size_to_read);

            memcpy(data->GetDataPointer() + size_to_read, 0, data_size - size_to_read); 
        }
    }

    return true;
};

int cRawFile::GetSize() const
{
    if(!m_File.IsOpenForRead())
        return 0;

    return m_File.GetSize();
};

void cRawFile::Close()
{
    m_File.Close();
};
