#include "stdafx.h"

#include "UIPropertyGrid.h"
#include "Interface.h"


/////////////////////////////////////////////////////////////////////////////////////////////
//  CUIPropertyGrid
/////////////////////////////////////////////////////////////////////////////////////////////

CUIPropertyGrid::CUIPropertyGrid(CMFCPropertyGridCtrl* pPropGrid)
  : m_pPropGrid(pPropGrid),
    m_pCurrentPropObj(NULL),
    m_pPropHolder(NULL)
{
}

IUIPropertyGridElement*
CUIPropertyGrid::createRoot() 
{
  return new CUIPropertyGridRoot(m_pPropGrid);
}

void
CUIPropertyGrid::setPropertyHolder(IPropertyHolder* pPropObj)
{
  if (m_pCurrentPropObj)
  {
    delete m_pCurrentPropObj;
    m_pCurrentPropObj = NULL;
  }

  if (m_pPropHolder)
  {
    m_pPropHolder->unselectPropHolder();
    m_pPropHolder = NULL;
  }

  m_pPropGrid->RemoveAll();

  if (pPropObj)
  {
    m_pCurrentPropObj = dynamic_cast<CUIPropertyGridElement*>(pPropObj->getPropObject(CGUIInterface::getInstance()));
    m_pCurrentPropObj->redraw();

    m_pPropHolder = pPropObj;
  }
  
  m_pPropGrid->Invalidate();
  m_pPropGrid->RedrawWindow();
}



/////////////////////////////////////////////////////////////////////////////////////////////
//  CUIPropertyGridElement
/////////////////////////////////////////////////////////////////////////////////////////////

CUIPropertyGridElement::~CUIPropertyGridElement()
{
  for (std::vector<CUIPropertyGridElement*>::iterator it = m_vecSubElements.begin();
       it != m_vecSubElements.end();
       it++)
    delete *it;

  m_vecSubElements.clear();
}





IUIPropertyGridElement*
CUIPropertyGridElement::addGroup(const std::string& sName)
{
  CUIPropertyGridElement* pElement = new CUIPropertyGridGroup(this, sName);

  m_vecSubElements.push_back(pElement);

  return pElement;
}





IUIPropertyGridElement*
CUIPropertyGridElement::addBool(const std::string& sName, const std::string& sDesc, std::function<bool()> fGet, std::function<void(bool)> fSet, bool bReadOnly)
{
  CUIPropertyGridElement* pElement = new CUIPropertyGridBool<std::function<bool()>, std::function<void(bool)>>(this, sName, sDesc, fGet, fSet, bReadOnly);

  m_vecSubElements.push_back(pElement);

  return pElement;
}

IUIPropertyGridElement*
CUIPropertyGridElement::addBool(const std::string& sName, const std::string& sDesc, std::function<bool()> fGet, FastDelegate1<bool> fSet, bool bReadOnly)
{
  CUIPropertyGridElement* pElement = new CUIPropertyGridBool<std::function<bool()>, FastDelegate1<bool>>(this, sName, sDesc, fGet, fSet, bReadOnly);

  m_vecSubElements.push_back(pElement);

  return pElement;
}

IUIPropertyGridElement*
CUIPropertyGridElement::addBool(const std::string& sName, const std::string& sDesc, FastDelegate0<bool> fGet, std::function<void(bool)> fSet, bool bReadOnly)
{
  CUIPropertyGridElement* pElement = new CUIPropertyGridBool<FastDelegate0<bool>, std::function<void(bool)>>(this, sName, sDesc, fGet, fSet, bReadOnly);

  m_vecSubElements.push_back(pElement);

  return pElement;
}

IUIPropertyGridElement*
CUIPropertyGridElement::addBool(const std::string& sName, const std::string& sDesc, FastDelegate0<bool> fGet, FastDelegate1<bool> fSet, bool bReadOnly)
{
  CUIPropertyGridElement* pElement = new CUIPropertyGridBool<FastDelegate0<bool>, FastDelegate1<bool>>(this, sName, sDesc, fGet, fSet, bReadOnly);

  m_vecSubElements.push_back(pElement);

  return pElement;
}





IUIPropertyGridElement*
CUIPropertyGridElement::addInt(const std::string& sName, const std::string& sDesc, std::function<int()> fGet, std::function<void(int)> fSet, bool bReadOnly, int min, int max)
{
  CUIPropertyGridElement* pElement = new CUIPropertyGridInt<std::function<int()>, std::function<void(int)>>(this, sName, sDesc, fGet, fSet, min, max, bReadOnly);

  m_vecSubElements.push_back(pElement);

  return pElement;
}

IUIPropertyGridElement*
CUIPropertyGridElement::addInt(const std::string& sName, const std::string& sDesc, std::function<int()> fGet, FastDelegate1<int> fSet, bool bReadOnly, int min, int max)
{
  CUIPropertyGridElement* pElement = new CUIPropertyGridInt<std::function<int()>, FastDelegate1<int>>(this, sName, sDesc, fGet, fSet, min, max, bReadOnly);

  m_vecSubElements.push_back(pElement);

  return pElement;
}

IUIPropertyGridElement*
CUIPropertyGridElement::addInt(const std::string& sName, const std::string& sDesc, FastDelegate0<int> fGet, std::function<void(int)> fSet, bool bReadOnly, int min, int max)
{
  CUIPropertyGridElement* pElement = new CUIPropertyGridInt<FastDelegate0<int>, std::function<void(int)>>(this, sName, sDesc, fGet, fSet, min, max, bReadOnly);

  m_vecSubElements.push_back(pElement);

  return pElement;
}

