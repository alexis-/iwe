#ifndef UI_PROJECT_TREE_H
# define UI_PROJECT_TREE_H

class CFileView;
class CItemList;

interface_class IUIProjectTreeElementBase
{
public:
  virtual void                      setReady(bool bReady) = 0;
  virtual void                      insertItem() = 0;

  virtual HTREEITEM                 getTreeItem() = 0;
};

template <class C = IUIProjectTreeElement>
class CUIProjectTreeElement : public C, public IUIProjectTreeElementBase
{
public:
  CUIProjectTreeElement(CViewTree* pViewTree, IUIProjectTreeElementBase* pParent);

public:
  virtual IUIProjectTreeElement*            addCategory(const std::string& sCatName);
  virtual IUIProjectTreeElement*            addItem(const std::string& sName /*, img*/);

  virtual void                              addRegexFilter(const std::string& sRegex);

public:
  virtual void                              setReady(bool bReady);

public:
  // Event stuffs (Context menu, ...?)

private:
  void                                      onResourceUpdate();

  virtual HTREEITEM                         getTreeItem()
  {
    return m_itemHandle;
  }

protected:
  CItemList*                                m_pItems;

  CViewTree*                                m_pViewTree;
  HTREEITEM                                 m_itemHandle;

  IUIProjectTreeElementBase*                m_pParent;
  
  std::vector<IUIProjectTreeElementBase*>   m_vecElements;
  std::list<std::string>                    m_listItems;
  std::vector<std::string>                  m_vecRegex;
};

class CUIProjectTreeCategory : public CUIProjectTreeElement<>
{
public:
  CUIProjectTreeCategory(CViewTree* pTree, IUIProjectTreeElementBase* pParent, const std::string& sCatName);
  
private:
  virtual void                              insertItem();

private:
  std::string                               m_sCatName;
};

class CUIProjectTree : public CUIProjectTreeElement<IUIProjectTree>
{
public:
  CUIProjectTree(CFileView* pViewTree);

public:
  virtual HTREEITEM                         getTreeItem()
  {
    return m_pViewTree->GetRootItem();
  }
  
private:
  virtual void                              insertItem() {};

private:
  void                                      onTreeLoaded();
  void                                      onTreeUnloaded();

private:
  CViewTree*                                m_pViewTree;
  CFileView*                                m_pFileView;
};

#endif // !UI_PROJECT_TREE_H