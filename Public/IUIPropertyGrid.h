#ifndef IUI_PROPERTY_GRID_H
# define IUI_PROPERTY_GRID_H

# include <functional>
# include <stack>
# include <stdarg.h>

interface_class IUIPropertyGridElement;

interface_class IUIPropertyGrid
{
public:
  virtual void                      setPropertyHolder(IPropertyHolder* pPropObj) = 0;

public:
  virtual IUIPropertyGridElement*   createRoot() = 0;
};

interface_class IUIPropertyGridElement
{
public:
  virtual IUIPropertyGridElement*   addGroup(const std::string& sName) = 0;

  virtual IUIPropertyGridElement*   addBool(const std::string& sName, const std::string& sDesc, std::function<bool()> fGet, std::function<void(bool)> fSet, bool bReadOnly = false) = 0;
  virtual IUIPropertyGridElement*   addBool(const std::string& sName, const std::string& sDesc, std::function<bool()> fGet, FastDelegate1<bool> fSet, bool bReadOnly = false) = 0;
  virtual IUIPropertyGridElement*   addBool(const std::string& sName, const std::string& sDesc, FastDelegate0<bool> fGet, std::function<void(bool)> fSet, bool bReadOnly = false) = 0;
  virtual IUIPropertyGridElement*   addBool(const std::string& sName, const std::string& sDesc, FastDelegate0<bool> fGet, FastDelegate1<bool> fSet, bool bReadOnly = false) = 0;

  virtual IUIPropertyGridElement*   addInt(const std::string& sName, const std::string& sDesc, std::function<int()> fGet, std::function<void(int)> fSet, bool bReadOnly = false, int min = -0xFFFFFFF, int max = 0xFFFFFFF) = 0;
  virtual IUIPropertyGridElement*   addInt(const std::string& sName, const std::string& sDesc, std::function<int()> fGet, FastDelegate1<int> fSet, bool bReadOnly = false, int min = -0xFFFFFFF, int max = 0xFFFFFFF) = 0;
  virtual IUIPropertyGridElement*   addInt(const std::string& sName, const std::string& sDesc, FastDelegate0<int> fGet, std::function<void(int)> fSet, bool bReadOnly = false, int min = -0xFFFFFFF, int max = 0xFFFFFFF) = 0;
  virtual IUIPropertyGridElement*   addInt(const std::string& sName, const std::string& sDesc, FastDelegate0<int> fGet, FastDelegate1<int> fSet, bool bReadOnly = false, int min = -0xFFFFFFF, int max = 0xFFFFFFF) = 0;
  
  virtual IUIPropertyGridElement*   addFloat(const std::string& sName, const std::string& sDesc, std::function<float()> fGet, std::function<void(float)> fSet, bool bReadOnly = false, float min = -std::numeric_limits<float>::max(), float max = std::numeric_limits<float>::max()) = 0;
  virtual IUIPropertyGridElement*   addFloat(const std::string& sName, const std::string& sDesc, std::function<float()> fGet, FastDelegate1<float> fSet, bool bReadOnly = false, float min = -std::numeric_limits<float>::max(), float max = std::numeric_limits<float>::max()) = 0;
  virtual IUIPropertyGridElement*   addFloat(const std::string& sName, const std::string& sDesc, FastDelegate0<float> fGet, std::function<void(float)> fSet, bool bReadOnly = false, float min = -std::numeric_limits<float>::max(), float max = std::numeric_limits<float>::max()) = 0;
  virtual IUIPropertyGridElement*   addFloat(const std::string& sName, const std::string& sDesc, FastDelegate0<float> fGet, FastDelegate1<float> fSet, bool bReadOnly = false, float min = -std::numeric_limits<float>::max(), float max = std::numeric_limits<float>::max()) = 0;
  
  virtual IUIPropertyGridElement*   addString(const std::string& sName, const std::string& sDesc, std::function<const std::string&()> fGet, std::function<void(const std::string&)> fSet, bool bReadOnly = false) = 0;
  virtual IUIPropertyGridElement*   addString(const std::string& sName, const std::string& sDesc, std::function<const std::string&()> fGet, FastDelegate1<const std::string&> fSet, bool bReadOnly = false) = 0;
  virtual IUIPropertyGridElement*   addString(const std::string& sName, const std::string& sDesc, FastDelegate0<const std::string&> fGet, std::function<void(const std::string&)> fSet, bool bReadOnly = false) = 0;
  virtual IUIPropertyGridElement*   addString(const std::string& sName, const std::string& sDesc, FastDelegate0<const std::string&> fGet, FastDelegate1<const std::string&> fSet, bool bReadOnly = false) = 0;

  virtual IUIPropertyGridElement*   addNTuple(const std::string& sName, std::function<float(int)> fGet, std::function<void(float*)> fSet, bool bReadOnly, int nNum, ...) = 0;
  virtual IUIPropertyGridElement*   addNTuple(const std::string& sName, std::function<float(int)> fGet, FastDelegate1<float*> fSet, bool bReadOnly, int nNum, ...) = 0;
  virtual IUIPropertyGridElement*   addNTuple(const std::string& sName, FastDelegate1<int, float> fGet, std::function<void(float*)> fSet, bool bReadOnly, int nNum, ...) = 0;
  virtual IUIPropertyGridElement*   addNTuple(const std::string& sName, FastDelegate1<int, float> fGet, FastDelegate1<float*> fSet, bool bReadOnly, int nNum, ...) = 0;
  
  virtual IUIPropertyGridElement*   addVector2(const std::string& sName, std::function<float(int)> fGet, std::function<void(float, float)> fSet, bool bReadOnly = false) = 0;
  virtual IUIPropertyGridElement*   addVector2(const std::string& sName, std::function<float(int)> fGet, FastDelegate2<float, float> fSet, bool bReadOnly = false) = 0;
  virtual IUIPropertyGridElement*   addVector2(const std::string& sName, FastDelegate1<int, float> fGet, std::function<void(float, float)> fSet, bool bReadOnly = false) = 0;
  virtual IUIPropertyGridElement*   addVector2(const std::string& sName, FastDelegate1<int, float> fGet, FastDelegate2<float, float> fSet, bool bReadOnly = false) = 0;

  virtual IUIPropertyGridElement*   addVector3(const std::string& sName, std::function<float(int)> fGet, std::function<void(float, float, float)> fSet, bool bReadOnly = false) = 0;
  virtual IUIPropertyGridElement*   addVector3(const std::string& sName, std::function<float(int)> fGet, FastDelegate3<float, float, float> fSet, bool bReadOnly = false) = 0;
  virtual IUIPropertyGridElement*   addVector3(const std::string& sName, FastDelegate1<int, float> fGet, std::function<void(float, float, float)> fSet, bool bReadOnly = false) = 0;
  virtual IUIPropertyGridElement*   addVector3(const std::string& sName, FastDelegate1<int, float> fGet, FastDelegate3<float, float, float> fSet, bool bReadOnly = false) = 0;

  virtual IUIPropertyGridElement*   addQuaternion(const std::string& sName, std::function<float(int)> fGet, std::function<void(float, float, float, float)> fSet, bool bReadOnly = false) = 0;
  virtual IUIPropertyGridElement*   addQuaternion(const std::string& sName, std::function<float(int)> fGet, FastDelegate4<float, float, float, float> fSet, bool bReadOnly = false) = 0;
  virtual IUIPropertyGridElement*   addQuaternion(const std::string& sName, FastDelegate1<int, float> fGet, std::function<void(float, float, float, float)> fSet, bool bReadOnly = false) = 0;
  virtual IUIPropertyGridElement*   addQuaternion(const std::string& sName, FastDelegate1<int, float> fGet, FastDelegate4<float, float, float, float> fSet, bool bReadOnly = false) = 0;

  virtual IUIPropertyGridElement*   addComboBox(const std::string& sName, const std::string& sDesc, std::function<const char*(int)> fGet, std::function<void(const std::string&, int)> fSet, bool bReadOnly = false) = 0;
  virtual IUIPropertyGridElement*   addComboBox(const std::string& sName, const std::string& sDesc, std::function<const char*(int)> fGet, FastDelegate2<const std::string&, int> fSet, bool bReadOnly = false) = 0;
  virtual IUIPropertyGridElement*   addComboBox(const std::string& sName, const std::string& sDesc, FastDelegate1<int, const char*> fGet, std::function<void(const std::string&, int)> fSet, bool bReadOnly = false) = 0;
  virtual IUIPropertyGridElement*   addComboBox(const std::string& sName, const std::string& sDesc, FastDelegate1<int, const char*> fGet, FastDelegate2<const std::string&, int> fSet, bool bReadOnly = false) = 0;

public:
  virtual void                      propertyChanged() = 0;

protected:
  virtual ~IUIPropertyGridElement() {};
};