IUIPropertyGridElement*
CUIPropertyGridElement::addInt(const std::string& sName, const std::string& sDesc, FastDelegate0<int> fGet, FastDelegate1<int> fSet, bool bReadOnly, int min, int max)
{
  CUIPropertyGridElement* pElement = new CUIPropertyGridInt<FastDelegate0<int>, FastDelegate1<int>>(this, sName, sDesc, fGet, fSet, min, max, bReadOnly);

  m_vecSubElements.push_back(pElement);

  return pElement;
}





IUIPropertyGridElement*
CUIPropertyGridElement::addFloat(const std::string& sName, const std::string& sDesc, std::function<float()> fGet, std::function<void(float)> fSet, bool bReadOnly, float min, float max)
{
  CUIPropertyGridElement* pElement = new CUIPropertyGridFloat<std::function<float()>, std::function<void(float)>>(this, sName, sDesc, fGet, fSet, min, max, bReadOnly);

  m_vecSubElements.push_back(pElement);

  return pElement;
}

IUIPropertyGridElement*
CUIPropertyGridElement::addFloat(const std::string& sName, const std::string& sDesc, std::function<float()> fGet, FastDelegate1<float> fSet, bool bReadOnly, float min, float max)
{
  CUIPropertyGridElement* pElement = new CUIPropertyGridFloat<std::function<float()>, FastDelegate1<float>>(this, sName, sDesc, fGet, fSet, min, max, bReadOnly);

  m_vecSubElements.push_back(pElement);

  return pElement;
}

IUIPropertyGridElement*
CUIPropertyGridElement::addFloat(const std::string& sName, const std::string& sDesc, FastDelegate0<float> fGet, std::function<void(float)> fSet, bool bReadOnly, float min, float max)
{
  CUIPropertyGridElement* pElement = new CUIPropertyGridFloat<FastDelegate0<float>, std::function<void(float)>>(this, sName, sDesc, fGet, fSet, min, max, bReadOnly);

  m_vecSubElements.push_back(pElement);

  return pElement;
}

IUIPropertyGridElement*
CUIPropertyGridElement::addFloat(const std::string& sName, const std::string& sDesc, FastDelegate0<float> fGet, FastDelegate1<float> fSet, bool bReadOnly, float min, float max)
{
  CUIPropertyGridElement* pElement = new CUIPropertyGridFloat<FastDelegate0<float>, FastDelegate1<float>>(this, sName, sDesc, fGet, fSet, min, max, bReadOnly);

  m_vecSubElements.push_back(pElement);

  return pElement;
}





IUIPropertyGridElement*
CUIPropertyGridElement::addString(const std::string& sName, const std::string& sDesc, std::function<const std::string&()> fGet, std::function<void(const std::string&)> fSet, bool bReadOnly)
{
  CUIPropertyGridElement* pElement = new CUIPropertyGridString<std::function<const std::string&()>, std::function<void(const std::string&)>>(this, sName, sDesc, fGet, fSet, bReadOnly);

  m_vecSubElements.push_back(pElement);

  return pElement;
}

IUIPropertyGridElement*
CUIPropertyGridElement::addString(const std::string& sName, const std::string& sDesc, std::function<const std::string&()> fGet, FastDelegate1<const std::string&> fSet, bool bReadOnly)
{
  CUIPropertyGridElement* pElement = new CUIPropertyGridString<std::function<const std::string&()>, FastDelegate1<const std::string&>>(this, sName, sDesc, fGet, fSet, bReadOnly);

  m_vecSubElements.push_back(pElement);

  return pElement;
}

IUIPropertyGridElement*
CUIPropertyGridElement::addString(const std::string& sName, const std::string& sDesc, FastDelegate0<const std::string&> fGet, std::function<void(const std::string&)> fSet, bool bReadOnly)
{
  CUIPropertyGridElement* pElement = new CUIPropertyGridString<FastDelegate0<const std::string&>, std::function<void(const std::string&)>>(this, sName, sDesc, fGet, fSet, bReadOnly);

  m_vecSubElements.push_back(pElement);

  return pElement;
}

IUIPropertyGridElement*
CUIPropertyGridElement::addString(const std::string& sName, const std::string& sDesc, FastDelegate0<const std::string&> fGet, FastDelegate1<const std::string&> fSet, bool bReadOnly)
{
  CUIPropertyGridElement* pElement = new CUIPropertyGridString<FastDelegate0<const std::string&>, FastDelegate1<const std::string&>>(this, sName, sDesc, fGet, fSet, bReadOnly);

  m_vecSubElements.push_back(pElement);

  return pElement;
}





IUIPropertyGridElement*
CUIPropertyGridElement::addNTuple(const std::string& sName, std::function<float(int)> fGet, std::function<void(float*)> fSet, bool bReadOnly, int nNum, ...)
{
  va_list ap;
  va_start(ap, nNum);

  CUIPropertyGridElement* pElement = new CUIPropertyGridNTuple<std::function<float(int)>, std::function<void(float*)>>(this, sName, fGet, fSet, bReadOnly, nNum, ap);

  m_vecSubElements.push_back(pElement);

  return pElement;
}

