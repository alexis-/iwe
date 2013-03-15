#ifndef IPROPERTY_HOLDER_H
# define IPROPERTY_HOLDER_H

interface_class IGUIInterface;
interface_class IUIPropertyGridElement;

interface_class IPropertyHolder
{
public:
  virtual IUIPropertyGridElement*   getPropObject(IGUIInterface* pGUI) = 0;
  virtual IUIPropertyGridElement*   getPropObject(IUIPropertyGridElement* pRoot) = 0;
  virtual void                      unselectPropHolder() = 0;
};

#endif // !IPROPERTY_HOLDER_H