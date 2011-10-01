#ifndef CITYCOORD_H
#define CITYCOORD_H

/** Coordinates on city map */
class CityCoord
{
public:
	CityCoord(float nx=0.0, float ny=0.0, float nz=0.0) : x(nx), y(ny), z(nz) {}

	float x, y, z;
};

#endif // CITYCOORD_H