IUIPropertyGridElement*
CUIPropertyGridElement::addNTuple(const std::string& sName, std::function<float(int)> fGet, FastDelegate1<float*> fSet, bool bReadOnly, int nNum, ...)
{
  va_list ap;
  va_start(ap, nNum);

  CUIPropertyGridElement* pElement = new CUIPropertyGridNTuple<std::function<float(int)>, FastDelegate1<float*>>(this, sName, fGet, fSet, bReadOnly, nNum, ap);

  m_vecSubElements.push_back(pElement);

  return pElement;
}

IUIPropertyGridElement*
CUIPropertyGridElement::addNTuple(const std::string& sName, FastDelegate1<int, float> fGet, std::function<void(float*)> fSet, bool bReadOnly, int nNum, ...)
{
  va_list ap;
  va_start(ap, nNum);

  CUIPropertyGridElement* pElement = new CUIPropertyGridNTuple<FastDelegate1<int, float>, std::function<void(float*)>>(this, sName, fGet, fSet, bReadOnly, nNum, ap);

  m_vecSubElements.push_back(pElement);

  return pElement;
}

IUIPropertyGridElement*
CUIPropertyGridElement::addNTuple(const std::string& sName, FastDelegate1<int, float> fGet, FastDelegate1<float*> fSet, bool bReadOnly, int nNum, ...)
{
  va_list ap;
  va_start(ap, nNum);

  CUIPropertyGridElement* pElement = new CUIPropertyGridNTuple<FastDelegate1<int, float>, FastDelegate1<float*>>(this, sName, fGet, fSet, bReadOnly, nNum, ap);

  m_vecSubElements.push_back(pElement);

  return pElement;
}





IUIPropertyGridElement*
CUIPropertyGridElement::addVector2(const std::string& sName, std::function<float(int)> fGet, std::function<void(float, float)> fSet, bool bReadOnly)
{
  CUIPropertyGridElement* pElement = new CUIPropertyGridVector2<std::function<float(int)>, std::function<void(float, float)>>(this, sName, fGet, fSet, bReadOnly);

  m_vecSubElements.push_back(pElement);

  return pElement;
}

IUIPropertyGridElement*
CUIPropertyGridElement::addVector2(const std::string& sName, std::function<float(int)> fGet, FastDelegate2<float, float> fSet, bool bReadOnly)
{
  CUIPropertyGridElement* pElement = new CUIPropertyGridVector2<std::function<float(int)>, FastDelegate2<float, float>>(this, sName, fGet, fSet, bReadOnly);

  m_vecSubElements.push_back(pElement);

  return pElement;
}

IUIPropertyGridElement*
CUIPropertyGridElement::addVector2(const std::string& sName, FastDelegate1<int, float> fGet, std::function<void(float, float)> fSet, bool bReadOnly)
{
  CUIPropertyGridElement* pElement = new CUIPropertyGridVector2<FastDelegate1<int, float>, std::function<void(float, float)>>(this, sName, fGet, fSet, bReadOnly);

  m_vecSubElements.push_back(pElement);

  return pElement;
}

IUIPropertyGridElement*
CUIPropertyGridElement::addVector2(const std::string& sName, FastDelegate1<int, float> fGet, FastDelegate2<float, float> fSet, bool bReadOnly)
{
  CUIPropertyGridElement* pElement = new CUIPropertyGridVector2<FastDelegate1<int, float>, FastDelegate2<float, float>>(this, sName, fGet, fSet, bReadOnly);

  m_vecSubElements.push_back(pElement);

  return pElement;
}




IUIPropertyGridElement*
CUIPropertyGridElement::addVector3(const std::string& sName, std::function<float(int)> fGet, std::function<void(float, float, float)> fSet, bool bReadOnly)
{
  CUIPropertyGridElement* pElement = new CUIPropertyGridVector3<std::function<float(int)>, std::function<void(float, float, float)>>(this, sName, fGet, fSet, bReadOnly);

  m_vecSubElements.push_back(pElement);

  return pElement;
}

IUIPropertyGridElement*
CUIPropertyGridElement::addVector3(const std::string& sName, std::function<float(int)> fGet, FastDelegate3<float, float, float> fSet, bool bReadOnly)
{
  CUIPropertyGridElement* pElement = new CUIPropertyGridVector3<std::function<float(int)>, FastDelegate3<float, float, float>>(this, sName, fGet, fSet, bReadOnly);

  m_vecSubElements.push_back(pElement);

  return pElement;
}

IUIPropertyGridElement*
CUIPropertyGridElement::addVector3(const std::string& sName, FastDelegate1<int, float> fGet, std::function<void(float, float, float)> fSet, bool bReadOnly)
{
  CUIPropertyGridElement* pElement = new CUIPropertyGridVector3<FastDelegate1<int, float>, std::function<void(float, float, float)>>(this, sName, fGet, fSet, bReadOnly);

  m_vecSubElements.push_back(pElement);

  return pElement;
}

IUIPropertyGridElement*
CUIPropertyGridElement::addVector3(const std::string& sName, FastDelegate1<int, float> fGet, FastDelegate3<float, float, float> fSet, bool bReadOnly)
{
  CUIPropertyGridElement* pElement = new CUIPropertyGridVector3<FastDelegate1<int, float>, FastDelegate3<float, float, float>>(this, sName, fGet, fSet, bReadOnly);

  m_vecSubElements.push_back(pElement);

  return pElement;
}





