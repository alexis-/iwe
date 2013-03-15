#include "stdafx.h"

#include "UITreeItemList.h"
#include "UIProjectTree.h"
#include "FileView.h"

#include <re2/re2.h>



/////////////////////////////////////////////////////////////////////////////////////////////
//  CUIProjectTree
/////////////////////////////////////////////////////////////////////////////////////////////

CUIProjectTree::CUIProjectTree(CFileView* pFileView)
  : CUIProjectTreeElement<IUIProjectTree>(&(pFileView->getViewTree()), NULL),
    m_pFileView(pFileView)
{
  m_pViewTree = &(pFileView->getViewTree());

  pFileView->eventTreeLoaded +=
    FastDelegate0<>(this, &CUIProjectTree::onTreeLoaded);
  pFileView->eventTreeUnloaded +=
    FastDelegate0<>(this, &CUIProjectTree::onTreeUnloaded);
}

void
CUIProjectTree::onTreeLoaded()
{
  setReady(true);
}

void
CUIProjectTree::onTreeUnloaded()
{
  setReady(false);
}



/////////////////////////////////////////////////////////////////////////////////////////////
//  CUIProjectTreeElement<C>
/////////////////////////////////////////////////////////////////////////////////////////////

template <class C>
CUIProjectTreeElement<C>::CUIProjectTreeElement(CViewTree* pViewTree, IUIProjectTreeElementBase* pParent)
  : m_itemHandle(NULL),
    m_pItems(NULL),
    m_pViewTree(pViewTree),
    m_pParent(pParent)
{
  GetResourceMgr()->eventUpdateFinished +=
    FastDelegate0<>(this, &CUIProjectTreeElement::onResourceUpdate);
}

template <class C>
IUIProjectTreeElement*
CUIProjectTreeElement<C>::addCategory(const std::string& sCatName)
{
  CUIProjectTreeCategory* pCat = new CUIProjectTreeCategory(m_pViewTree, this, sCatName);

  m_vecElements.push_back(pCat);

  return pCat;
}

// http://www.codeproject.com/Articles/14703/How-to-use-32-bit-icons-in-CTreeCtrl
template <class C>
IUIProjectTreeElement*
CUIProjectTreeElement<C>::addItem(const std::string& sName /*, img*/)
{
  return NULL;
}

template <class C>
void
CUIProjectTreeElement<C>::addRegexFilter(const std::string& sRegex)
{
  m_vecRegex.push_back(sRegex);

  onResourceUpdate();
}

template <class C>
void
CUIProjectTreeElement<C>::setReady(bool bReady)
{
  if (bReady)
  {
    insertItem();

    m_pItems = new CItemList(m_pViewTree, m_itemHandle, 2);
    m_pItems->Update(m_listItems);
  }
  else if (m_pItems)
  {
    delete m_pItems;

    m_pItems = NULL;
    m_itemHandle = NULL;
  }

  for (size_t i = 0; i < m_vecElements.size(); i++)
    m_vecElements[i]->setReady(bReady);
}

template <class C>
void
CUIProjectTreeElement<C>::onResourceUpdate()
{
  const std::vector<std::pair<const Ogre::FileInfo, const std::string>>& vecFiles = GetResourceMgr()->getFiles();
  std::vector<std::pair<const Ogre::FileInfo, const std::string>>::const_iterator fileIt = vecFiles.begin();

  m_listItems.clear();

  for (; fileIt != vecFiles.end(); fileIt++)
  {
    const Ogre::FileInfo& fileInfo = (*fileIt).first;
  
    for (std::vector<std::string>::const_iterator it = m_vecRegex.begin();
         it != m_vecRegex.end();
         it++)
    {
      re2::RE2 re(*it);
      
      if (re.FullMatch(fileInfo.filename, re))
        m_listItems.push_back(fileInfo.filename);
    }
  }

  if (m_pItems)
    m_pItems->Update(m_listItems);
}



/////////////////////////////////////////////////////////////////////////////////////////////
//  CUIProjectTreeCategory
/////////////////////////////////////////////////////////////////////////////////////////////

CUIProjectTreeCategory::CUIProjectTreeCategory(CViewTree* pTree, IUIProjectTreeElementBase* pParent, const std::string& sCatName)
  : CUIProjectTreeElement<>(pTree, pParent),
    m_sCatName(sCatName)
{
}

void
CUIProjectTreeCategory::insertItem()
{
  m_itemHandle = m_pViewTree->InsertItem(m_sCatName.c_str(), 0, 0, m_pParent->getTreeItem());
}