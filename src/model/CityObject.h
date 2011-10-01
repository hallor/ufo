#ifndef CITYOBJECT_H
#define CITYOBJECT_H

#include "CityCoord.h"
#include "Property.h"

/** Base class for every object that is placed on city map (unit, vehicle, dimension gate, whatever) */
class CityObject
{
public:

	MEMBER(CityCoord, Coord);
	MEMBER(bool, OnEarth); // False if object is in alien dimension
};

#endif // CITYOBJECT_H
