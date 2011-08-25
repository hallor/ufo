#include "bitmap.h"

cBitmapData::cBitmapData()
{
	m_PixelData = NULL;
	memset(&m_Header, 0, sizeof(m_Header));
	memset(&m_Meta, 0, sizeof(m_Meta));

	m_Header.bfType = 19778;
	m_Header.bfOffBits = sizeof(m_Header) + sizeof(m_Meta);
	m_Meta.biPlanes = 1;
	m_Meta.biBitCount = 32;
	m_Meta.biSize = sizeof(m_Meta);
	m_Meta.biCompression = BI_RGB;
};
cBitmapData::~cBitmapData()
{
	Clear();
};

SPixel & cBitmapData::At(int x, int y)
{
	if(x < 0 || x >= m_Meta.biWidth)
		__debugbreak();

	if(y < 0 || y >= m_Meta.biHeight)
		__debugbreak();

	return m_PixelData[x + m_Meta.biWidth * y];
}

void cBitmapData:: Create(int width, int height)
{
	Clear();
	if(width <= 0 || height <= 0)
	{
		////std::cout<<"width or height invalid"<<std::endl;
		return;
	}

	m_PixelData = new SPixel[width * height];
		
	m_Meta.biWidth = width;
	m_Meta.biHeight = height;
	m_Meta.biSizeImage = m_Meta.biWidth * m_Meta.biHeight * m_Meta.biBitCount / 8;
};

void cBitmapData::ClearPixels(const SPixel &torep)
{
	for(int i = 0; i < m_Meta.biWidth; ++i)
	{
		for(int j = 0; j < m_Meta.biHeight; ++j)
		{
			At(i, j) = torep;
		}
	}
}

void cBitmapData::SaveToFile(const std::string &file)
{
	FILE *f = fopen(file.c_str(), "wb");
	if(!f)
	{
		////std::cout<<"Couldn't create file: "<<file<<std::endl;
		return;
	}

	fwrite(&m_Header, sizeof(m_Header), 1, f);
	fwrite(&m_Meta, sizeof(m_Meta), 1, f);

	fwrite(m_PixelData, m_Meta.biSizeImage, 1, f);
	fclose(f);

	////std::cout<<"Saved file: "<<file<<std::endl;
	return;
}

void cBitmapData::Clear()
{
	if(m_PixelData)
		delete [] m_PixelData;

	m_PixelData = NULL;
	m_Meta.biWidth = 0;
	m_Meta.biHeight = 0;
	m_Meta.biSizeImage = 0;
};