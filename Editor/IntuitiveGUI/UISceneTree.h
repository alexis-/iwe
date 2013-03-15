#ifndef UI_SCENE_TREE_H
# define UI_SCENE_TREE_H

class CItemList;

class CUITreeElement : public IUITreeElement
{
public:
  CUITreeElement(CViewTree* pViewTree);
  CUITreeElement(CViewTree* pViewTree, CUITreeElement* pParent);
  CUITreeElement(CViewTree* pViewTree, CUITreeElement* pParent, const std::string& sName);

  ~CUITreeElement();

public:
  virtual IUITreeElement*                   addCategory(const std::string& sCatName);
  virtual IUITreeElement*                   addCategory(int iNum, ...);
  virtual IUITreeElement*                   addItem(const std::string& sName /*, img*/);

public:
  virtual void                              insertItem();

public:
  // Event stuffs (Context menu, ...?)

protected:
  Event1<CUITreeElement*>                   eventDestroyed;

private:
  void                                      onResourceUpdate();
  void                                      onChildDestroyed(CUITreeElement* pElem);

  virtual const std::string&                getName()
  {
    return m_sName;
  }

  virtual HTREEITEM                         getTreeItem()
  {
    return m_itemHandle;
  }

protected:
  std::string                               m_sName;

  CViewTree*                                m_pViewTree;
  HTREEITEM                                 m_itemHandle;

  CUITreeElement*                           m_pParent;
  
  std::vector<CUITreeElement*>              m_vecElements;
};

extern std::map<HTREEITEM, CUITreeElement*> mapElements; // Ugly hack, move this to root element

/////////////////////////////////////////////////////////////////////////////////////////////
//  CUIProjectTree
/////////////////////////////////////////////////////////////////////////////////////////////

template <typename T>
class CUITree : public CUITreeElement
{
public:
  CUITree(CViewTree* pViewTree, T* pTreeHolder)
    : CUITreeElement(pViewTree),
      m_pViewTree(pViewTree)
  {
    pTreeHolder->eventTreeLoaded +=
      FastDelegate0<>(this, &CUITree::onTreeLoaded);
    pTreeHolder->eventTreeUnloaded +=
      FastDelegate0<>(this, &CUITree::onTreeUnloaded);

    pViewTree->eventItemSelected +=
      FastDelegate1<HTREEITEM>(this, &CUITree::onItemSelected);
    pViewTree->eventItemDeleted +=
      FastDelegate1<HTREEITEM>(this, &CUITree::onItemDeleted);
  }
  

public:
  virtual HTREEITEM                         getTreeItem()
  {
    return m_pViewTree->GetRootItem();
  }
  
private:
  virtual void                              insertItem() {};

private:
  void                                      onTreeLoaded()
  {
  }

  void                                      onTreeUnloaded()
  {
    for (auto it = m_vecElements.begin(); it != m_vecElements.end(); it++)
      delete *it;

    m_vecElements.clear();
  }
  
  void                                      onItemSelected(HTREEITEM item)
  {
    auto it = mapElements.find(item);

    if (it != mapElements.end())
      it->second->eventItemSelected(it->second);
  }

  void                                      onItemDeleted(HTREEITEM item)
  {
    auto it = mapElements.find(item);

    if (it != mapElements.end())
      it->second->eventItemDeleted(it->second);
  }

private:
  CViewTree*                                m_pViewTree;
};



#endif // !UI_SCENE_TREE_H