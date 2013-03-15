#ifndef UI_PROPERTY_GRID_H
# define UI_PROPERTY_GRID_H

# include "CPropertyGridCustomCtrl.h"

class CUIPropertyGridElement;



class CUIPropertyGrid : public IUIPropertyGrid
{
public:
  CUIPropertyGrid(CMFCPropertyGridCtrl* pPropGrid);

public:
  virtual IUIPropertyGridElement*         createRoot();
  virtual void                            setPropertyHolder(IPropertyHolder* pPropObj);

private:
  IPropertyHolder*                        m_pPropHolder;
  CMFCPropertyGridCtrl*                   m_pPropGrid;
  CUIPropertyGridElement*                 m_pCurrentPropObj;
};



class CUIPropertyGridElement : public IUIPropertyGridElement
{
public:
  virtual ~CUIPropertyGridElement();

public:
  virtual IUIPropertyGridElement*         addGroup(const std::string& sName);
  
  virtual IUIPropertyGridElement*         addBool(const std::string& sName, const std::string& sDesc, std::function<bool()> fGet, std::function<void(bool)> fSet, bool bReadOnly = false);
  virtual IUIPropertyGridElement*         addBool(const std::string& sName, const std::string& sDesc, std::function<bool()> fGet, FastDelegate1<bool> fSet, bool bReadOnly = false);
  virtual IUIPropertyGridElement*         addBool(const std::string& sName, const std::string& sDesc, FastDelegate0<bool> fGet, std::function<void(bool)> fSet, bool bReadOnly = false);
  virtual IUIPropertyGridElement*         addBool(const std::string& sName, const std::string& sDesc, FastDelegate0<bool> fGet, FastDelegate1<bool> fSet, bool bReadOnly = false);

  virtual IUIPropertyGridElement*         addInt(const std::string& sName, const std::string& sDesc, std::function<int()> fGet, std::function<void(int)> fSet, bool bReadOnly = false, int min = -0xFFFFFFF, int max = 0xFFFFFFF);
  virtual IUIPropertyGridElement*         addInt(const std::string& sName, const std::string& sDesc, std::function<int()> fGet, FastDelegate1<int> fSet, bool bReadOnly = false, int min = -0xFFFFFFF, int max = 0xFFFFFFF);
  virtual IUIPropertyGridElement*         addInt(const std::string& sName, const std::string& sDesc, FastDelegate0<int> fGet, std::function<void(int)> fSet, bool bReadOnly = false, int min = -0xFFFFFFF, int max = 0xFFFFFFF);
  virtual IUIPropertyGridElement*         addInt(const std::string& sName, const std::string& sDesc, FastDelegate0<int> fGet, FastDelegate1<int> fSet, bool bReadOnly = false, int min = -0xFFFFFFF, int max = 0xFFFFFFF);
  
  virtual IUIPropertyGridElement*         addFloat(const std::string& sName, const std::string& sDesc, std::function<float()> fGet, std::function<void(float)> fSet, bool bReadOnly = false, float min = -FLT_MAX, float max = FLT_MAX);
  virtual IUIPropertyGridElement*         addFloat(const std::string& sName, const std::string& sDesc, std::function<float()> fGet, FastDelegate1<float> fSet, bool bReadOnly = false, float min = -FLT_MAX, float max = FLT_MAX);
  virtual IUIPropertyGridElement*         addFloat(const std::string& sName, const std::string& sDesc, FastDelegate0<float> fGet, std::function<void(float)> fSet, bool bReadOnly = false, float min = -FLT_MAX, float max = FLT_MAX);
  virtual IUIPropertyGridElement*         addFloat(const std::string& sName, const std::string& sDesc, FastDelegate0<float> fGet, FastDelegate1<float> fSet, bool bReadOnly = false, float min = -FLT_MAX, float max = FLT_MAX);
  
