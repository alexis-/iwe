#ifndef IUI_SCENE_TREE_H
# define IUI_SCENE_TREE_H

//# include <stdarg.h>

interface_class IUITreeElement
{
public:
  virtual ~IUITreeElement() {};

public:
  virtual const std::string&        getName() = 0;

public:
  virtual IUITreeElement*           addCategory(const std::string& sCatName) = 0;
  virtual IUITreeElement*           addCategory(int iNum, ...) = 0;
  virtual IUITreeElement*           addItem(const std::string& sName /*, img*/) = 0;

public:
  Event1<IUITreeElement*>           eventItemDeleted;
  Event1<IUITreeElement*>           eventItemSelected;
};

#endif // !IUI_SCENE_TREE_H