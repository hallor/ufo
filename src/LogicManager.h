#ifndef LOGICMANAGER_H
#define LOGICMANAGER_H


class City;
/** Class responsible for keeping and updating game logic (excluding gfx) */
class LogicManager
{
public:
    LogicManager();
		~LogicManager();
		void init();
private:
		City * m_city;
};

#endif // LOGICMANAGER_H
