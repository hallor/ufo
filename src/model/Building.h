#ifndef BUILDING_H
#define BUILDING_H

#include <vector>
#include "CityCoord.h"

class Unit;
class Vehicle;

/** Base class for any building (including alien) */
class Building
{
public:
    Building();
		virtual ~Building();

		// Can this vehicle land inside this building ??
		virtual bool canLand(const Vehicle * v) const;

		// Can this unit enter this building
		virtual bool canEnter(const Unit * u) const;

		// When unit enters/exits building it's placed somewhere (+ it's added to building object list).
		// If building is then destroyed, units/vehicles may get damaged
		// Should be called once unit/vehicle is on proper position (landing zone and so on).
		// May move objects in boundaries of building -- feature that will allow to spread units in building
		virtual void enter(const Unit * u);
		virtual void enter(const Vehicle * v);

		virtual void exit(const Unit * u);
		virtual void exit(const Vehicle * v);

		// Returns places where flying vehicles can land
		virtual const CityCoordV & landingZones() const;

		// Returns places where ground vehicles can enter building
		virtual const CityCoordV & taxiZones() const;

		// Pedestrian tube entrances
		virtual const CityCoordV & tubeZones() const;

protected:
		// Returns places where flying vehicles can land
		CityCoordV & landingZones();

		// Returns places where ground vehicles can enter building
		CityCoordV & taxiZones();

		// Pedestrian tube entrances
		CityCoordV & tubeZones();

private:
		class Private;
		Private * p;
};

#endif // BUILDING_H