IUIPropertyGridElement*
CUIPropertyGridElement::addQuaternion(const std::string& sName, std::function<float(int)> fGet, std::function<void(float, float, float, float)> fSet, bool bReadOnly)
{
  CUIPropertyGridElement* pElement = new CUIPropertyGridQuaternion<std::function<float(int)>, std::function<void(float, float, float, float)>>(this, sName, fGet, fSet, bReadOnly);

  m_vecSubElements.push_back(pElement);

  return pElement;
}

IUIPropertyGridElement*
CUIPropertyGridElement::addQuaternion(const std::string& sName, std::function<float(int)> fGet, FastDelegate4<float, float, float, float> fSet, bool bReadOnly)
{
  CUIPropertyGridElement* pElement = new CUIPropertyGridQuaternion<std::function<float(int)>, FastDelegate4<float, float, float, float>>(this, sName, fGet, fSet, bReadOnly);

  m_vecSubElements.push_back(pElement);

  return pElement;
}

IUIPropertyGridElement*
CUIPropertyGridElement::addQuaternion(const std::string& sName, FastDelegate1<int, float> fGet, std::function<void(float, float, float, float)> fSet, bool bReadOnly)
{
  CUIPropertyGridElement* pElement = new CUIPropertyGridQuaternion<FastDelegate1<int, float>, std::function<void(float, float, float, float)>>(this, sName, fGet, fSet, bReadOnly);

  m_vecSubElements.push_back(pElement);

  return pElement;
}

IUIPropertyGridElement*
CUIPropertyGridElement::addQuaternion(const std::string& sName, FastDelegate1<int, float> fGet, FastDelegate4<float, float, float, float> fSet, bool bReadOnly)
{
  CUIPropertyGridElement* pElement = new CUIPropertyGridQuaternion<FastDelegate1<int, float>, FastDelegate4<float, float, float, float>>(this, sName, fGet, fSet, bReadOnly);

  m_vecSubElements.push_back(pElement);

  return pElement;
}





IUIPropertyGridElement*
CUIPropertyGridElement::addComboBox(const std::string& sName, const std::string& sDesc, std::function<const char*(int)> fGet, std::function<void(const std::string&, int)> fSet, bool bReadOnly)
{
  CUIPropertyGridElement* pElement = new CUIPropertyGridComboBox<std::function<const char*(int)>, std::function<void(const std::string&, int)>>(this, sName, sDesc, fGet, fSet, bReadOnly);

  m_vecSubElements.push_back(pElement);

  return pElement;
}

IUIPropertyGridElement*
CUIPropertyGridElement::addComboBox(const std::string& sName, const std::string& sDesc, std::function<const char*(int)> fGet, FastDelegate2<const std::string&, int> fSet, bool bReadOnly)
{
  CUIPropertyGridElement* pElement = new CUIPropertyGridComboBox<std::function<const char*(int)>, FastDelegate2<const std::string&, int>>(this, sName, sDesc, fGet, fSet, bReadOnly);

  m_vecSubElements.push_back(pElement);

  return pElement;
}

IUIPropertyGridElement*
CUIPropertyGridElement::addComboBox(const std::string& sName, const std::string& sDesc, FastDelegate1<int, const char*> fGet, std::function<void(const std::string&, int)> fSet, bool bReadOnly)
{
  CUIPropertyGridElement* pElement = new CUIPropertyGridComboBox<FastDelegate1<int, const char*>, std::function<void(const std::string&, int)>>(this, sName, sDesc, fGet, fSet, bReadOnly);

  m_vecSubElements.push_back(pElement);

  return pElement;
}

IUIPropertyGridElement*
CUIPropertyGridElement::addComboBox(const std::string& sName, const std::string& sDesc, FastDelegate1<int, const char*> fGet, FastDelegate2<const std::string&, int> fSet, bool bReadOnly)
{
  CUIPropertyGridElement* pElement = new CUIPropertyGridComboBox<FastDelegate1<int, const char*>, FastDelegate2<const std::string&, int>>(this, sName, sDesc, fGet, fSet, bReadOnly);

  m_vecSubElements.push_back(pElement);

  return pElement;
}





/////////////////////////////////////////////////////////////////////////////////////////////
//  CUIPropertyGridRoot
/////////////////////////////////////////////////////////////////////////////////////////////

CUIPropertyGridRoot::CUIPropertyGridRoot(CMFCPropertyGridCtrl* pPropGrid)
  : m_pPropGrid(pPropGrid)
{
}

CUIPropertyGridRoot::~CUIPropertyGridRoot()
{
  m_pPropGrid->RemoveAll();
}

void
CUIPropertyGridRoot::addElement(CMFCPropertyGridProperty* pElement)
{
  m_pPropGrid->AddProperty(pElement);
}

void
CUIPropertyGridRoot::redraw()
{
  for (std::vector<CUIPropertyGridElement*>::iterator it = m_vecSubElements.begin();
       it != m_vecSubElements.end();
       it++)
    (*it)->redraw();
}



/////////////////////////////////////////////////////////////////////////////////////////////
//  CUIPropertyGridGroup
/////////////////////////////////////////////////////////////////////////////////////////////

CUIPropertyGridGroup::CUIPropertyGridGroup(CUIPropertyGridElement* pParent, const std::string& sName)
{
  m_pProp = new CMFCPropertyGridProperty(sName.c_str());

  pParent->addElement(m_pProp);
  
}

