#include "stdafx.h"

#include "UITreeItemList.h"

////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////



/////////////////////////////////////////////////////////////////////////////////////////////
//  CItemList
/////////////////////////////////////////////////////////////////////////////////////////////


CItemList::~CItemList()
{
  ClearChildrens();
}

void
CItemList::ClearChildrens()
{
  for (auto it = m_mapItems.begin(); it != m_mapItems.end(); it++)
    m_pTree->DeleteItem((*it).second);

  m_mapItems.clear();
}

void
CItemList::Update(const std::list<std::string>& vecNewItem)
{
	// Items to remove
	for (std::map<std::string, HTREEITEM>::iterator it = m_mapItems.begin();
			 it != m_mapItems.end();
			 it++)
	{
		if (!listContains(vecNewItem, (*it).first))
		{
			m_pTree->DeleteItem((*it).second);
			m_mapItems.erase(it);
		}
	}

  HTREEITEM lastItem = NULL;

	// Items to add
	for (std::list<std::string>::const_iterator it = vecNewItem.begin();
			 it != vecNewItem.end();
			 it++)
	{
		std::map<std::string, HTREEITEM>::iterator treeIt;
		if ((treeIt = m_mapItems.find(*it)) == m_mapItems.end())
		{
			HTREEITEM newItem = m_pTree->InsertItem((*it).c_str(), m_iIconID, m_iIconID, m_hParent, lastItem);
			m_mapItems[*it] = newItem;
      lastItem = newItem;
		}
    else
      lastItem = (*treeIt).second;
	}

	m_pTree->RedrawWindow();
}

void
CItemList::Update(const std::string& sItem)
{
  std::map<std::string, HTREEITEM>::iterator it = m_mapItems.find(sItem);

  if (it == m_mapItems.end())
  {
		m_pTree->DeleteItem((*it).second);
		m_mapItems.erase(it);

    return;
  }
  
	HTREEITEM newItem = m_pTree->InsertItem(sItem.c_str(), m_iIconID, m_iIconID, m_hParent);
	m_mapItems[sItem] = newItem;
}

void
CItemList::Add(std::string sItem)
{
	HTREEITEM newItem = m_pTree->InsertItem(sItem.c_str(), m_iIconID, m_iIconID, m_hParent);
	m_mapItems[sItem] = newItem;
  
	m_pTree->RedrawWindow();
}

void
CItemList::Remove(std::string sItem)
{
	if (m_mapItems.find(sItem) != m_mapItems.end())
	{
		HTREEITEM remItem = m_mapItems[sItem];
		
		m_pTree->DeleteItem(remItem);
		m_mapItems.erase(sItem);
	}
  
	m_pTree->RedrawWindow();
}