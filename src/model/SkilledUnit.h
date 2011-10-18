#ifndef SKILLEDUNIT_H
#define SKILLEDUNIT_H
#include <string>
#include <vector>
#include "Unit.h"

/** Class represent units that have (some) skills/attrs. This may be soldiers, scientists, aliens and so on */
/** Unit type/class (scientist/soldier) is based on attribute values ;) */
class SkilledUnit : public Unit
{
public:

	// Possible attribute types
	enum AttributeType
	{
		A_HEALTH = 0,
		A_ACCURACY,
		A_REACTIONS,
		A_SPEED,
		A_STAMINA,
		A_BRAVERY,
		A_STRENGTH,
		A_PSI_ENERGY,
		A_PSI_ATTACK,
		A_PSI_DEFENSE,
		A_ENGINEERING,
		A_BIOCHEMISTRY,
		A_PHYSICS,
		A_INFILTRATION, // for aliens, so better aliens infiltrate better
		A_END // "end" attribute
	};

	/** Class representing one "attribute" - read only (can be modified only by Unit) */
	class Attribute
	{
	public:
		virtual const std::string & getName() const;
		virtual int getValue() const;
		virtual int getMax() const;
		virtual int getBase() const;
	};

	SkilledUnit();

	const Attribute & getAttribute(AttributeType type);

	// Returns if it's combat unit (engineering skills at 0)
	bool isSoldier() const;
	// Returns true if it's engineering unit (non-combat skill non-zero)
	bool isEngineer() const;

	// Will be called (for example) during training
	virtual void train(AttributeType type);

	// Will be called when unit will be using this attribute (training on the job)
	virtual void use(AttributeType type);
};

#endif // SKILLEDUNIT_H