CUIPropertyGridGroup::~CUIPropertyGridGroup()
{
}

void
CUIPropertyGridGroup::addElement(CMFCPropertyGridProperty* pElement)
{
  m_pProp->AddSubItem(pElement);
}

void
CUIPropertyGridGroup::redraw()
{
  m_pProp->Expand(false);
  m_pProp->Expand(true);
  m_pProp->Redraw();
}



/////////////////////////////////////////////////////////////////////////////////////////////
//  CUIPropertyGridBool
/////////////////////////////////////////////////////////////////////////////////////////////

template <typename T1, typename T2>
CUIPropertyGridBool<T1, T2>::CUIPropertyGridBool(CUIPropertyGridElement* pParent, const std::string& sName, const std::string& sDesc, T1 fGet, T2 fSet, bool bReadOnly)
{
  m_bInRecurse = false;
  m_fGet = fGet;
  m_fSet = fSet;

  m_pProp = new CBoolProp(sName.c_str(), fGet(), sDesc.c_str());
  //m_pProp->AllowEdit(!bReadOnly);
  m_pProp->eventUpdate += FastDelegate1<bool>(this, & CUIPropertyGridBool::onValueChange);

  pParent->addElement(m_pProp);
}

template <typename T1, typename T2>
CUIPropertyGridBool<T1, T2>::~CUIPropertyGridBool()
{
}

template <typename T1, typename T2>
void
CUIPropertyGridBool<T1, T2>::addElement(CMFCPropertyGridProperty* pElement)
{
}

template <typename T1, typename T2>
void
CUIPropertyGridBool<T1, T2>::propertyChanged()
{
  if (!m_bInRecurse)
  {
    m_bInRecurse = true;
    m_pProp->setValue(m_fGet());
    m_bInRecurse = false;
  }
}

template <typename T1, typename T2>
void
CUIPropertyGridBool<T1, T2>::onValueChange(bool bNewVal)
{
  if (!m_bInRecurse)
  {
    m_bInRecurse = true;
    m_fSet(bNewVal);
    m_bInRecurse = false;
  }
}



/////////////////////////////////////////////////////////////////////////////////////////////
//  CUIPropertyGridInt
/////////////////////////////////////////////////////////////////////////////////////////////

template <typename T1, typename T2>
CUIPropertyGridInt<T1, T2>::CUIPropertyGridInt(CUIPropertyGridElement* pParent, const std::string& sName, const std::string& sDesc, T1 fGet, T2 fSet, int min, int max, bool bReadOnly)
{
  m_bInRecurse = false;
  m_fGet = fGet;
  m_fSet = fSet;

  m_pProp = new CIntProp(sName.c_str(), fGet(), min, max, sDesc.c_str());
  m_pProp->AllowEdit(!bReadOnly);
  m_pProp->eventUpdate += FastDelegate1<int>(this, & CUIPropertyGridInt::onValueChange);

  pParent->addElement(m_pProp);
}

template <typename T1, typename T2>
CUIPropertyGridInt<T1, T2>::~CUIPropertyGridInt()
{
}

template <typename T1, typename T2>
void
CUIPropertyGridInt<T1, T2>::addElement(CMFCPropertyGridProperty* pElement)
{
}

template <typename T1, typename T2>
void
CUIPropertyGridInt<T1, T2>::propertyChanged()
{
  if (!m_bInRecurse)
  {
    m_bInRecurse = true;
    m_pProp->setValue(m_fGet());
    m_bInRecurse = false;
  }
}

template <typename T1, typename T2>
void
CUIPropertyGridInt<T1, T2>::onValueChange(int iNewVal)
{
  if (!m_bInRecurse)
  {
    m_bInRecurse = true;
    m_fSet(iNewVal);
    m_bInRecurse = false;
  }
}



/////////////////////////////////////////////////////////////////////////////////////////////
//  CUIPropertyGridFloat
/////////////////////////////////////////////////////////////////////////////////////////////

template <typename T1, typename T2>
CUIPropertyGridFloat<T1, T2>::CUIPropertyGridFloat(CUIPropertyGridElement* pParent, const std::string& sName, const std::string& sDesc, T1 fGet, T2 fSet, float min, float max, bool bReadOnly)
{
  m_bInRecurse = false;
  m_fGet = fGet;
  m_fSet = fSet;

  m_pProp = new CFloatProp(sName.c_str(), fGet(), min, max, sDesc.c_str());
  m_pProp->AllowEdit(!bReadOnly);
  m_pProp->eventUpdate += FastDelegate1<float>(this, & CUIPropertyGridFloat::onValueChange);

  pParent->addElement(m_pProp);
}

template <typename T1, typename T2>
CUIPropertyGridFloat<T1, T2>::~CUIPropertyGridFloat()
{
}

template <typename T1, typename T2>
void
CUIPropertyGridFloat<T1, T2>::addElement(CMFCPropertyGridProperty* pElement)
{
}

template <typename T1, typename T2>
void
CUIPropertyGridFloat<T1, T2>::propertyChanged()
{
  if (!m_bInRecurse)
  {
    m_bInRecurse = true;
    m_pProp->setValue(m_fGet());
    m_bInRecurse = false;
  }
}

