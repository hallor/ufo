#pragma once
#include "bitmap.h"
#include <vector>

class cPalette
{
public:
	cPalette()
	{
		m_Valid = false;
	}

	bool LoadFrom(const std::string &file);
	bool LoadFrom(int length, void *data);

	void SaveTo(const std::string &file);

	const SPixel &At(unsigned int pos) const;

	bool IsValid() const;

	void Clear();

protected:

	std::vector<SPixel> m_Data;	
	bool m_Valid;
};