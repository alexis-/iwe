#ifndef IUI_PROJECT_TREE_H
# define IUI_PROJECT_TREE_H


interface_class IUIProjectTreeElement
{
public:
  virtual IUIProjectTreeElement*    addCategory(const std::string& sCatName) = 0;

  virtual void                      addRegexFilter(const std::string& sRegex) = 0;

};

interface_class IUIProjectTree : public IUIProjectTreeElement
{
};

#endif // !IUI_PROJECT_TREE_H