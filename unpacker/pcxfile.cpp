#include "pcxfile.h"

void cPCX::LoadFrom(const std::string &file)
{
	FILE *f = fopen(file.c_str(), "rb");
	if(!f)
	{
		//std::cout<<"Couldn't open file: "<<file.c_str()<<std::endl;
		return;
	}

	fseek(f, 0, SEEK_END);
	int length = ftell(f);
	fseek(f, 0, SEEK_SET);
	//std::cout<<"	Total file size is:"<<length<<std::endl;
	if(length < sizeof(m_Header))
	{
		//std::cout<<"	File is too small to be real pcx"<<std::endl;
		return;
	}

	fread(&m_Header, sizeof(m_Header), 1, f);

	int pos = ftell(f);

	if(m_Header.m_Bpp == 4)
	{
		m_Palette.LoadFrom(16, m_Header.m_Palette);
	}
	else
	if(m_Header.m_Bpp == 8)
	{
		if(length >= sizeof(m_Header) + 769)
		{
			fseek(f, -769, SEEK_END);
			char tmp[769] = {0};
			fread(tmp, sizeof(tmp), 1, f);
			if(tmp[0] != 0x0C)
				__debugbreak();

			m_Palette.LoadFrom(256, tmp + 1); 
			
			fseek(f, pos, SEEK_SET);
		}
		else
		{
			//std::cout<<"File damaged, no palette info"<<std::endl;
		}
	}
	else
	{
		//std::cout<<"Unrecognized palette"<<std::endl;
	}
	
	//m_Palette.LoadFrom("packed\\palette");

	m_Bitmap.Create(m_Header.m_EndX - m_Header.m_StartX + 1, m_Header.m_EndY - m_Header.m_StartY + 1);

	int line_lgt = m_Header.m_BPLine * m_Header.m_Planes;
	int line_padding = (line_lgt * 8 / m_Header.m_Bpp) - m_Bitmap.GetWidth();
	if(line_lgt <= 0 || line_padding < 0)
		__debugbreak();


	for(int i = 0; i < m_Bitmap.GetHeight(); ++i)
	{
		for(int j = 0; j < line_lgt;)
		{
			unsigned char data = 0;
			char runcount = 0;
			unsigned char runvalue = 0;
			fread(&data, sizeof(data), 1, f);
			if((data & 0xC0) == 0xC0)
			{
				runcount = data & 0x3F;
				fread(&runvalue, sizeof(runvalue), 1, f);
			}
			else
			{
				runcount = 1;
				runvalue = data; 
			}
		

			for(int z = 0; z < runcount && j + z < m_Bitmap.GetWidth(); ++z)
				m_Bitmap.At(j + z, m_Bitmap.GetHeight() - i - 1) = m_Palette.At(runvalue);

			j += runcount;
		}
	}
};

void cPCX::SaveTo(const std::string &file)
{
	if(m_Bitmap.GetHeight() > 0 && m_Bitmap.GetWidth() > 0)
		m_Bitmap.SaveToFile(file);
};