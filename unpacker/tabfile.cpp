#include "tabfile.h"
#include <iostream>

bool cTabFile::LoadFrom(const std::string &file)
{
	std::string tmp = file;
	tmp += ".tab";
	FILE *f = fopen(tmp.c_str(), "rb");
	if(!f)
	{
		////std::cout<<"Couldn't load images offset file: "<<tmp<<std::endl;
		return false;
	}

	bool data_is_good = true;

	while(data_is_good)
	{
		int offset = 0;
		fread(&offset, sizeof(offset), 1, f);
		
		data_is_good = !feof(f) && !ferror(f);
		
		if(data_is_good)
		{
			////std::cout<<"Image ("<<GetImagesCount()<<") at: "<<offset<<"x"<<offset*4<<std::endl;
			m_Offsets.push_back(offset * 4);
		}
	}

	fclose(f);
	PerformInternalCheck();
	////std::cout<<"Loaded images offset file: "<<tmp<<std::endl;
	////std::cout<<"	Found "<<GetImagesCount()<<" images"<<std::endl;

	return IsValid();
}

int cTabFile::GetImageOffset(int img) const
{
	if(img < 0 || img >= GetImagesCount())
		return -1;

	return m_Offsets[img];
}

void cTabFile::PerformInternalCheck()
{
	if(!GetImagesCount())
	{
		m_Valid = false;
		return;
	}

	int prev = -1;
	for(int i = 0; i < GetImagesCount(); ++i)
	{
		int next = GetImageOffset(i);
		if(next <= prev)
		{
			Clear();
			return;
		}
		prev = next;
	}

	m_Valid = true;
}

void cTabFile::Clear()
{
	m_Valid = false;
	m_Offsets.clear();
}