  virtual IUIPropertyGridElement*         addString(const std::string& sName, const std::string& sDesc, std::function<const std::string&()> fGet, std::function<void(const std::string&)> fSet, bool bReadOnly = false);
  virtual IUIPropertyGridElement*         addString(const std::string& sName, const std::string& sDesc, std::function<const std::string&()> fGet, FastDelegate1<const std::string&> fSet, bool bReadOnly = false);
  virtual IUIPropertyGridElement*         addString(const std::string& sName, const std::string& sDesc, FastDelegate0<const std::string&> fGet, std::function<void(const std::string&)> fSet, bool bReadOnly = false);
  virtual IUIPropertyGridElement*         addString(const std::string& sName, const std::string& sDesc, FastDelegate0<const std::string&> fGet, FastDelegate1<const std::string&> fSet, bool bReadOnly = false);

  virtual IUIPropertyGridElement*         addNTuple(const std::string& sName, std::function<float(int)> fGet, std::function<void(float*)> fSet, bool bReadOnly, int nNum, ...);
  virtual IUIPropertyGridElement*         addNTuple(const std::string& sName, std::function<float(int)> fGet, FastDelegate1<float*> fSet, bool bReadOnly, int nNum, ...);
  virtual IUIPropertyGridElement*         addNTuple(const std::string& sName, FastDelegate1<int, float> fGet, std::function<void(float*)> fSet, bool bReadOnly, int nNum, ...);
  virtual IUIPropertyGridElement*         addNTuple(const std::string& sName, FastDelegate1<int, float> fGet, FastDelegate1<float*> fSet, bool bReadOnly, int nNum, ...);

  virtual IUIPropertyGridElement*         addVector2(const std::string& sName, std::function<float(int)> fGet, std::function<void(float, float)> fSet, bool bReadOnly = false);
  virtual IUIPropertyGridElement*         addVector2(const std::string& sName, std::function<float(int)> fGet, FastDelegate2<float, float> fSet, bool bReadOnly = false);
  virtual IUIPropertyGridElement*         addVector2(const std::string& sName, FastDelegate1<int, float> fGet, std::function<void(float, float)> fSet, bool bReadOnly = false);
  virtual IUIPropertyGridElement*         addVector2(const std::string& sName, FastDelegate1<int, float> fGet, FastDelegate2<float, float> fSet, bool bReadOnly = false);
  
  virtual IUIPropertyGridElement*         addVector3(const std::string& sName, std::function<float(int)> fGet, std::function<void(float, float, float)> fSet, bool bReadOnly = false);
  virtual IUIPropertyGridElement*         addVector3(const std::string& sName, std::function<float(int)> fGet, FastDelegate3<float, float, float> fSet, bool bReadOnly = false);
  virtual IUIPropertyGridElement*         addVector3(const std::string& sName, FastDelegate1<int, float> fGet, std::function<void(float, float, float)> fSet, bool bReadOnly = false);
  virtual IUIPropertyGridElement*         addVector3(const std::string& sName, FastDelegate1<int, float> fGet, FastDelegate3<float, float, float> fSet, bool bReadOnly = false);

  virtual IUIPropertyGridElement*         addQuaternion(const std::string& sName, std::function<float(int)> fGet, std::function<void(float, float, float, float)> fSet, bool bReadOnly = false);
  virtual IUIPropertyGridElement*         addQuaternion(const std::string& sName, std::function<float(int)> fGet, FastDelegate4<float, float, float, float> fSet, bool bReadOnly = false);
  virtual IUIPropertyGridElement*         addQuaternion(const std::string& sName, FastDelegate1<int, float> fGet, std::function<void(float, float, float, float)> fSet, bool bReadOnly = false);
  virtual IUIPropertyGridElement*         addQuaternion(const std::string& sName, FastDelegate1<int, float> fGet, FastDelegate4<float, float, float, float> fSet, bool bReadOnly = false);
  
  virtual IUIPropertyGridElement*         addComboBox(const std::string& sName, const std::string& sDesc, std::function<const char*(int)> fGet, std::function<void(const std::string&, int)> fSet, bool bReadOnly = false);
  virtual IUIPropertyGridElement*         addComboBox(const std::string& sName, const std::string& sDesc, std::function<const char*(int)> fGet, FastDelegate2<const std::string&, int> fSet, bool bReadOnly = false);
  virtual IUIPropertyGridElement*         addComboBox(const std::string& sName, const std::string& sDesc, FastDelegate1<int, const char*> fGet, std::function<void(const std::string&, int)> fSet, bool bReadOnly = false);
  virtual IUIPropertyGridElement*         addComboBox(const std::string& sName, const std::string& sDesc, FastDelegate1<int, const char*> fGet, FastDelegate2<const std::string&, int> fSet, bool bReadOnly = false);

public:
  virtual void                            addElement(CMFCPropertyGridProperty* pElement) = 0;
  virtual void                            redraw() {};

protected:
  std::vector<CUIPropertyGridElement*>    m_vecSubElements;
};



