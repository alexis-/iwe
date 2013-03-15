#ifndef ENTITY_MANAGER_H
# define ENTITY_MANAGER_H

namespace Ogre
{
	class Entity;
};

class CBaseEntity;

# include "IBaseEntity.h"
# define MAX_ENTITIES						4096

class CClassView;

class CEntityManager : public IEntityManager
{
public:
	static CEntityManager*									getInstance();
	~CEntityManager();

public:
  virtual IEntity*                        createEntity();
	virtual void														clear();

public:
	IBaseEntity*														newEnt();
  
	//void																		setClassView(CClassView* pClassView);

public:
	virtual iterator begin()
	{
		return m_vecEnts.begin();
	}

	virtual iterator end()
	{
		return m_vecEnts.end();
	}

	virtual const_iterator begin() const
	{
		return m_vecEnts.begin();
	}

	virtual const_iterator end() const
	{
		return m_vecEnts.end();
	}

	virtual uint getCount() const
	{
		return m_uEntCount;
	}

	//inline void setClosing()
	//{
	//	m_bClosing = true;
	//}

private:
	//std::map<std::string, Ogre::Entity*>		m_mapEntities;
	IBaseEntity*														m_pEntArr[MAX_ENTITIES];
	std::vector<uint>												m_vecFreeSlots;
	std::vector<IEntity*>								    m_vecEnts;
	uint																		m_uLastSlot;
	uint																		m_uEntCount;
	//CClassView*															m_pClassView;
	//bool																		m_bClosing;

private:
	uint																		getNewEntID();

private:
	CEntityManager();
	static CEntityManager*									m_pInst;

	friend CBaseEntity;
	void																		removeEnt(IBaseEntity* ent);
};

#endif // !ENTITY_MANAGER_H