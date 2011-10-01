#ifndef DIMENSIONGATE_H
#define DIMENSIONGATE_H

#include <utility>
#include "CityObject.h"
#include "Property.h"
#include "Vehicle.h"

/** Works +- like portal */
class DimensionGate : public CityObject
{
public:

	CONST_MEMBER(DimensionGate *, Other);

	bool CanEnter(const Vehicle * v)
	{
		return v->GetDimensionDrive();
	}
};

#endif // DIMENSIONGATE_H