///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

# define IWE_IMPLEMENT_PROP_BEGIN(ThisClass)                                                                \
  void ThisClass::unselectPropHolder()                                                                      \
  {                                                                                                         \
    for (std::vector<BasePropVar*>::iterator it = __m_propHolderVars.begin();                               \
         it != __m_propHolderVars.end();                                                                    \
         it++)                                                                                              \
      (*it)->eventChanged.clear();                                                                          \
    __m_propHolderVars.clear();                                                                             \
  }                                                                                                         \
  IUIPropertyGridElement* ThisClass::getPropObject(IGUIInterface* pGUI)                                     \
  {                                                                                                         \
    return getPropObject(pGUI->getUIManager()->getPropertyGrid()->createRoot());                            \
  }                                                                                                         \
  IUIPropertyGridElement* ThisClass::getPropObject(IUIPropertyGridElement* pRoot)                           \
  {                                                                                                         \
    ThisClass* pInst = this;                                                                                \
    IUIPropertyGridElement* pLastElement = NULL;                                                            \
    std::stack<IUIPropertyGridElement*> parentStack;                                                        \
    parentStack.push(pRoot);

# define IWE_IMPLEMENT_PROP_OVERRIDE_BEGIN(ThisClass, BaseClass)                                            \
  IUIPropertyGridElement* ThisClass::getPropObject(IUIPropertyGridElement* pRoot)                           \
  {                                                                                                         \
    ThisClass* pInst = this;                                                                                \
    IUIPropertyGridElement* pLastElement = NULL;                                                            \
    std::stack<IUIPropertyGridElement*> parentStack;                                                        \
    pRoot = BaseClass :: getPropObject(pRoot);                                                              \
    parentStack.push(pRoot);

///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

# define IWE_IMPLEMENT_PROP_END()                                                                           \
    return pRoot;                                                                                           \
  }

///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

# define IWE_DEFINE_PROP_HOLDER()                                                                           \
  virtual IUIPropertyGridElement*   getPropObject(IGUIInterface* pGUI);                                     \
  virtual IUIPropertyGridElement*   getPropObject(IUIPropertyGridElement* pRoot);                           \
  virtual void                      unselectPropHolder();                                                   \
  std::vector<BasePropVar*>         __m_propHolderVars;

# define IWE_DEFINE_PROP_HOLDER_OVERRIDE()                                                                  \
  virtual IUIPropertyGridElement*   getPropObject(IUIPropertyGridElement* pRoot);

///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

# define _IWE_PROP_DEF(Name, Description, Var, Func, Getter, Setter, ReadOnly)                              \
    pLastElement = parentStack.top()->Func(Name, Description, Getter, Setter, ReadOnly);                    \
    Var.eventChanged += FastDelegate0<>(pLastElement, &IUIPropertyGridElement::propertyChanged);            \
    __m_propHolderVars.push_back(&Var);

# define IWE_PROP_DEF(Name, Description, Var, Func, Type, ReadOnly)                                         \
  _IWE_PROP_DEF(Name, Description, Var, Func,                                                               \
                                   [pInst]() -> Type { return pInst->Var; },                                \
                                   [pInst](Type newVal) { pInst->Var.Get() = newVal; }, ReadOnly)

# define IWE_PROP_DEF_S(Name, Description, Var, Func, SetterDelegate, Type, ReadOnly)                       \
  _IWE_PROP_DEF(Name, Description, Var, Func,                                                               \
                                   [pInst]() -> Type { return pInst->Var; },                                \
                                   SetterDelegate, ReadOnly)

# define IWE_PROP_DEF_G(Name, Description, Var, Func, GetterDelegate, Type, ReadOnly)                       \
  _IWE_PROP_DEF(Name, Description, Var, Func,                                                               \
                                   GetterDelegate,                                                          \
                                   [pInst](Type newVal) { pInst->Var.Get() = newVal; }, ReadOnly)

///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

# define _IWE_PROP_DEF_BOUND(Name, Description, Var, Func, Getter, Setter, ReadOnly, Min, Max)              \
    pLastElement = parentStack.top()->Func(Name, Description, Getter, Setter, ReadOnly, Min, Max);          \
    Var.eventChanged += FastDelegate0<>(pLastElement, &IUIPropertyGridElement::propertyChanged);            \
    __m_propHolderVars.push_back(&Var);

# define IWE_PROP_DEF_BOUND(Name, Description, Var, Func, Type, ReadOnly, Min, Max)                         \
  _IWE_PROP_DEF_BOUND(Name, Description, Var, Func,                                                         \
                                   [pInst]() -> Type { return pInst->Var; },                                \
                                   [pInst](Type newVal) { pInst->Var.Get() = newVal; }, ReadOnly, Min, Max)

# define IWE_PROP_DEF_BOUND_S(Name, Description, Var, Func, SetterDelegate, Type, ReadOnly, Min, Max)       \
  _IWE_PROP_DEF_BOUND(Name, Description, Var, Func,                                                         \
                                   [pInst]() -> Type { return pInst->Var; },                                \
                                   SetterDelegate, ReadOnly, Min, Max)

# define IWE_PROP_DEF_BOUND_G(Name, Description, Var, Func, GetterDelegate, Type, ReadOnly, Min, Max)       \
  _IWE_PROP_DEF_BOUND(Name, Description, Var, Func,                                                         \
                                   GetterDelegate,                                                          \
                                   [pInst](Type newVal) { pInst->Var.Get() = newVal; }, ReadOnly, Min, Max)

///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

# define IWE_PROP_HOLDER_OBJ(Obj)                                                                           \
    pLastElement = Obj->getPropObject(parentStack.top());

///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

# define IWE_PROP_GROUP(Name)                                                                               \
    pLastElement = parentStack.top()->addGroup(Name);

///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

# define IWE_PROP_LEVEL_BEGIN(Name)                                                                         \
    IWE_PROP_GROUP(Name)                                                                                    \
    {                                                                                                       \
      parentStack.push(pLastElement);

# define IWE_PROP_LEVEL_END()                                                                               \
      parentStack.pop();                                                                                    \
    }

///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

# define IWE_PROP_BOOL(Name, Description, Var, ReadOnly)                                                    \
  IWE_PROP_DEF(Name, Description, Var, addBool, bool, ReadOnly)

# define IWE_PROP_BOOL_S(Name, Description, Var, SetterDelegate, ReadOnly)                                  \
  IWE_PROP_DEF_S(Name, Description, Var, addBool, SetterDelegate, bool, ReadOnly)

# define IWE_PROP_BOOL_G(Name, Description, Var, GetterDelegate, ReadOnly)                                  \
  IWE_PROP_DEF_G(Name, Description, Var, addBool, GetterDelegate, bool, ReadOnly)

# define IWE_PROP_BOOL_GS(Name, Description, Var, GetterDelegate, SetterDelegate, ReadOnly)                 \
  _IWE_PROP_DEF(Name, Description, Var, addBool, GetterDelegate, SetterDelegate, ReadOnly)

///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

# define IWE_PROP_INT(Name, Description, Var, ReadOnly)                                                     \
  IWE_PROP_DEF(Name, Description, Var, addInt, int, ReadOnly)

# define IWE_PROP_INT_S(Name, Description, Var, SetterDelegate, ReadOnly)                                   \
  IWE_PROP_DEF_S(Name, Description, Var, addInt, SetterDelegate, int, ReadOnly)

# define IWE_PROP_INT_G(Name, Description, Var, GetterDelegate, ReadOnly)                                   \
  IWE_PROP_DEF_G(Name, Description, Var, addInt, GetterDelegate, int, ReadOnly)

# define IWE_PROP_INT_GS(Name, Description, Var, GetterDelegate, SetterDelegate, ReadOnly)                  \
  _IWE_PROP_DEF(Name, Description, Var, addInt, GetterDelegate, SetterDelegate, ReadOnly)

///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

# define IWE_PROP_FLOAT(Name, Description, Var, ReadOnly)                                                   \
  IWE_PROP_DEF(Name, Description, Var, addFloat, float, ReadOnly)

# define IWE_PROP_FLOAT_S(Name, Description, Var, SetterDelegate, ReadOnly)                                 \
  IWE_PROP_DEF_S(Name, Description, Var, addFloat, SetterDelegate, float, ReadOnly)

# define IWE_PROP_FLOAT_G(Name, Description, Var, GetterDelegate, ReadOnly)                                 \
  IWE_PROP_DEF_G(Name, Description, Var, addFloat, GetterDelegate, float, ReadOnly)

# define IWE_PROP_FLOAT_GS(Name, Description, Var, GetterDelegate, SetterDelegate, ReadOnly)                \
  _IWE_PROP_DEF(Name, Description, Var, addFloat, GetterDelegate, SetterDelegate, ReadOnly)

///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

# define IWE_PROP_INT_BOUND(Name, Description, Var, ReadOnly, Min, Max)                                     \
  IWE_PROP_DEF(Name, Description, Var, addInt, int, ReadOnly, Min, Max)

# define IWE_PROP_INT_BOUND_S(Name, Description, Var, SetterDelegate, ReadOnly, Min, Max)                   \
  IWE_PROP_DEF_S(Name, Description, Var, addInt, SetterDelegate, int, ReadOnly, Min, Max)

# define IWE_PROP_INT_BOUND_G(Name, Description, Var, GetterDelegate, ReadOnly, Min, Max)                   \
  IWE_PROP_DEF_G(Name, Description, Var, addInt, GetterDelegate, int, ReadOnly, Min, Max)

# define IWE_PROP_INT_BOUND_GS(Name, Description, Var, GetterDelegate, SetterDelegate, ReadOnly, Min, Max)  \
  _IWE_PROP_DEF(Name, Description, Var, addInt, GetterDelegate, SetterDelegate, ReadOnly, Min, Max)

///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

# define IWE_PROP_FLOAT_BOUND(Name, Description, Var, ReadOnly, Min, Max)                                   \
  IWE_PROP_DEF_BOUND(Name, Description, Var, addFloat, float, ReadOnly, Min, Max)

# define IWE_PROP_FLOAT_BOUND_S(Name, Description, Var, SetterDelegate, ReadOnly, Min, Max)                 \
  IWE_PROP_DEF_BOUND_S(Name, Description, Var, addFloat, SetterDelegate, float, ReadOnly, Min, Max)

# define IWE_PROP_FLOAT_BOUND_G(Name, Description, Var, GetterDelegate, ReadOnly, Min, Max)                 \
  IWE_PROP_DEF_BOUND_G(Name, Description, Var, addFloat, GetterDelegate, float, ReadOnly, Min, Max)

# define IWE_PROP_FLOAT_BOUND_GS(Name, Description, Var, GetterDelegate, SetterDelegate, ReadOnly, Min, Max)\
  _IWE_PROP_DEF_BOUND(Name, Description, Var, addFloat, GetterDelegate, SetterDelegate, ReadOnly, Min, Max)

///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

# define IWE_PROP_STRING(Name, Description, Var, ReadOnly)                                                  \
  IWE_PROP_DEF(Name, Description, Var, addString, const std::string&, ReadOnly)

# define IWE_PROP_STRING_S(Name, Description, Var, SetterDelegate, ReadOnly)                                \
  IWE_PROP_DEF_S(Name, Description, Var, addString, SetterDelegate, const std::string&, ReadOnly)

# define IWE_PROP_STRING_G(Name, Description, Var, GetterDelegate, ReadOnly)                                \
  IWE_PROP_DEF_G(Name, Description, Var, addString, GetterDelegate, const std::string&, ReadOnly)

# define IWE_PROP_STRING_GS(Name, Description, Var, GetterDelegate, SetterDelegate, ReadOnly)               \
  _IWE_PROP_DEF(Name, Description, Var, addString, GetterDelegate, SetterDelegate, ReadOnly)

///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

# define _IWE_PROP_NTUPLE(Name, Var, Getter, Setter, ReadOnly, Num, ...)                                    \
    pLastElement = parentStack.top()->addNTuple(Name, Getter, Setter, ReadOnly, Num, __VA_ARGS__);          \
    Var.eventChanged += FastDelegate0<>(pLastElement, &IUIPropertyGridElement::propertyChanged);            \
    __m_propHolderVars.push_back(&Var);

# define IWE_PROP_NTUPLE(Name, Var, ReadOnly, Num, ...)                                                     \
  _IWE_PROP_NTUPLE(Name, Var, [pInst](int i) -> float { return pInst->Var.Get()[i]; },                      \
    [pInst](float* f) { for (int i = 0; i < Num; i++) pInst->Var.Get()[i] = f[i]; },                        \
    ReadOnly, Num, __VA_ARGS__, std::numeric_limits<float>::max())

# define IWE_PROP_NTUPLE_S(Name, Var, SetterDelegate, ReadOnly, Num, ...)                                   \
  _IWE_PROP_NTUPLE(Name, Var, [pInst](int i) -> float { return pInst->Var.Get()[i]; },                      \
                          SetterDelegate, ReadOnly, Num, __VA_ARGS__, std::numeric_limits<float>::max())

# define IWE_PROP_NTUPLE_G(Name, Var, GetterDelegate, ReadOnly, Num, ...)                                   \
  _IWE_PROP_NTUPLE(Name, Var, GetterDelegate,                                                               \
    [pInst](float* f) { for (int i = 0; i < Num; i++) pInst->Var.Get()[i] = f[i]; },                        \
    ReadOnly, Num, __VA_ARGS__, std::numeric_limits<float>::max())

# define IWE_PROP_NTUPLE_GS(Name, Var, GetterDelegate, SetterDelegate, ReadOnly, Num, ...)                  \
  _IWE_PROP_NTUPLE(Name, Description, GetterDelegate, SetterDelegate, ReadOnly, Num, __VA_ARGS__,           \
    std::numeric_limits<float>::max())

///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

# define _IWE_PROP_VECTOR2(Name, Var, Getter, Setter, ReadOnly)                                             \
    pLastElement = parentStack.top()->addVector2(Name, Getter, Setter, ReadOnly);                           \
    Var.eventChanged += FastDelegate0<>(pLastElement, &IUIPropertyGridElement::propertyChanged);            \
    __m_propHolderVars.push_back(&Var);

# define IWE_PROP_VECTOR2(Name, Var, ReadOnly)                                                              \
  _IWE_PROP_VECTOR2(Name, Var, [pInst](int i) -> float { return pInst->Var.Get()[i]; },                     \
    [pInst](float fX, float fY) {                                                                           \
      pInst->Var.Get()[0] = fX; pInst->Var.Get()[1] = fY; }, ReadOnly)

# define IWE_PROP_VECTOR2_S(Name, Var, SetterDelegate, ReadOnly)                                            \
  _IWE_PROP_VECTOR2(Name, Var, [pInst](int i) -> float { return pInst->Var.Get()[i]; },                     \
                          SetterDelegate, ReadOnly)

# define IWE_PROP_VECTOR2_G(Name, Var, GetterDelegate, ReadOnly)                                            \
  _IWE_PROP_VECTOR2(Name, Var, GetterDelegate,                                                              \
    [pInst](float fX, float fY) {                                                                           \
      pInst->Var.Get()[0] = fX; pInst->Var.Get()[1] = fY; }, ReadOnly)

# define IWE_PROP_VECTOR2_GS(Name, Var, GetterDelegate, SetterDelegate, ReadOnly)                           \
  _IWE_PROP_VECTOR2(Name, Description, GetterDelegate, SetterDelegate, ReadOnly)

///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

# define _IWE_PROP_VECTOR3(Name, Var, Getter, Setter, ReadOnly)                                             \
    pLastElement = parentStack.top()->addVector3(Name, Getter, Setter, ReadOnly);                           \
    Var.eventChanged += FastDelegate0<>(pLastElement, &IUIPropertyGridElement::propertyChanged);            \
    __m_propHolderVars.push_back(&Var);

# define IWE_PROP_VECTOR3(Name, Var, ReadOnly)                                                              \
  _IWE_PROP_VECTOR3(Name, Var, [pInst](int i) -> float { return pInst->Var.Get()[i]; },                     \
    [pInst](float fX, float fY, float fZ) {                                                                 \
      pInst->Var.Get()[0] = fX; pInst->Var.Get()[1] = fY;                                                   \
      pInst->Var.Get()[2] = fZ; }, ReadOnly)

# define IWE_PROP_VECTOR3_S(Name, Var, SetterDelegate, ReadOnly)                                            \
  _IWE_PROP_VECTOR3(Name, Var, [pInst](int i) -> float { return pInst->Var.Get()[i]; },                     \
                          SetterDelegate, ReadOnly)

# define IWE_PROP_VECTOR3_G(Name, Var, GetterDelegate, ReadOnly)                                            \
  _IWE_PROP_VECTOR3(Name, Var, GetterDelegate,                                                              \
    [pInst](float fX, float fY, float fZ) {                                                                 \
      pInst->Var.Get()[0] = fX; pInst->Var.Get()[1] = fY;                                                   \
      pInst->Var.Get()[2] = fZ; }, ReadOnly)

# define IWE_PROP_VECTOR3_GS(Name, Var, GetterDelegate, SetterDelegate, ReadOnly)                           \
  _IWE_PROP_VECTOR3(Name, Description, GetterDelegate, SetterDelegate, ReadOnly)

///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

# define _IWE_PROP_QUATERNION(Name, Var, Getter, Setter, ReadOnly)                                          \
    pLastElement = parentStack.top()->addQuaternion(Name, Getter, Setter, ReadOnly);                        \
    Var.eventChanged += FastDelegate0<>(pLastElement, &IUIPropertyGridElement::propertyChanged);            \
    __m_propHolderVars.push_back(&Var);

# define IWE_PROP_QUATERNION(Name, Var, ReadOnly)                                                           \
  _IWE_PROP_QUATERNION(Name, Var, [pInst](int i) -> float { return pInst->Var.Get()[i]; },                  \
    [pInst](float fW, float fX, float fY, float fZ) {                                                       \
      pInst->Var.Get()[0] = fW; pInst->Var.Get()[1] = fX;                                                   \
      pInst->Var.Get()[2] = fY; pInst->Var.Get()[3] = fZ; }, ReadOnly)

# define IWE_PROP_QUATERNION_S(Name, Var, SetterDelegate, ReadOnly)                                         \
  _IWE_PROP_QUATERNION(Name, Var, [pInst](int i) -> float { return pInst->Var.Get()[i]; },                  \
                          SetterDelegate, ReadOnly)

# define IWE_PROP_QUATERNION_G(Name, Var, GetterDelegate, ReadOnly)                                         \
  _IWE_PROP_QUATERNION(Name, Var, GetterDelegate,                                                           \
    [pInst](float fW, float fX, float fY, float fZ) {                                                       \
      pInst->Var.Get()[0] = fW; pInst->Var.Get()[1] = fX;                                                   \
      pInst->Var.Get()[2] = fY; pInst->Var.Get()[3] = fZ; }, ReadOnly)

# define IWE_PROP_QUATERNION_GS(Name, Var, GetterDelegate, SetterDelegate, ReadOnly)                        \
  _IWE_PROP_QUATERNION(Name, Description, GetterDelegate, SetterDelegate, ReadOnly)

///////////////////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////////////////

# define _IWE_PROP_COMBO(Name, Description, Var, Var2, Getter, Setter, ReadOnly)                            \
    pLastElement = parentStack.top()->addComboBox(Name, Description, Getter, Setter, ReadOnly);             \
    Var.eventChanged += FastDelegate0<>(pLastElement, &IUIPropertyGridElement::propertyChanged);            \
    Var2.eventChanged += FastDelegate0<>(pLastElement, &IUIPropertyGridElement::propertyChanged);           \
    __m_propHolderVars.push_back(&Var);                                                                     \
    __m_propHolderVars.push_back(&Var2);

# define IWE_PROP_COMBO(Name, Description, Var, Var2, ReadOnly)                                             \
  _IWE_PROP_COMBO(Name, Description, Var, Var2,                                                             \
    [pInst](int index) -> const char* {                                                                     \
      if (index < 0) return pInst->Var2.Get().c_str();                                                      \
      else if (index >= pInst->Var.Get().size()) return NULL;                                               \
      return pInst->Var.Get()[index].c_str();                                                               \
    },                                                                                                      \
    [pInst](const std::string& sNewVal, int index) {                                                        \
      if (index < 0) pInst->Var2 = sNewVal; }, ReadOnly)

# define IWE_PROP_COMBO_S(Name, Description, Var, Var2, SetterDelegate, ReadOnly)                           \
  _IWE_PROP_COMBO(Name, Description, Var, Var2,                                                             \
    [pInst](int index) -> const char* {                                                                     \
      if (index < 0) return pInst->Var2.Get().c_str();                                                      \
      else if (index >= pInst->Var.Get().size()) return NULL;                                               \
      return pInst->Var.Get()[index].c_str();                                                               \
    }, SetterDelegate, ReadOnly)

# define IWE_PROP_COMBO_G(Name, Description, Var, Var2, GetterDelegate, ReadOnly)                           \
  _IWE_PROP_COMBO(Name, Description, Var, Var2, GetterDelegate                                              \
    [pInst](const std::string& sNewVal, int index) {                                                        \
      if (index < 0) pInst->Var2 = sNewVal; }, ReadOnly)

# define IWE_PROP_COMBO_GS(Name, Description, Var, Var2, GetterDelegate, SetterDelegate, ReadOnly)          \
  _IWE_PROP_COMBO(Name, Description, Var, Var2, GetterDelegate, SetterDelegate, ReadOnly)


#endif // !UIPropertyGrid