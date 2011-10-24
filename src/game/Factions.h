#pragma once

#include <string>

namespace EFactionName
{
    enum TYPE
    {
        XCom = 0,
        Alien,
        Government,
        Megapol,
        CultOfSirius,
        Marsec,
        Superdynamics,
        GeneralMetro,
        Cyberweb,
        Transtellar,
        Solmine,
        Sensovision,
        Lifetree,
        Nutrivend,
        Evonet,
        SanctuaryClinic,
        Nanotech,
        Energen,
        Synthemesh,
        GravballLeague,
        Psyke,
        Diablo,
        Osiron,
        SELF,
        MutantAlliance,
        Extropians,
        Technocrats,
        Civilian,
        _COUNT
    };

    std::string ToString(TYPE t);
    TYPE FromString(const std::string &str);
}
