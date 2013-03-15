#ifndef UI_TREE_ITEM_LIST_H
# define UI_TREE_ITEM_LIST_H

class CItemList
{
public:
	CItemList(CViewTree* tree, HTREEITEM parent, int iconID)
		: m_pTree(tree),
			m_hParent(parent)
	{
		m_iIconID = iconID;
	}

  ~CItemList();

public:
	void Update(const std::list<std::string>& vecNewItem);
  void Update(const std::string& sItem);
	void Add(std::string sItem);
	void Remove(std::string sItem);

  void ClearChildrens();

private:
	std::map<std::string, HTREEITEM>			m_mapItems;
	CViewTree*														m_pTree;
	HTREEITEM															m_hParent;
	int																		m_iIconID;
};

#endif // !UI_TREE_ITEM_LIST_H