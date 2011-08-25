#pragma once
#include "palette.h"
#include "bitmap.h"

struct SPCXHeader
{
	char m_Manufacturer;
	char m_Version;
	char m_Encoding;
	char m_Bpp;
	short m_StartX;
	short m_StartY;
	short m_EndX;
	short m_EndY;
	short m_DpiX;
	short m_DpiY;
	char m_Palette[48];
	char m_Reserved;
	char m_Planes;
	short m_BPLine;
	short m_PaletteInfo;
	short m_HScreen;
	short m_VScreen;
	char m_Unused[54];
};

class cPCX
{
public:
	void LoadFrom(const std::string &file);
	void SaveTo(const std::string &file);

protected:
	SPCXHeader		m_Header;
	cPalette		m_Palette;
	cBitmapData		m_Bitmap;
};