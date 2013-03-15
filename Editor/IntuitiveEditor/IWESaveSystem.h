#ifndef IWE_SAVE_SYSTEM_H
# define IWE_SAVE_SYSTEM_H

# include "SaveSystem.h"


#define IWE_VERSION							(int)100


class CIWESaveSystem : CSaveSystem
{
public:
	CIWESaveSystem();

public:
	virtual bool									OnSave(const std::string& filename);
	virtual bool									OnLoad(const std::string& filename);
};

#endif // !IWE_SAVE_SYSTEM_H