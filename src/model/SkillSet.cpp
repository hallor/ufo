#include "game_pch.h"
#include <vector>
#include <algorithm>
#include <stdexcept>
#include "SkillSet.h"

using std::string;
using std::vector;
using std::max;
using std::min;

// TODO: move it to i18n code
static string attr_names[ISkillSet::A_END] =
{
    "Health", "Accuracy", "Reactions",
    "Speed", "Stamina", "Bravery",
    "Strength", "Psi energy", "Psi attack",
    "Psi defense", "Engineering", "Biochemistry",
    "Physics", "Infiltration"
};

static string attr_none("Unknowning");

class Attribute : public SkillSet::IAttribute
{
public:

    Attribute(const string * n = &attr_none) : name(n) {}

    virtual const string & getName() const { return *name; }
    virtual int getValue() const { return current; }
    virtual int getMax() const { return max; }
    virtual int getBase() const { return base; }

    void reset(int val)
    {
        base = max = current = val;
    }

    const string * name; // Name of attribute
    int base; // Value at which unit starts
    int max; // Max value that makes sense for this skill (usually 100)
    int current; // Current value
};



class SkillSet::Private
{
public:
    vector<Attribute> attr;
};

SkillSet::SkillSet() : p(0)
{
    p = new Private;
    p->attr.reserve(ISkillSet::A_END);
    for (int i=0; i<ISkillSet::A_END; ++i)
        p->attr.push_back(Attribute(attr_names+i));
}

SkillSet::~SkillSet()
{
    delete p;
}

// Returns if atribute set represents "soldier" unit
bool SkillSet::isSoldier() const
{
    return !isEngineer();
}

// Returns true if it's engineering unit (non-combat skill non-zero)
bool SkillSet::isEngineer() const
{
    return p->attr[ISkillSet::A_ENGINEERING].getValue() ||
            p->attr[ISkillSet::A_BIOCHEMISTRY].getValue() ||
            p->attr[ISkillSet::A_PHYSICS].getValue();
}

// Will be called (for example) during training.
void SkillSet::train(AttributeType /*type*/)
{
    throw std::logic_error("Not implemented.");
}

// Will be called when unit will be using this attribute (training on the job)
void SkillSet::use(AttributeType /*type*/)
{
    throw std::logic_error("Not implemented.");
}

// "Damages" attribute by requested value (saturated at 0)
bool SkillSet::damage(AttributeType type, int value)
{
    Attribute & a = p->attr[type];

    a.current = max(a.current - value, 0);
    return a.getValue() == 0;
}

// "Fixes" previously damaged attribute (up to value x)
bool SkillSet::heal(AttributeType type, int value)
{
    Attribute & a = p->attr[type];
    a.current = min(a.current + value, a.max);
    return a.getValue() == a.max;
}

//// Generates soldier
//void SkillSet::generateSoldier(const Rand & r)
//{
//    for (int i=0; i<ISkillSet::A_END; ++i)
//        p->attr[i].reset(0);
//
//    for (int i=0; i<ISkillSet::A_ENGINEERING; ++i) // Combat skills
//        p->attr[i].reset(30 + r.get() % 30);
//}
//
//// Generates "random" scientist
//void SkillSet::generateEngineer(const Rand & r)
//{
//    for (int i=0; i<ISkillSet::A_END; ++i)
//        p->attr[i].reset(0);
//
//    for (int i=0; i<ISkillSet::A_ENGINEERING; ++i) // poor combat skills
//        p->attr[i].reset(25 + r.get() % 10);
//
//    p->attr[ISkillSet::A_ENGINEERING + r.get() % 3].reset(50 + r.get() % 50);
//}
//
//
//void SkillSet::generateInfiltrator(const Rand & r)
//{
//    generateSoldier(r);
//    p->attr[ISkillSet::A_INFILTRATION].reset(50 + r.get() % 50);
//}