template <typename T1, typename T2>
void
CUIPropertyGridFloat<T1, T2>::onValueChange(float fNewVal)
{
  if (!m_bInRecurse)
  {
    m_bInRecurse = true;
    m_fSet(fNewVal);
    m_bInRecurse = false;
  }
}



/////////////////////////////////////////////////////////////////////////////////////////////
//  CUIPropertyGridString
/////////////////////////////////////////////////////////////////////////////////////////////

template <typename T1, typename T2>
CUIPropertyGridString<T1, T2>::CUIPropertyGridString(CUIPropertyGridElement* pParent, const std::string& sName, const std::string& sDesc, T1 fGet, T2 fSet, bool bReadOnly)
{
  m_bInRecurse = false;
  m_fGet = fGet;
  m_fSet = fSet;

  m_pProp = new CStringProp(sName.c_str(), fGet(), sDesc.c_str());
  m_pProp->AllowEdit(!bReadOnly);
  m_pProp->eventUpdate += FastDelegate1<const char*>(this, & CUIPropertyGridString::onValueChange);

  pParent->addElement(m_pProp);
}

template <typename T1, typename T2>
CUIPropertyGridString<T1, T2>::~CUIPropertyGridString()
{
}

template <typename T1, typename T2>
void
CUIPropertyGridString<T1, T2>::addElement(CMFCPropertyGridProperty* pElement)
{
}

template <typename T1, typename T2>
void
CUIPropertyGridString<T1, T2>::propertyChanged()
{
  if (!m_bInRecurse)
  {
    m_bInRecurse = true;
    m_pProp->setValue(m_fGet());
    m_bInRecurse = false;
  }
}

template <typename T1, typename T2>
void
CUIPropertyGridString<T1, T2>::onValueChange(const char* sNewVal)
{
  if (!m_bInRecurse)
  {
    m_bInRecurse = true;
    m_fSet(sNewVal);
    m_bInRecurse = false;
  }
}



/////////////////////////////////////////////////////////////////////////////////////////////
//  CUIPropertyGridNTuple
/////////////////////////////////////////////////////////////////////////////////////////////

template <typename T1, typename T2>
CUIPropertyGridNTuple<T1, T2>::CUIPropertyGridNTuple(CUIPropertyGridElement* pParent, const std::string& sName, T1 fGet, T2 fSet, bool bReadOnly, int nNum, va_list ap)
{
  m_bInRecurse = false;
  m_fGet = fGet;
  m_fSet = fSet;

  m_pProp = new CNtupleProp(sName.c_str(), NULL, true);
  m_pProp->AllowEdit(!bReadOnly);

  pParent->addElement(m_pProp);
  
  const char* subNames[64];
  const char* subDesc[64];
  float subRangeMin[64];
  float subRangeMax[64];
  bool subReadOnly[64];

  for (int i = 0; i < 64; i++)
  {
    subNames[i] = subDesc[i] = "";
    subRangeMin[i] = -std::numeric_limits<float>::max();
    subRangeMax[i] = std::numeric_limits<float>::max();
    subReadOnly[i] = bReadOnly;
  }

  {
    // Names
    const char* pTxt = va_arg(ap, const char*);

    if ((int)pTxt != 0xe0000000)
    {
      int i = 0;

      do
      {
        subNames[i] = pTxt;

        pTxt = va_arg(ap, const char*);
      } while (++i < nNum);

      // Descriptions
      if ((int)pTxt != 0xe0000000)
      {
        int i = 0;

        do
        {
          subDesc[i] = pTxt;

          if (++i < nNum)
            pTxt = va_arg(ap, const char*);
        } while (i < nNum);

        // Read only
        int iVal = va_arg(ap, int);

        if (iVal == 0 || iVal == 1)
        {
          int i = 0;

          do
          {
            subReadOnly[i] = (iVal == 1);

            if (++i < nNum)
              iVal = va_arg(ap, int);
          } while (i < nNum);

          // Bounds
          float fVal = (float)va_arg(ap, double);

          if (fVal != std::numeric_limits<float>::max())
          {
            // Minimums
            int i = 0;

            do
            {
              subRangeMin[i] = fVal;

              if (++i < nNum)
                fVal = (float)va_arg(ap, double);
            } while (i < nNum);

            // Maximums
            fVal = (float)va_arg(ap, double);
            i = 0;

            do
            {
              subRangeMax[i] = fVal;

              if (++i < nNum)
                fVal = (float)va_arg(ap, double);
            } while (i < nNum);
          }
        }
      }
    }
  }

  m_nNum = nNum;
  m_fTmpVals = new float[nNum];
  m_pSubProps = new CNtupleNumberSubProp*[nNum];

  for (int i = 0; i < nNum; i++)
  {
    float fVal = fGet(i);
    m_fTmpVals[i] = fVal;
    m_pSubProps[i] = new CNtupleNumberSubProp(subNames[i], fVal, i, subDesc[i], subRangeMin[i], subRangeMax[i]);
    m_pSubProps[i]->AllowEdit(!subReadOnly[i]);
    m_pSubProps[i]->eventUpdate += FastDelegate3<float, int, bool>(this, &CUIPropertyGridNTuple::onValueChange);
    
    m_pProp->AddSubItem(m_pSubProps[i]);
  }
}

template <typename T1, typename T2>
CUIPropertyGridNTuple<T1, T2>::~CUIPropertyGridNTuple()
{
  delete m_fTmpVals;
  delete m_pSubProps;
}

