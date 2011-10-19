#ifndef UNIT_H
#define UNIT_H
#include <string>
#include "CityObject.h"
#include "Factions.h"

/** Base class for any unit - this not includes vehicles */
class Unit : public CityObject
{
public:
	enum UnitType {
		T_HUMAN = 0,
		T_ANDROID,
		T_HYBRID,
		T_ALIEN,
		T_NPC, // placeholder for various NPC's like sectoids, robots and soon
	};
    Unit();

		std::string name; // name of unit, may be "Anthropod" or normal name from gen.
		EFactionName::TYPE affinity; // corporation this unit belongs to
};

#endif // UNIT_H
