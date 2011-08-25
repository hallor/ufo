#include "stats.h"
#include <ostream>
#include <fstream>

void SaveStats(const std::string &file, const sPckChunkStats &stats)
{
	std::string tmp_file = file;
	tmp_file += ".txt";

	std::ofstream ostr(tmp_file);
	if(!ostr.is_open())
		return;

	ostr<<"Width: "<<stats.m_Width<<std::endl;
	ostr<<"Height: "<<stats.m_Height<<std::endl;
	ostr<<"Lines: "<<stats.m_LinesCount<<std::endl;
	ostr<<"Valid checksums: "<<stats.m_ValidChecksums<<std::endl;
	ostr<<"Invalid checksums: "<<stats.m_InvalidChecksums<<std::endl;

	ostr.close();
}

void SaveStats(const std::string &file, const sPckFileStats &stats)
{
	std::string tmp_file = file;
	tmp_file += ".txt";

	std::ofstream ostr(tmp_file);
	if(!ostr.is_open())
		return;
	
	ostr<<"Valid checksums: "<<stats.m_ValidChecksums<<std::endl;
	ostr<<"Invalid checksums: "<<stats.m_InvalidChecksums<<std::endl;
	ostr<<"Valid lines/height count: "<<stats.m_WithValidLineCount<<std::endl;
	ostr<<"Invalid lines/height count: "<<stats.m_WithInvalidLineCount<<std::endl; 
	ostr<<"Valid ratio: "<<stats.m_WithValidLineCount / (float)stats.m_ChunksCount<<std::endl;
	ostr<<"Chunks: "<<stats.m_ChunksCount<<std::endl;
	ostr<<"Valid chunks: "<<stats.m_ValidImages<<std::endl;
	ostr<<"Invalid chunks: "<<stats.m_InvalidImages<<std::endl;
	ostr<<"Width min: "<<stats.m_WidthMin<<std::endl;	
	ostr<<"Height min: "<<stats.m_HeightMin<<std::endl;
	ostr<<"Width max: "<<stats.m_WidthMax<<std::endl;
	ostr<<"Height min: "<<stats.m_HeightMax<<std::endl;

	ostr.close();
}

void SaveStats(const std::string &file, const sPckGlobalStats &stats)
{
	std::string tmp_file = file;
	tmp_file += ".txt";

	std::ofstream ostr(tmp_file);
	if(!ostr.is_open())
		return;
		
	ostr<<"Width min: "<<stats.m_WidthMin<<std::endl;	
	ostr<<"Height min: "<<stats.m_HeightMin<<std::endl;
	ostr<<"Width max: "<<stats.m_WidthMax<<std::endl;
	ostr<<"Width min: "<<stats.m_HeightMax<<std::endl;

	ostr.close();
}