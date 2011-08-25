#pragma once

#include <vector>
#include <string>

class cTabFile
{
public:
	cTabFile()
	{
		m_Valid = false;
	}

	bool LoadFrom(const std::string &file);

	int GetImagesCount() const { return m_Offsets.size(); }
	int GetImageOffset(int image) const;

	bool IsValid() const { return m_Valid; }

	void Clear();

protected:

	void PerformInternalCheck();

	bool m_Valid;
	std::vector<int> m_Offsets;
};