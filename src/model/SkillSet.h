#pragma once
#include <string>
#include <vector>

/**
  Skill set for all humanoid(ish) units used mostly for tactical game.
This includes - Soldiers, Scientists, Aliens and so on.

For X-Com soldiers for example this will be (probably) wrapped by proxy that does training
*/

class ISkillSet
{
public:
    // Possible attribute types - DO NOT REORDER - generation depends on them ;)
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
    virtual ~ISkillSet() {}

    // Will be called (for example) during training.
    virtual void train(AttributeType type) = 0;

    // Will be called when unit will be using this attribute (training on the job)
    virtual void use(AttributeType type) = 0;
};

class Rand;

/** Generic skill set implementation - usable for anyone */
class SkillSet : public ISkillSet
{
public:
    /** Class representing one "attribute" - read only (can be modified only by Unit) */
    class IAttribute
    {
    public:
        virtual const std::string & getName() const = 0;
        virtual int getValue() const = 0;
        virtual int getMax() const = 0; //current max for this attribute (usually == value)
        virtual int getBase() const = 0;
    };

    SkillSet();
    virtual ~SkillSet();

    const IAttribute & getAttribute(AttributeType type);

    // Returns if atribute set represents "soldier" unit
    virtual bool isSoldier() const;
    // Returns true if it's engineering unit (non-combat skill non-zero)
    virtual bool isEngineer() const;

    // Will be called (for example) during training.
    virtual void train(AttributeType type);

    // Will be called when unit will be using this attribute (training on the job)
    virtual void use(AttributeType type);

    // "Damages" attribute by requested value (saturated at 0)
    // returns true when value drops to 0
    virtual bool damage(AttributeType type, int value);

    // "Fixes" previously damaged attribute (up to value x)
    // returns true when value saturates
    virtual bool heal(AttributeType type, int value);

    // Generates soldier
    void generateSoldier(const Rand & r);
    // Generates "random" scientist
    void generateEngineer(const Rand & r);
    // Generates infiltrator
    void generateInfiltrator(const Rand & r);

private:
    // TODO: copy constructors for pimpls
    class Private;
    Private * p;
};
