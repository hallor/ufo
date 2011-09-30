#ifndef GAMEMODEL_H
#define GAMEMODEL_H

namespace EGameDifficulty
{
    enum TYPE
    {
	    Novice = 1,
	    Easy,
	    Medium,
	    Hard,
	    Superhuman
    };
};

/** Root for whole game model*/
class GameModel
{
public:
    GameModel();
};

#endif // GAMEMODEL_H
