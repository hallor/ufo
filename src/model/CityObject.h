#ifndef CITYOBJECT_H
#define CITYOBJECT_H

#include "CityCoord.h"
#include "Property.h"

/** Base class for every object that is placed on city map (unit, vehicle, dimension gate, whatever) */
// May be put inside buildings and so on ??
class CityObject
{
public:

	MEMBER(CityCoord, Coord);
};

#endif // CITYOBJECT_H
