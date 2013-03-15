#ifndef SAVE_SYSTEM_MANAGER_H
# define SAVE_SYSTEM_MANAGER_H

# include "SaveSystem.h"

class CSaveSystemManager
{
public:
	static CSaveSystemManager*												getInstance();

public:
	void																							registerSystem(CSaveSystem* pSystem);
	CSaveSystem*																			getFromExt(const std::string& sExt);
	
public:
	std::map<std::string, CSaveSystem*>::iterator			begin()
	{
		return m_mapSaveSystems.begin();
	}

	std::map<std::string, CSaveSystem*>::iterator			end()
	{
		return m_mapSaveSystems.end();
	}

private:
	CSaveSystemManager();
	static CSaveSystemManager*												m_pInst;

	std::map<std::string, CSaveSystem*>								m_mapSaveSystems;
};

#endif // !SAVE_SYSTEM_MANAGER_H