template <typename T1, typename T2>
void
CUIPropertyGridNTuple<T1, T2>::addElement(CMFCPropertyGridProperty* pElement)
{
}

template <typename T1, typename T2>
void
CUIPropertyGridNTuple<T1, T2>::propertyChanged()
{
  if (!m_bInRecurse)
  {
    m_bInRecurse = true;

    for (int i = 0; i < m_nNum; i++)
    {
      float fVal = m_fGet(i);

      m_fTmpVals[i] = fVal;
      m_pSubProps[i]->setValue(fVal);
    }
    
    m_bInRecurse = false;
  }
}

template <typename T1, typename T2>
void
CUIPropertyGridNTuple<T1, T2>::onValueChange(float fNewVal, int index, bool bSingle)
{
  if (!m_bInRecurse)
  {
    m_fTmpVals[index] = fNewVal;

    if (index == m_nNum - 1 || bSingle)
    {
      m_bInRecurse = true;

      m_fSet(m_fTmpVals);
      
      m_bInRecurse = false;
    }
  }
}



/////////////////////////////////////////////////////////////////////////////////////////////
//  CUIPropertyGridVector2
/////////////////////////////////////////////////////////////////////////////////////////////

template <typename T1, typename T2>
CUIPropertyGridVector2<T1, T2>::CUIPropertyGridVector2(CUIPropertyGridElement* pParent, const std::string& sName, T1 fGet, T2 fSet, bool bReadOnly)
{
  m_bInRecurse = false;
  m_fGet = fGet;
  m_fSet = fSet;

  m_pProp = new CNtupleProp(sName.c_str(), NULL, true);
  m_pProp->AllowEdit(!bReadOnly);

  pParent->addElement(m_pProp);
  
  const char* subNames[2] = { "X", "Y" };
  const char* subDesc[2] = { "X Component", "Y Component" };

  for (int i = 0; i < 2; i++)
  {
    float fVal = fGet(i);
    m_fTmpVals[i] = fVal;
    m_pSubProps[i] = new CNtupleNumberSubProp(subNames[i], fVal, i, subDesc[i]);
    m_pSubProps[i]->AllowEdit(!bReadOnly);
    m_pSubProps[i]->eventUpdate += FastDelegate3<float, int, bool>(this, &CUIPropertyGridVector2::onValueChange);
    
    m_pProp->AddSubItem(m_pSubProps[i]);
  }
}

template <typename T1, typename T2>
CUIPropertyGridVector2<T1, T2>::~CUIPropertyGridVector2()
{
}

template <typename T1, typename T2>
void
CUIPropertyGridVector2<T1, T2>::addElement(CMFCPropertyGridProperty* pElement)
{
}

template <typename T1, typename T2>
void
CUIPropertyGridVector2<T1, T2>::propertyChanged()
{
  if (!m_bInRecurse)
  {
    m_bInRecurse = true;

    for (int i = 0; i < 2; i++)
    {
      float fVal = m_fGet(i);

      m_fTmpVals[i] = fVal;
      m_pSubProps[i]->setValue(fVal);
    }
    
    m_bInRecurse = false;
  }
}

template <typename T1, typename T2>
void
CUIPropertyGridVector2<T1, T2>::onValueChange(float fNewVal, int index, bool bSingle)
{
  if (!m_bInRecurse)
  {
    m_fTmpVals[index] = fNewVal;

    if (index == 1 || bSingle)
    {
      m_bInRecurse = true;

      m_fSet(m_fTmpVals[0], m_fTmpVals[1]);
      
      m_bInRecurse = false;
    }
  }
}



/////////////////////////////////////////////////////////////////////////////////////////////
//  CUIPropertyGridVector3
/////////////////////////////////////////////////////////////////////////////////////////////

template <typename T1, typename T2>
CUIPropertyGridVector3<T1, T2>::CUIPropertyGridVector3(CUIPropertyGridElement* pParent, const std::string& sName, T1 fGet, T2 fSet, bool bReadOnly)
{
  m_bInRecurse = false;
  m_fGet = fGet;
  m_fSet = fSet;

  m_pProp = new CNtupleProp(sName.c_str(), NULL, true);
  m_pProp->AllowEdit(!bReadOnly);

  pParent->addElement(m_pProp);
  
  const char* subNames[3] = { "X", "Y", "Z" };
  const char* subDesc[3] = { "X Component", "Y Component", "Z Component" };

  for (int i = 0; i < 3; i++)
  {
    float fVal = fGet(i);
    m_fTmpVals[i] = fVal;
    m_pSubProps[i] = new CNtupleNumberSubProp(subNames[i], fVal, i, subDesc[i]);
    m_pSubProps[i]->AllowEdit(!bReadOnly);
    m_pSubProps[i]->eventUpdate += FastDelegate3<float, int, bool>(this, &CUIPropertyGridVector3::onValueChange);
    
    m_pProp->AddSubItem(m_pSubProps[i]);
  }
}

template <typename T1, typename T2>
CUIPropertyGridVector3<T1, T2>::~CUIPropertyGridVector3()
{
}

template <typename T1, typename T2>
void
CUIPropertyGridVector3<T1, T2>::addElement(CMFCPropertyGridProperty* pElement)
{
}

