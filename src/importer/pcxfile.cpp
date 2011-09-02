#include <cstddef>
#include "pcxfile.h"

#include "exceptions.h"

tRGBA * cPCXFile::render() const
{
	if (m_Bitmap.isValid() && m_Palette.isValid())
		return m_Bitmap.render(m_Palette);
	return NULL;
}

void cPCXFile::loadFrom(std::istream & file) throw()
{
	if (!file.good())
		throw exceptions::io();

	int start_pos = (int)file.tellg();

	file.seekg(0, std::ios::end);
	int length = (int)file.tellg() - start_pos;
	file.seekg(start_pos);

	if(length < (int)sizeof(m_Header))
		throw exceptions::load_resource();

	if (!file.good())
		throw exceptions::load_resource();

	file.read((char*)&m_Header, sizeof(m_Header));

	if (file.gcount()!=sizeof(m_Header))
		throw exceptions::load_resource();

	if(m_Header.m_Bpp == 4)
	{
		file.seekg(start_pos + (int)offsetof(SPCXHeader, m_Palette));
		m_Palette.loadFrom(file, 16);
	}
	else if(m_Header.m_Bpp == 8)
	{
		if((unsigned int)length > sizeof(m_Header) + 768)
		{
			file.seekg(-769, std::ios::end);

			if (file.get() != 0xC)
				throw exceptions::load_resource();

			m_Palette.loadFrom(file);

			file.seekg(start_pos + sizeof(m_Header));
		}
		else
			throw exceptions::load_resource();
	} else
		throw exceptions::load_resource();

	m_Bitmap.create(m_Header.m_EndX - m_Header.m_StartX + 1, m_Header.m_EndY - m_Header.m_StartY + 1);

	int line_lgt = m_Header.m_BPLine * m_Header.m_Planes;
	int line_padding = (line_lgt * 8 / m_Header.m_Bpp) - m_Bitmap.width();

	if (line_lgt <= 0 || line_padding < 0)
		throw exceptions::load_resource();


	for(int i = 0; i < m_Bitmap.height(); ++i)
	{
		char runcount = 0;
		char data = 0;
		char runvalue = 0;

		for(int j = 0; j < line_lgt; j+= runcount)
		{

			if (!file.good())
				throw exceptions::load_resource();

			file.get(data);

			if (!file.good())
				throw exceptions::load_resource();

			if((data & 0xC0) == 0xC0)
			{
				runcount = data & 0x3F;
				file.get(runvalue);
				if (!file.good())
					throw exceptions::load_resource();
			}
			else
			{
				runcount = 1;
				runvalue = data;
			}

			for(int z = 0; z < runcount && j + z < m_Bitmap.width(); ++z)
				m_Bitmap.pixel(j + z, i) = runvalue;
		}
	}
}
