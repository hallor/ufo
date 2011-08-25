#include "bitmap.h"
#include "palette.h"
#include "tabfile.h"
#include "pcxfile.h"
#include "pckfile.h"

using namespace std;


void UnpackPackage(const std::string &in_dir, const std::string &out_dir, const std::string &package, bool strip_pck, bool strip_stats)
{
	std::cout<<"Unpacking: "<<in_dir+package<<std::endl;
	cPckFile pck_file;
	if(!pck_file.LoadPalette(in_dir + package))
	{
		std::cout<<"Couldn't load palette: "<<in_dir + package + ".pal - trying default"<<std::endl;
		if(!pck_file.LoadPalette(in_dir + "palette"))
		{
			std::cout<<"Default palette missing, cannot unpack"<<std::endl;
			return;
		}
	}
	pck_file.LoadPackage(in_dir + package);
	pck_file.StripTo(out_dir + package, strip_pck, strip_stats);
	return;
}


void BatchUnpack(const std::string &in_dir, const std::string &out_dir, bool strip_pck, bool strip_stats)
{
	std::string out_add;

	std::string package = "";

	WIN32_FIND_DATA data;
	HANDLE handle = FindFirstFile("packed\\*.pck", &data);
	bool succed = handle != INVALID_HANDLE_VALUE;
	while(succed)
	{
		package = data.cFileName;
		package = package.substr(0, package.length() - 4);
		CreateDirectory((out_dir + package).c_str(), NULL);
		string out_add = package + "\\";
		UnpackPackage(in_dir, out_dir + out_add, package.c_str(), strip_pck, strip_stats); 
		succed = FindNextFile(handle, &data);
	}
	
	FindClose(handle);
}


int main(int argc, char *argv[])
{
	std::string in_dir = "packed\\";
	std::string out_dir = "unpacked\\";
	
	BatchUnpack(in_dir, out_dir, false, false);
	system("pause");

	return 0;
}