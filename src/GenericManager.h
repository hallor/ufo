#ifndef GENERICMANAGER_H
#define GENERICMANAGER_H

/** Generic interface for managers */
class GenericManager
{
public:
	virtual ~GenericManager() {}

	virtual bool init() = 0;
	virtual bool update() = 0;
	virtual bool exit() = 0;
};

#endif // GENERICMANAGER_H