class CUIPropertyGridRoot : public CUIPropertyGridElement
{
public:
  CUIPropertyGridRoot(CMFCPropertyGridCtrl* pPropGrid);
  virtual ~CUIPropertyGridRoot();

private:
  virtual void                            addElement(CMFCPropertyGridProperty* pElement);
  virtual void                            propertyChanged() {};

  virtual void                            redraw();

private:
  CMFCPropertyGridCtrl*                   m_pPropGrid;
};



class CUIPropertyGridGroup : public CUIPropertyGridElement
{
public:
  CUIPropertyGridGroup(CUIPropertyGridElement* pParent, const std::string& sName);
  virtual ~CUIPropertyGridGroup();

private:
  virtual void                            addElement(CMFCPropertyGridProperty* pElement);
  virtual void                            propertyChanged() {};
  virtual void                            redraw();

private:
  CMFCPropertyGridProperty*               m_pProp;
};



template <typename T1, typename T2>
class CUIPropertyGridBool : public CUIPropertyGridElement
{
public:
  CUIPropertyGridBool(CUIPropertyGridElement* pParent, const std::string& sName, const std::string& sDesc, T1 fGet, T2 fSet, bool bReadOnly = false);
  virtual ~CUIPropertyGridBool();

private:
  virtual void                            addElement(CMFCPropertyGridProperty* pElement);
  virtual void                            propertyChanged();
  void                                    onValueChange(bool bNewVal);

private:
  CBoolProp*                              m_pProp;
  T1                                      m_fGet;
  T2                                      m_fSet;
  bool                                    m_bInRecurse;
};



template <typename T1, typename T2>
class CUIPropertyGridInt : public CUIPropertyGridElement
{
public:
  CUIPropertyGridInt(CUIPropertyGridElement* pParent, const std::string& sName, const std::string& sDesc, T1 fGet, T2 fSet, int min = -0xFFFFFFF, int max = 0xFFFFFFF, bool bReadOnly = false);
  virtual ~CUIPropertyGridInt();

private:
  virtual void                            addElement(CMFCPropertyGridProperty* pElement);
  virtual void                            propertyChanged();
  void                                    onValueChange(int iNewVal);

private:
  CIntProp*                               m_pProp;
  T1                                      m_fGet;
  T2                                      m_fSet;
  bool                                    m_bInRecurse;
};



template <typename T1, typename T2>
class CUIPropertyGridFloat : public CUIPropertyGridElement
{
public:
  CUIPropertyGridFloat(CUIPropertyGridElement* pParent, const std::string& sName, const std::string& sDesc, T1 fGet, T2 fSet, float min = -FLT_MAX, float max = FLT_MAX, bool bReadOnly = false);
  virtual ~CUIPropertyGridFloat();

private:
  virtual void                            addElement(CMFCPropertyGridProperty* pElement);
  virtual void                            propertyChanged();
  void                                    onValueChange(float fNewVal);

private:
  CFloatProp*                             m_pProp;
  T1                                      m_fGet;
  T2                                      m_fSet;
  bool                                    m_bInRecurse;
};



template <typename T1, typename T2>
class CUIPropertyGridString : public CUIPropertyGridElement
{
public:
  CUIPropertyGridString(CUIPropertyGridElement* pParent, const std::string& sName, const std::string& sDesc, T1 fGet, T2 fSet, bool bReadOnly = false);
  virtual ~CUIPropertyGridString();

private:
  virtual void                            addElement(CMFCPropertyGridProperty* pElement);
  virtual void                            propertyChanged();
  void                                    onValueChange(const char* fNewVal);

private:
  CStringProp*                            m_pProp;
  T1                                      m_fGet;
  T2                                      m_fSet;
  bool                                    m_bInRecurse;
};



