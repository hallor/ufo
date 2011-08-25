#include "pckfile.h"
#include "bitmap.h"
#include <algorithm>

int PckLookupTable[10] = {0, 0, 1, 1, 1, 2, 2, 3, 3, 3};

bool cPckLine::MakeFrom(void *data, int data_size)
{
	Release();

	if(!data)
		return false;

	if(data_size < sizeof(m_Header))
		return false;

	int data_done = 0;
	memcpy(&m_Header, data, sizeof(m_Header));
	data_done += sizeof(m_Header);

	if(m_Header.m_DataLength > data_size - data_done)
	{
		//std::cout<<"Not enough data for line"<<std::endl;
		return false;
	}

	m_Data = new unsigned char[m_Header.m_DataLength];
	memcpy(m_Data, (unsigned char*)data + data_done, m_Header.m_DataLength);

	m_Valid = true;
	return true;
}

bool cPckChunk::MakeFrom(void *data, int data_size)
{
	Release();
	if(!data)
		return false;

	if(data_size < sizeof(m_Header) + sizeof(m_Eof)) // header + eof size
		return false;

	int data_done = 0;
	
	memcpy(&m_Header, data, sizeof(m_Header));
	if(!m_Header.IsValid())
	{
		//std::cout<<"Damaged header!"<<std::endl;
		return false;
	}
	data_done += sizeof(m_Header);

	while(data_done < data_size)
	{
		if(data_size - data_done < sizeof(m_Eof))
		{
			//std::cout<<"Damaged file! No eof tag"<<std::endl;
			return false;
		}
	
		memcpy(&m_Eof, (unsigned char*)data + data_done, sizeof(m_Eof));
		if(m_Eof.IsValid())
		{
			data_done += sizeof(m_Eof);
			//if(data_done  != data_size)
				//std::cout<<"Detected eof tag before real eof"<<std::endl;			
		}
		else
		{
			cPckLine *line = new cPckLine;
			m_Lines.push_back(line);
			if(!line->MakeFrom((unsigned char*)data + data_done, data_size - data_done))
			{
				//std::cout<<"Couldnt make line, file seems to be damaged"<<std::endl;
				return false;
			}
			else
			{
				data_done += line->GetTotalDataSize();
			}
		}
	}


	//std::cout<<"Done parsing file, everything seems ok"<<std::endl;
	m_Valid = true;

	FillStats();

	return true;
}

bool cPckChunk::SaveAsSinglePck(const std::string &out)
{
	if(!m_Valid)
		return false;

	std::string tmp_pck = out;
	tmp_pck += ".pck";

	std::string tmp_tab = out;
	tmp_tab += ".tab";

	FILE *file = fopen(tmp_tab.c_str(), "wb");
	if(!file)
	{
		//std::cout<<"Couldnt create file: "<<tmp_tab<<std::endl;
		return false;
	}

	int tmp = 0;
	fwrite(&tmp, sizeof(tmp), 1, file);
	fclose(file);

	file = fopen(tmp_pck.c_str(), "wb");
	if(!file)
	{
		//std::cout<<"Couldnt create file: "<<tmp_pck<<std::endl;
		return false;
	}

	fwrite(&m_Header, sizeof(m_Header), 1, file);
	for(int i = 0; i < m_Lines.size(); ++i)
	{
		cPckLine* line = m_Lines[i];
		const sPckLineHeader &head = line->GetHeader();
		fwrite(&head, sizeof(head), 1, file);
		fwrite(line->GetData(), head.m_DataLength, 1, file);
	}
	
	fwrite(&m_Eof, sizeof(m_Eof), 1, file);
	fclose(file);
	return true;
}

bool cPckChunk::SaveAsBmp(const std::string &out, const cPalette &pal)
{
	if(!m_Valid || !pal.IsValid())
		return false;

	cBitmapData bmp;
	bmp.Create(m_Header.m_Width, m_Header.m_Height);

	for(int i = 0; i < m_Lines.size(); ++i)
		{
			cPckLine *line = m_Lines[i];
			
			int line_in_bmp = line->GetHeader().m_LineIndex;
			line_in_bmp = (line_in_bmp / 10) * 4 + PckLookupTable[line_in_bmp % 10];
			if(line_in_bmp >= m_Header.m_Height)
				continue;
			
			for(int j = line->GetHeader().m_Skip ;j < line->GetHeader().m_DataLength + line->GetHeader().m_Skip && j < m_Header.m_Width; ++j)
			{
				int palette_idx = line->GetData()[j - line->GetHeader().m_Skip];
				const SPixel &px = pal.At(palette_idx);
				if(palette_idx > 0)
				{
					bmp.At(j, m_Header.m_Height - line_in_bmp - 1) = px;
				}
			}
		}

	bmp.SaveToFile(out + ".bmp");

	return true;
}

bool cPckChunk::GetStats(sPckChunkStats &out)
{
	if(!m_Valid)
		return false;

	out = m_Stats;

	return true;
}

