#pragma once

#include <iostream>
#include <string>
#include <vector>
#include "tabfile.h"
#include "palette.h"
#include "stats.h"

struct sPckHeader
{
	sPckHeader()
	{
		Clear();
	}

	void Clear()
	{	
		memset(this, 0, sizeof(*this));
	}

	bool IsValid() const
	{
		return m_Header == 1 && m_Width > 0 && m_Height > 0;
	}

	int m_Header;
	short m_MinSkipLeft;
	short m_Width;
	short m_SkipTop;
	short m_Height;
};

struct sPckEofTag
{
	sPckEofTag()
	{
		Clear();
	}

	void Clear()
	{
		memset(this, 0, sizeof(*this));
	}

	bool IsValid() const { return m_Data[0] == 0xFFFFFFFF; }

	int m_Data[4];
};

struct sPckLineHeader
{
	sPckLineHeader()
	{
		Clear();
	}

	void Clear()
	{
		memset(this, 0, sizeof(*this));
	}

	unsigned char m_Checksum;
	unsigned char m_LineIndex;
	unsigned char m_Unknown1[2];
	unsigned char m_Skip;
	unsigned char m_DataLength;
	unsigned char m_Unknown2[2];
};

class cPckLine
{
public:
	cPckLine()
	{
		m_Data = NULL;
		m_Valid = false;
	}

	~cPckLine()
	{
		Release();
	}

	void Release()
	{	
		if(m_Data)
			delete [] m_Data;

		m_Data = NULL;
		m_Header.Clear();
		m_Valid = false;
	}

	bool MakeFrom(void *data, int data_size);

	bool IsValid() const { return m_Valid; }

	int GetTotalDataSize() const { return IsValid() ? sizeof(m_Header) + m_Header.m_DataLength : 0; }
	
	const sPckLineHeader &GetHeader() const { return m_Header; }
	const unsigned char* const GetData() const { return m_Data; }

protected:

	 void CreateData(int size)
	 {
		 Release();
		 m_Data = new unsigned char[size];
	 }

	sPckLineHeader m_Header;
	unsigned char *m_Data;
	bool m_Valid;
};

class cPckChunk
{
public:
	cPckChunk()
	{
		m_Valid = false;
	}

	~cPckChunk()
	{
		Release();
	}

	void Release()
	{
		for(unsigned int i = 0; i < m_Lines.size(); ++i)
		{
			delete m_Lines[i];
		}

		m_Header.Clear();
		m_Lines.clear();
		m_Eof.Clear();
		m_Valid = false;
	}

	int GetWidth() const { return IsValid() ? m_Header.m_Width : 0; }
	int GetHeight() const { return IsValid() ? m_Header.m_Height : 0; }

	int GetLinesCount() const { return IsValid() ? m_Lines.size() : 0; }

	bool IsValid() const { return m_Valid; }

	bool MakeFrom(void *data, int data_size);
	// No extension
	bool SaveAsSinglePck(const std::string &file);
	bool SaveAsBmp(const std::string &file, const cPalette &pal);

	bool GetStats(sPckChunkStats &stats);

protected:

	void FillStats();

	sPckHeader m_Header;
	std::vector<cPckLine*> m_Lines;
	sPckEofTag m_Eof;

	sPckChunkStats m_Stats;

	bool m_Valid;
};

class cPckFile
{
public:
	cPckFile();
	~cPckFile();

	// don't pass extensions here
	// loads .tab and associated .pck file if possible
	bool LoadPackage(const std::string &source);
	// loads palette to use when stripping files to .bmp
	bool LoadPalette(const std::string &source);
	// strips all loaded files and outputs them to $dest folder
	bool StripTo(const std::string &dest, bool strip_pck = false, bool strip_stats = false);
	// releases all internal data
	void Release();

	bool GetStats(sPckFileStats &stats);

	bool IsValid() const { return m_Valid; }

protected:

	void CreateBuffer(int size);
	void DeleteBuffer();

	void FillStats();

	cPckChunk *m_Chunks;
	cTabFile m_Tab;
	cPalette m_Palette;
	unsigned char *m_Buffer;
	int m_BufferLength;
	bool m_Valid;
	sPckFileStats m_Stats;
};