template <typename T1, typename T2>
class CUIPropertyGridNTuple : public CUIPropertyGridElement
{
public:
  CUIPropertyGridNTuple(CUIPropertyGridElement* pParent, const std::string& sName, T1 fGet, T2 fSet, bool bReadOnly, int nNum, va_list ap);
  virtual ~CUIPropertyGridNTuple();

private:
  virtual void                            addElement(CMFCPropertyGridProperty* pElement);
  virtual void                            propertyChanged();
  void                                    onValueChange(float fNewVal, int index, bool bSingle);

private:
  CNtupleProp*                            m_pProp;
  CNtupleNumberSubProp**                  m_pSubProps;
  float*                                  m_fTmpVals;
  T1                                      m_fGet;
  T2                                      m_fSet;
  bool                                    m_bInRecurse;
  int                                     m_nNum;
};



template <typename T1, typename T2>
class CUIPropertyGridVector2 : public CUIPropertyGridElement
{
public:
  CUIPropertyGridVector2(CUIPropertyGridElement* pParent, const std::string& sName, T1 fGet, T2 fSet, bool bReadOnly = false);
  virtual ~CUIPropertyGridVector2();

private:
  virtual void                            addElement(CMFCPropertyGridProperty* pElement);
  virtual void                            propertyChanged();
  void                                    onValueChange(float fNewVal, int index, bool bSingle);

private:
  CNtupleProp*                            m_pProp;
  CNtupleNumberSubProp*                   m_pSubProps[2];
  float                                   m_fTmpVals[2];
  T1                                      m_fGet;
  T2                                      m_fSet;
  bool                                    m_bInRecurse;
};



template <typename T1, typename T2>
class CUIPropertyGridVector3 : public CUIPropertyGridElement
{
public:
  CUIPropertyGridVector3(CUIPropertyGridElement* pParent, const std::string& sName, T1 fGet, T2 fSet, bool bReadOnly = false);
  virtual ~CUIPropertyGridVector3();

private:
  virtual void                            addElement(CMFCPropertyGridProperty* pElement);
  virtual void                            propertyChanged();
  void                                    onValueChange(float fNewVal, int index, bool bSingle);

private:
  CNtupleProp*                            m_pProp;
  CNtupleNumberSubProp*                   m_pSubProps[3];
  float                                   m_fTmpVals[3];
  T1                                      m_fGet;
  T2                                      m_fSet;
  bool                                    m_bInRecurse;
};



template <typename T1, typename T2>
class CUIPropertyGridQuaternion : public CUIPropertyGridElement
{
public:
  CUIPropertyGridQuaternion(CUIPropertyGridElement* pParent, const std::string& sName, T1 fGet, T2 fSet, bool bReadOnly = false);
  virtual ~CUIPropertyGridQuaternion();

private:
  virtual void                            addElement(CMFCPropertyGridProperty* pElement);
  virtual void                            propertyChanged();
  void                                    onValueChange(float fNewVal, int index, bool bSingle);

private:
  CNtupleProp*                            m_pProp;
  CNtupleNumberSubProp*                   m_pSubProps[4];
  float                                   m_fTmpVals[4];
  T1                                      m_fGet;
  T2                                      m_fSet;
  bool                                    m_bInRecurse;
};



template <typename T1, typename T2>
class CUIPropertyGridComboBox : public CUIPropertyGridElement
{
public:
  CUIPropertyGridComboBox(CUIPropertyGridElement* pParent, const std::string& sName, const std::string& sDesc, T1 fGet, T2 fSet, bool bReadOnly = false);
  virtual ~CUIPropertyGridComboBox();

private:
  virtual void                            addElement(CMFCPropertyGridProperty* pElement);
  virtual void                            propertyChanged();
  void                                    onValueChange(const std::string& sNewStr, int index);

private:
  CComboBoxExProp*                        m_pProp;
  T1                                      m_fGet;
  T2                                      m_fSet;
  bool                                    m_bInRecurse;
};



#endif // !UI_PROPERTY_GRID_H