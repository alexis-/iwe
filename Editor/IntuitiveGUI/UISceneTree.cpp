#include "stdafx.h"

#include "UITreeItemList.h"
#include "UISceneTree.h"


std::map<HTREEITEM, CUITreeElement*> mapElements = std::map<HTREEITEM, CUITreeElement*>();


/////////////////////////////////////////////////////////////////////////////////////////////
//  CUITreeElement
/////////////////////////////////////////////////////////////////////////////////////////////

CUITreeElement::CUITreeElement(CViewTree* pViewTree)
  : m_itemHandle(NULL),
    m_pViewTree(pViewTree),
    m_pParent(NULL),
    m_sName("")
{
}

CUITreeElement::CUITreeElement(CViewTree* pViewTree, CUITreeElement* pParent)
{
  CUITreeElement(pViewTree, pParent, "");
}

CUITreeElement::CUITreeElement(CViewTree* pViewTree, CUITreeElement* pParent, const std::string& sName)
  : m_itemHandle(NULL),
    m_pViewTree(pViewTree),
    m_pParent(pParent),
    m_sName(sName)
{
  insertItem();
}

CUITreeElement::~CUITreeElement()
{
  while (m_vecElements.size() > 0)
    delete m_vecElements[0];

  if (m_itemHandle)
  {
    m_pViewTree->DeleteItem(m_itemHandle);
    m_pViewTree->RedrawWindow();

    auto it = ::mapElements.find(m_itemHandle);
    if (it != ::mapElements.end())
      ::mapElements.erase(it);

    m_itemHandle = NULL;
  }

  m_pParent->onChildDestroyed(this);
}

void
CUITreeElement::onChildDestroyed(CUITreeElement* pChild)
{
  const std::string& sName = pChild->getName();


  for (auto it = m_vecElements.begin(); it != m_vecElements.end(); it++)
    if (*it == pChild)
    {
      m_vecElements.erase(it);
      break;
    }
}

IUITreeElement*
CUITreeElement::addCategory(const std::string& sCatName)
{
  for (auto it = m_vecElements.begin(); it != m_vecElements.end(); it++)
    if ((*it)->getName() == sCatName)
      return *it;

  CUITreeElement* pCat = new CUITreeElement(m_pViewTree, this, sCatName);

  m_vecElements.push_back(pCat);

  return pCat;
}

IUITreeElement*
CUITreeElement::addCategory(int iNum, ...)
{
  va_list ap;
  va_start(ap, iNum);

  CUITreeElement* ret = NULL;
  CUITreeElement* cur = this;

  for (int i = 0; i < iNum; i++)
  {
    char* pStr = va_arg(ap, char*);

    if (!pStr)
      break;

    cur = dynamic_cast<CUITreeElement*>(cur->addCategory(pStr));

    if (!ret)
      ret = cur;
  }

  va_end(ap);

  return ret;
}

IUITreeElement*
CUITreeElement::addItem(const std::string& sName /*, img*/)
{
  return addCategory(sName);
}

void
CUITreeElement::insertItem()
{
  m_itemHandle = m_pViewTree->InsertItem(m_sName.c_str(), 0, 0, m_pParent->getTreeItem());
  m_pViewTree->RedrawWindow();

  ::mapElements[m_itemHandle] = this;
}