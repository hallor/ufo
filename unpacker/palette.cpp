#include "palette.h"
#include <string>

bool cPalette::LoadFrom(const std::string &file)
{
	Clear();

	std::string tmp_file_name = file;
	tmp_file_name += ".pal";

	FILE *f = fopen(tmp_file_name.c_str(), "rb");
	if(!f)
	{
		////std::cout<<"Couldn't open palette: "<<tmp_file_name<<std::endl;
		return false;
	}

	char pdata[3] = {0};

	bool read_successfull = true;
	while(read_successfull)
	{
		int read = fread(pdata, sizeof(pdata), 1, f);

		read_successfull &= read == 1;
		read_successfull &= !feof(f) && !ferror(f);
	
		if(read_successfull)
		{
			SPixel px;
			px.r = pdata[0];
			px.g = pdata[1];
			px.b = pdata[2];
			m_Data.push_back(px);
		}
	}
	
	fclose(f);

	m_Valid = m_Data.size() == 16 || m_Data.size() == 256;

	////std::cout<<"Loaded palette: "<<tmp_file_name<<std::endl;
	////std::cout<<"	Size: "<<m_Data.size()<<std::endl;
	return true;
};

bool cPalette::LoadFrom(int length, void *data)
{
	Clear();

	if(length <= 0 || !data)
		return false;

	for(int i = 0; i < length; ++i)
	{
		char *pdata = (char*)data + i*3;
		SPixel px;
		px.r = pdata[0];
		px.g = pdata[1];
		px.b = pdata[2];
		m_Data.push_back(px);
	}

	m_Valid = m_Data.size() == 16 || m_Data.size() == 256;
	return true;
};

void cPalette::SaveTo(const std::string &file)
{
	if(m_Data.size() != 256)
	{
		return;
	}

	cBitmapData bmp;
	bmp.Create(16, 16);

	for(int i = 0; i < 16; ++i)
	{
		for(int j = 0; j < 16; ++j)
		{
			bmp.At(i, 16 - j - 1) = At(i + j * 16);
		}
	}

	bmp.SaveToFile(file);

};

const SPixel& cPalette::At(unsigned int pos) const
{
	if(pos >= m_Data.size())
		__debugbreak();

	return m_Data[pos];
};

bool cPalette::IsValid() const 
{
	return m_Valid && m_Data.size() == 16 || m_Data.size() == 256;
}

void cPalette::Clear()
{
	m_Data.clear();
	m_Valid = false;
};