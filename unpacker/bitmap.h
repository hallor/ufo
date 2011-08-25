#pragma once
#include <iostream>
#include <windows.h>
#include <string>

struct SPixel
{
	unsigned char b;
	unsigned char g;
	unsigned char r;
	unsigned char a;
	
	SPixel()
	{
		a = b = r = g = 0;
	}
};

class cBitmapData
{
public:

	cBitmapData();

	~cBitmapData();

	SPixel &At(int x, int y);

	void Create(int width, int height);
	void ClearPixels(const SPixel& torep);

	int GetWidth() const { return m_Meta.biWidth; };
	int GetHeight() const { return m_Meta.biHeight; };

	void SaveToFile(const std::string &file);

	void Clear();

protected:

	BITMAPFILEHEADER m_Header;
	BITMAPINFOHEADER m_Meta;

	SPixel *m_PixelData;
};
