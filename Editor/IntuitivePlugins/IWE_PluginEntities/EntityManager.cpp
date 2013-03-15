#include "IWEPlugin.h"

#include "CBaseEntity.h"
#include "EntityManager.h"

//#include "ClassView.h"

CEntityManager* CEntityManager::m_pInst = NULL;

CEntityManager*
CEntityManager::getInstance()
{
	if (!m_pInst)
		m_pInst = new CEntityManager();

	return m_pInst;
}

CEntityManager::CEntityManager()
	: m_uLastSlot(0),
		m_uEntCount(0)
		//m_bClosing(false)
{
}

CEntityManager::~CEntityManager()
{
}

IEntity*
CEntityManager::createEntity()
{
  return newEnt();
}

IBaseEntity*
CEntityManager::newEnt()
{
	uint entID = getNewEntID();

	IBaseEntity* pEnt = new CBaseEntity(entID);

	m_pEntArr[entID] = pEnt;
	m_vecEnts.push_back(pEnt);

  eventEntityCreated(pEnt);

	return pEnt;
}

uint
CEntityManager::getNewEntID()
{
	uint entID = 0;

	if (!m_vecFreeSlots.empty())
	{
		entID = m_vecFreeSlots.back();
		m_vecFreeSlots.pop_back();
	}
	else
		entID = m_uLastSlot++;

	m_uEntCount++;

	//if (!m_bClosing)
	//{
	//	std::ostringstream ostream;
	//	ostream << "Entity (" << entID << ")";

	//	//m_pClassView->AddInstanceItems(ostream.str());
	//}

	return entID;
}

void
CEntityManager::removeEnt(IBaseEntity* ent)
{
	uint entID = ent->getEntID();

	if (entID == m_uLastSlot - 1)
		m_uLastSlot--;
	else
		m_vecFreeSlots.push_back(entID);

	for (iterator it = begin(); it != end(); it++)
	{
		if (*it == ent)
		{
			m_vecEnts.erase(it);
			break;
		}
	}

	m_pEntArr[entID] = NULL;
	m_uEntCount--;

	//std::ostringstream ostream;
	//ostream << "Entity (" << entID << ")";
	
	//if (!m_bClosing)
		//m_pClassView->RemoveInstanceItems(ostream.str());
  eventEntityRemoved(ent);
}

//void
//CEntityManager::setClassView(CClassView* pClassView)
//{
//	m_pClassView = pClassView;
//}

void
CEntityManager::clear()
{
	for (int i = m_uEntCount - 1; i >= 0; i--)
		delete m_pEntArr[i];

	memset(m_pEntArr, 0, MAX_ENTITIES * 4);

	m_vecEnts.clear();
	m_vecFreeSlots.clear();

	m_uLastSlot = 0;
	m_uEntCount = 0;
}