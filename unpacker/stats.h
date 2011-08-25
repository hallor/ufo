#pragma once

#include <string>
struct sPckChunkStats
{
	sPckChunkStats()
	{
		memset(this, 0, sizeof(*this));
	}

	int m_Width;
	int m_Height;

	int m_LinesCount;

	int m_ValidChecksums;
	int m_InvalidChecksums;
};

struct sPckFileStats
{
	sPckFileStats()
	{
		memset(this, 0, sizeof(*this));
	}
	
	int m_ValidChecksums;
	int m_InvalidChecksums;

	int m_WithValidLineCount;
	int m_WithInvalidLineCount;
	
	int m_WidthMin;
	int m_WidthMax;
	int m_HeightMin;
	int m_HeightMax;
	
	int m_ChunksCount;
	int m_ValidImages;
	int m_InvalidImages;


};

struct sPckGlobalStats
{
	sPckGlobalStats()
	{
		memset(this, 0, sizeof(*this));
	}

	int m_PackagesCount;
	int m_WidthMin;
	int m_WidthMax;
	int m_HeightMin;
	int m_HeightMax;
};

void SaveStats(const std::string &file, const sPckChunkStats &stats);
void SaveStats(const std::string &file, const sPckFileStats &stats);
void SaveStats(const std::string &file, const sPckGlobalStats &stats);
