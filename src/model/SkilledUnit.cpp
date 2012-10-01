#include "game_pch.h"
#include "SkilledUnit.h"

class Attribute
{
public:

	virtual const std::string & getName() const;
	virtual int getValue() const { return current; }
	virtual int getMax() const { return max; }
	virtual int getBase() const { return base; }

	std::string name; // Name of attribute
	int base; // Value at which unit starts
	int max; // Max value that makes sense for this skill (usually 100)
	int current; // Current value
};


// TODO: move it to i18n code
static std::string attr_names[SkilledUnit::A_END] =
{

};

SkilledUnit::SkilledUnit()
{
}