void cPckChunk::FillStats()
{
	if(!m_Valid)
		return;

	memset(&m_Stats, 0, sizeof(m_Stats));

	m_Stats.m_Height = m_Header.m_Height;
	m_Stats.m_Width = m_Header.m_Width;
	m_Stats.m_LinesCount = m_Lines.size();

	for(unsigned int i = 0; i < m_Lines.size(); ++i)
	{
		const sPckLineHeader &header = m_Lines[i]->GetHeader();
		if((header.m_Checksum & 0x3f) == (header.m_Skip & 0x3f))
		{
			++m_Stats.m_ValidChecksums;
		}
		else
			++m_Stats.m_InvalidChecksums;
	}
}

cPckFile::cPckFile()
{
	m_Chunks = NULL;
	m_Buffer = NULL;
	m_BufferLength = 0;
	m_Valid = false;
}

cPckFile::~cPckFile()
{
	Release();
}

bool cPckFile::LoadPackage(const std::string &src)
{
	Release();
	if(src.empty())
		return false;

	if(!m_Tab.LoadFrom(src))
	{
		return false;
	}

	FILE *file = fopen((src + ".pck").c_str(), "rb");

	if(!file)
		return false;

	int file_size = 0;
	fseek(file, 0, SEEK_END);
	file_size = ftell(file);
	fseek(file, 0, SEEK_SET);

	m_Chunks = new cPckChunk[m_Tab.GetImagesCount()];
	
	int read = 0;

	int images_count = m_Tab.GetImagesCount();

	for(int i = 0; i < images_count; ++i)
	{
		int data_size = 0;
		if(i == images_count - 1) 
			data_size = file_size - m_Tab.GetImageOffset(images_count - 1);
		else
			data_size = m_Tab.GetImageOffset(i + 1) - m_Tab.GetImageOffset(i);  

		if(data_size + read > file_size)
			break;

		CreateBuffer(data_size);
		fread(m_Buffer, data_size, 1, file);

		m_Chunks[i].MakeFrom(m_Buffer, data_size);
	}

	fclose(file);

	m_Valid = true;
	FillStats();
	return true;
}

bool cPckFile::LoadPalette(const std::string &src)
{
	m_Palette.LoadFrom(src);
	return m_Palette.IsValid();
}

bool cPckFile::StripTo(const std::string &dest, bool strip_pck, bool strip_stats)
{
	if(!m_Chunks || !m_Tab.IsValid() || !m_Palette.IsValid())
		return false;

	for(int i = 0; i < m_Tab.GetImagesCount(); ++i)
	{
		if(!m_Chunks[i].IsValid())
			continue;

		char buf[1024];
		sprintf_s<1024>(buf, "%s%i", dest.c_str(), i);
		m_Chunks[i].SaveAsBmp(buf, m_Palette);
		
		if(strip_pck)
			m_Chunks[i].SaveAsSinglePck(buf);
		
		sPckChunkStats stats;
		if(strip_stats && m_Chunks[i].GetStats(stats))
			SaveStats(std::string(buf) + "_stats", stats);
	}

	if(strip_stats)
		SaveStats(dest + "_fullstats", m_Stats);

	return true;
}

void cPckFile::Release()
{
	if(m_Chunks)
		delete [] m_Chunks;

	m_Chunks = NULL;
	m_Tab.Clear();

	m_Valid = false;

	DeleteBuffer();
}

bool cPckFile::GetStats(sPckFileStats &stats)
{
	if(!m_Valid)
		return false;

	stats = m_Stats;

	return true;
}

void cPckFile::CreateBuffer(int size)
{
	if(size > m_BufferLength)
	{
		DeleteBuffer();
		m_Buffer = new unsigned char[size];
		m_BufferLength = size;
	}
}

void cPckFile::DeleteBuffer()
{
	if(m_Buffer)
		delete [] m_Buffer;

	m_Buffer = NULL;
	m_BufferLength = 0;
}

void cPckFile::FillStats()
{
	if(!m_Valid)
		return;

	memset(&m_Stats, 0, sizeof(m_Stats));
	int maxx = -1, maxy = -1;
	int minx = 0x3fffffff, miny = 0x3fffffff;

	m_Stats.m_ChunksCount = m_Tab.GetImagesCount();

	for(int i = 0; i < m_Tab.GetImagesCount(); ++i)
	{
		cPckChunk *chunk = &m_Chunks[i];
		if(!chunk->IsValid())
			++m_Stats.m_InvalidImages;
		else
			++m_Stats.m_ValidImages;

		sPckChunkStats chunk_stats;
		if(chunk->GetStats(chunk_stats))
		{
			m_Stats.m_InvalidChecksums += chunk_stats.m_InvalidChecksums;
			m_Stats.m_ValidChecksums += chunk_stats.m_ValidChecksums;
		}

		if(chunk->GetHeight() < chunk->GetLinesCount())
			++m_Stats.m_WithInvalidLineCount;
		else
			++m_Stats.m_WithValidLineCount;

		maxx = max(maxx, chunk->GetWidth());
		minx = min(minx, chunk->GetWidth());
		maxy = max(maxy, chunk->GetHeight());
		miny = min(miny, chunk->GetWidth());
	}
	
	m_Stats.m_WidthMax = maxx;
	m_Stats.m_WidthMin = minx;
	m_Stats.m_HeightMax = maxy;
	m_Stats.m_HeightMin = miny;
}