template <typename T1, typename T2>
void
CUIPropertyGridVector3<T1, T2>::propertyChanged()
{
  if (!m_bInRecurse)
  {
    m_bInRecurse = true;

    for (int i = 0; i < 3; i++)
    {
      float fVal = m_fGet(i);

      m_fTmpVals[i] = fVal;
      m_pSubProps[i]->setValue(fVal);
    }
    
    m_bInRecurse = false;
  }
}

template <typename T1, typename T2>
void
CUIPropertyGridVector3<T1, T2>::onValueChange(float fNewVal, int index, bool bSingle)
{
  if (!m_bInRecurse)
  {
    m_fTmpVals[index] = fNewVal;

    if (index == 2 || bSingle)
    {
      m_bInRecurse = true;

      m_fSet(m_fTmpVals[0], m_fTmpVals[1], m_fTmpVals[2]);
      
      m_bInRecurse = false;
    }
  }
}



/////////////////////////////////////////////////////////////////////////////////////////////
//  CUIPropertyGridQuaternion
/////////////////////////////////////////////////////////////////////////////////////////////

template <typename T1, typename T2>
CUIPropertyGridQuaternion<T1, T2>::CUIPropertyGridQuaternion(CUIPropertyGridElement* pParent, const std::string& sName, T1 fGet, T2 fSet, bool bReadOnly)
{
  m_bInRecurse = false;
  m_fGet = fGet;
  m_fSet = fSet;

  m_pProp = new CNtupleProp(sName.c_str(), NULL, true);
  m_pProp->AllowEdit(!bReadOnly);

  pParent->addElement(m_pProp);
  
  const char* subNames[4] = { "W", "X", "Y", "Z" };
  const char* subDesc[4] = { "W Component", "X Component", "Y Component", "Z Component" };

  for (int i = 0; i < 4; i++)
  {
    float fVal = fGet(i);
    m_fTmpVals[i] = fVal;
    m_pSubProps[i] = new CNtupleNumberSubProp(subNames[i], fVal, i, subDesc[i]);
    m_pSubProps[i]->AllowEdit(!bReadOnly);
    m_pSubProps[i]->eventUpdate += FastDelegate3<float, int, bool>(this, &CUIPropertyGridQuaternion::onValueChange);
    
    m_pProp->AddSubItem(m_pSubProps[i]);
  }
}

template <typename T1, typename T2>
CUIPropertyGridQuaternion<T1, T2>::~CUIPropertyGridQuaternion()
{
}

template <typename T1, typename T2>
void
CUIPropertyGridQuaternion<T1, T2>::addElement(CMFCPropertyGridProperty* pElement)
{
}

template <typename T1, typename T2>
void
CUIPropertyGridQuaternion<T1, T2>::propertyChanged()
{
  if (!m_bInRecurse)
  {
    m_bInRecurse = true;

    for (int i = 0; i < 4; i++)
    {
      float fVal = m_fGet(i);

      m_fTmpVals[i] = fVal;
      m_pSubProps[i]->setValue(fVal);
    }
    
    m_bInRecurse = false;
  }
}

template <typename T1, typename T2>
void
CUIPropertyGridQuaternion<T1, T2>::onValueChange(float fNewVal, int index, bool bSingle)
{
  if (!m_bInRecurse)
  {
    m_fTmpVals[index] = fNewVal;

    if (index == 3 || bSingle)
    {
      m_bInRecurse = true;

      m_fSet(m_fTmpVals[0], m_fTmpVals[1], m_fTmpVals[2], m_fTmpVals[3]);
      
      m_bInRecurse = false;
    }
  }
}



/////////////////////////////////////////////////////////////////////////////////////////////
//  CUIPropertyGridComboBox
/////////////////////////////////////////////////////////////////////////////////////////////

template <typename T1, typename T2>
CUIPropertyGridComboBox<T1, T2>::CUIPropertyGridComboBox(CUIPropertyGridElement* pParent, const std::string& sName, const std::string& sDesc, T1 fGet, T2 fSet, bool bReadOnly)
{
  m_bInRecurse = false;
  m_fGet = fGet;
  m_fSet = fSet;

  m_pProp = new CComboBoxExProp(sName.c_str(), fGet(-1), sDesc.c_str());
  m_pProp->AllowEdit(!bReadOnly);
  m_pProp->eventUpdate += FastDelegate2<const std::string&, int>(this, &CUIPropertyGridComboBox::onValueChange);
  
  const char* pText = NULL;
  for (int i = 0; (pText = fGet(i)) != NULL; i++)
    m_pProp->AddOption(pText);

  pParent->addElement(m_pProp);
}

template <typename T1, typename T2>
CUIPropertyGridComboBox<T1, T2>::~CUIPropertyGridComboBox()
{
}

template <typename T1, typename T2>
void
CUIPropertyGridComboBox<T1, T2>::addElement(CMFCPropertyGridProperty* pElement)
{
}

template <typename T1, typename T2>
void
CUIPropertyGridComboBox<T1, T2>::propertyChanged()
{
  if (!m_bInRecurse)
  {
    m_bInRecurse = true;
    m_pProp->update(m_fGet);
    m_bInRecurse = false;
  }
}

template <typename T1, typename T2>
void
CUIPropertyGridComboBox<T1, T2>::onValueChange(const std::string& fNewVal, int index)
{
  if (!m_bInRecurse)
  {
    m_bInRecurse = true;
    m_fSet(fNewVal, index);
    m_bInRecurse = false;
  }
}