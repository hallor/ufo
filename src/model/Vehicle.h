#ifndef VEHICLE_H
#define VEHICLE_H

#include "Property.h"
#include "CityObject.h"

class VehicleArmor
{
public:
	MEMBER(int, ArmorTop);
	MEMBER(int, ArmorBottom);
	MEMBER(int, ArmorLeft);
	MEMBER(int, ArmorRight);
	MEMBER(int, ArmorFront);
	MEMBER(int, ArmorBack);

	int getOverall() const
	{
		return m_ArmorBack + m_ArmorBottom + m_ArmorFront + m_ArmorLeft + m_ArmorRight + m_ArmorTop;
	}
};

/** Base vehicle class - includes every craft (incl. ufos and overspawn) */
class Vehicle : public CityObject
{
public:
	Vehicle();

	MEMBER(int, Constitution);
	MEMBER(int, GrossWeight);
	MEMBER(int, MaxPassengers);
	MEMBER(int, NetPrice);
	MEMBER(int, Accuracy);
	MEMBER(int, CargoSpace);
	MEMBER(VehicleArmor, Armor);
	MEMBER(bool, DimensionDrive); // If set, vehicle is capable of using Dimension Gates
};

#endif // VEHICLE_H
