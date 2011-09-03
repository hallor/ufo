#include <string>
#include <algorithm>
#include <vector>
#include <map>
#include <fstream>
#include "importmanager.h"
#include "exceptions.h"

#include "pcxfile.h"

#include "logger.h"
using namespace std;

/* Max 15 resource types (4 bits reserved)*/
enum ImportResource
{
	/* Raster resources */
	RES_Invalid = 0,
	RES_PCX = 1,
	RES_PCK, //.tab files not needed - paired automatically
	RES_MouseCursor, // mouse.dat file
	/* Sound resources */
	RES_Sound,

	RES_END
};


/* 32-Bit structure split */
struct sResourceID
{
	unsigned type : 4; // max 15 types of resources
	unsigned file : 12; // max 4096 "files" of given type
	unsigned record : 16; // max 65k records per file
};

typedef union {
	struct sResourceID s;
	ResourceID id;
} rid;

inline ResourceID createID(unsigned int type, unsigned int file, unsigned int record)
{
	rid r;
	r.s.type = type;
	r.s.file = file;
	r.s.record = record;
	return r.id;
}

struct Importer
{
	virtual ~Importer() {};
	// Returns true if this file may belong to that importer
	virtual bool pathMatches(const std::string & fn) = 0;

	virtual ResourceID getResourceID(const std::string & fn, unsigned record) = 0;

	virtual void * getResource(unsigned file, unsigned record) = 0;

	virtual void freeResource(unsigned file, unsigned record) = 0;

	virtual void freeResources() = 0;

	ImportResource type;
};

struct CaseCmp
{
	bool operator() (const char & a, const char & b) const
	{
		return (tolower(a) == tolower(b));
	}
};

class StreamSource : public std::ifstream
{
public:
	StreamSource(const std::string & prefix, const std::string & postfix) : std::ifstream(prefix+"/"+postfix)
	{
		LogInfo("StreamSource: %s", (prefix+"/"+postfix).c_str());
	}
};

// Matched by extension, Surfaces should be freed after use (all will take ~500MB)
class PCXImporter : public Importer
{
	const string & game_root;
	std::map<std::string, int> file_map; // map filename -> file id
	std::vector<std::string> name_map; // map file id -> filename
	int freeID;

	std::vector<cPCXFile *> pcx; //these structures should eat ~70mb when every original pcx is loaded
	std::vector<Surface *> surfaces;
public:
	PCXImporter(const std::string & r) : game_root(r)
	{
		// This is +- number used in game pcx files (388). They will be loaded on-demand,
		// as extracted size would be ~0.5G
		pcx.resize(512, NULL);
		name_map.resize(512);
		surfaces.resize(512, NULL);
		freeID=-1;
		type = RES_PCX;
	}

	~PCXImporter()
	{
		freeResources();
	}

	virtual bool pathMatches(const std::string & fn)
	{
		if (fn.length()<5) // a.pcx
			return false;
		std::string x = fn.substr(fn.length() - 4, 4);
		return std::equal(x.begin(), x.end(), ".pcx", CaseCmp());
	}

	// Record is always 0
	virtual ResourceID getResourceID(const std::string & fn, unsigned record)
	{
		if (record !=0)
			return 0;
		if (file_map.count(fn))
			return file_map.at(fn);

		if (freeID > (int)pcx.size())
			throw std::bad_alloc();

		freeID++;
		file_map[fn] = freeID;
		name_map[freeID] = fn;
		return createID(type, freeID, 0);
	}

	virtual void * getResource(unsigned file, unsigned record)
	{
		if (record !=0)
			return NULL;
		if (file > freeID)
			return NULL;
		if (surfaces[file])
			return surfaces[file];
		/* Not available - try to generate from PCX...*/
		if (!pcx[file])
		{
			StreamSource f(game_root, name_map[file]);
			cPCXFile * p = new cPCXFile();
			pcx[file] = p;
			p->loadFrom(f);
		}
		surfaces[file] = new Surface(pcx[file]->render());

		return surfaces[file];
	}

	virtual void freeResource(unsigned file, unsigned record)
	{
		if (record !=0)
			return;
		if (file > freeID)
			return;

		if (surfaces[file])
		{
			surfaces[file]->clear();
			delete surfaces[file];
			surfaces[file]=NULL;
		}
		if (pcx[file])
		{
			delete pcx[file];
			pcx[file] = NULL;
		}
	}

	virtual void freeResources()
	{
		for (unsigned i=0; i<=freeID; ++i)
			freeResource(i, 0);
	}
};

class ImportManagerFilesystem : public ImportManager
{
public:

	ImportManagerFilesystem(const std::string & gamedir) : dir(gamedir)
	{
		importers.resize(16);
		importers[RES_PCX] = new PCXImporter(dir);
	}

	~ImportManagerFilesystem()
	{
		for (unsigned i=0; i<importers.size(); ++i)
			delete importers[i];
		importers.clear();
	}

	virtual ResourceID getResourceID(const std::string & fn, int record = 0)
	{
		for (unsigned i=0; i<importers.size(); ++i)
		{
			if (importers[i] && importers[i]->pathMatches(fn))
			{
				return importers[i]->getResourceID(fn, record);
				break;
			}
		}
		return 0;
	}

	virtual Surface * getSurface(ResourceID id)
	{
		rid r;
		r.id = id;
		if (!importers[r.s.type])
			return NULL; //TODO: replace with "error" pixmap
		return (Surface*)importers[r.s.type]->getResource(r.s.file, r.s.record);
	}

	virtual SoundSample * getSound(ResourceID /*id*/)
	{
		return NULL;
	}

	virtual void freeResource(ResourceID id)
	{
		rid r;
		r.id = id;
		if (!importers[r.s.type])
			return;
		importers[r.s.type]->freeResource(r.s.file, r.s.record);
	}

	virtual void freeResources()
	{
		for (int i=0; importers.size(); ++i)
			if (importers[i])
				importers[i]->freeResources();
	}

	const std::string dir;
	vector<Importer*> importers;
};



ImportManager * CreateImportManager(const std::string & gamedir)
{
	return new ImportManagerFilesystem(gamedir);
}
