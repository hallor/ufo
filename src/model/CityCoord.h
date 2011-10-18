#ifndef CITYCOORD_H
#define CITYCOORD_H
#include <vector>

/** Coordinates on city map */
class CityCoord
{
public:
	static CityCoord null;

	// Positions should be > 0 (-1 == invalid
	CityCoord(float nx=0.0, float ny=0.0, float nz=0.0, bool alien = false) : x(nx), y(ny), z(nz),
	alien_dimension(alien) {}

	float x, y, z;
	bool alien_dimension;
};


typedef std::vector<CityCoord> CityCoordV;
#endif // CITYCOORD_H
