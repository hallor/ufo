#include "Factions.h"

std::string EFactionName::ToString(EFactionName::TYPE t)
{
#define WRITE(p) case p: return #p;
    switch(t)
    {
        WRITE(XCom)
        WRITE(Alien)
        WRITE(Government)
        WRITE(Megapol)
        WRITE(CultOfSirius)
        WRITE(Marsec)
        WRITE(Superdynamics)
        WRITE(GeneralMetro)
        WRITE(Cyberweb)
        WRITE(Transtellar)
        WRITE(Solmine)
        WRITE(Sensovision)
        WRITE(Lifetree)
        WRITE(Nutrivend)
        WRITE(Evonet)
        WRITE(SanctuaryClinic)
        WRITE(Nanotech)
        WRITE(Energen)
        WRITE(Synthemesh)
        WRITE(GravballLeague)
        WRITE(Psyke)
        WRITE(Diablo)
        WRITE(Osiron)
        WRITE(SELF)
        WRITE(MutantAlliance)
        WRITE(Extropians)
        WRITE(Technocrats)
        WRITE(Civilian)
    }
#undef WRITE

    return "Unknown faction";
}

EFactionName::TYPE EFactionName::FromString(const std::string &str)
{
#define IF_RET(p) if(#p == str) return p; else
    IF_RET(XCom)
    IF_RET(Alien)
    IF_RET(Government)
    IF_RET(Megapol)
    IF_RET(CultOfSirius)
    IF_RET(Marsec)
    IF_RET(Superdynamics)
    IF_RET(GeneralMetro)
    IF_RET(Cyberweb)
    IF_RET(Transtellar)
    IF_RET(Solmine)
    IF_RET(Sensovision)
    IF_RET(Lifetree)
    IF_RET(Nutrivend)
    IF_RET(Evonet)
    IF_RET(SanctuaryClinic)
    IF_RET(Nanotech)
    IF_RET(Energen)
    IF_RET(Synthemesh)
    IF_RET(GravballLeague)
    IF_RET(Psyke)
    IF_RET(Diablo)
    IF_RET(Osiron)
    IF_RET(SELF)
    IF_RET(MutantAlliance)
    IF_RET(Extropians)
    IF_RET(Technocrats)
    IF_RET(Civilian)
#undef IF_RET

    return _COUNT;
}