#ifndef IMPORTMANAGER_H
#define IMPORTMANAGER_H

#include <cstdint>
#include <string>

class Surface;

class SoundSample //TODO: remove later
{

};

typedef uint32_t ResourceID;

/** Class that handles loading all legacy game resources, and returning proper data.
 Will also abstract out numbering of resources (so may be called by id's not by strings)

 Numbers are session dependent (should not be serialized).

 0 is invalid resource id ("empty")
*/
class ImportManager
{
public:
	virtual ~ImportManager() {}

	/** Returns unique ID for resource located in file fn (relative to game root)
	 at position 'record'. This number is unique only during program execution*/
	virtual ResourceID getResourceID(const std::string & fn, int record = 0) = 0;

	/** Access time should be O(0), but surface may be loaded here. Owned by manager */
	virtual const Surface * getSurface(ResourceID id) = 0;

	/** As in @sa getSurface() */
	virtual const SoundSample * getSound(ResourceID id) = 0;

	/** Frees resource - unloads cached data */
	virtual void freeResource(ResourceID id) = 0;

	/** Frees *ALL* allocated gamedata */
	virtual void freeResources() = 0;
};

/** Factory - to replace later by zip access function */
ImportManager * CreateImportManager(const std::string & gamedir);

#endif // IMPORTMANAGER_H
