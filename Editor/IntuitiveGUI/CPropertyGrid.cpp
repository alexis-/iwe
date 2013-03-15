#include "stdafx.h"

#include "CPropertyGrid.h"
#include "MainFrm.h"

/* The __VA_NARGS__ macro returns the number of arguments that have been
  * passed to it.
  */

#include <exception>
#include <stack>

#ifndef __VA_NARGS__
# define __VA_NARGS__(...) \
         __VA_NARGS___(__VA_ARGS__,__VA_NARGS_RSEQ_N())
# define __VA_NARGS___(...) \
         __VA_NARGS__N(__VA_ARGS__)
# define __VA_NARGS__N(\
          _1, _2, _3, _4, _5, _6, _7, _8, _9,_10, \
         _11,_12,_13,_14,_15,_16,_17,_18,_19,_20, \
         _21,_22,_23,_24,_25,_26,_27,_28,_29,_30, \
         _31,_32,_33,_34,_35,_36,_37,_38,_39,_40, \
         _41,_42,_43,_44,_45,_46,_47,_48,_49,_50, \
         _51,_52,_53,_54,_55,_56,_57,_58,_59,_60, \
         _61,_62,_63,N,...) N
# define __VA_NARGS_RSEQ_N() \
         63,62,61,60,                   \
         59,58,57,56,55,54,53,52,51,50, \
         49,48,47,46,45,44,43,42,41,40, \
         39,38,37,36,35,34,33,32,31,30, \
         29,28,27,26,25,24,23,22,21,20, \
         19,18,17,16,15,14,13,12,11,10, \
         9,8,7,6,5,4,3,2,1,0 
#endif // !__VA_NARGS__

# define __MFC_PROP_GRID_CREATE_ACCESSOR(ClassName, Variable, Ret)                                \
  [](ClassName* pInst) -> Ret {                                                                   \
    if (!pInst)                                                                                   \
      throw std::exception("NULL pointer while trying to access " #ClassName "::" #Variable);     \
    return pInst->Variable;                                                                       \
  }

//# define __MFC_PROP_GRID_PARENT                                                                   \
//  static void _prop_grid_addToParent(std::stack<CMFCPropertyGridProperty*>& stack,                \
//                                     CMFCPropertyGridCtrl&                  grid,                 \
//                                     CMFCPropertyGridProperty*              item)                 \
//  {                                                                                               \
//    if (stack.empty())                                                                            \
//      grid.AddProperty(item);                                                                     \
//    else                                                                                          \
//      stack.top()->AddSubItem(item);                                                              \
//  }

template <typename T>
class CMFCProp
{
  typedef CMFCProp<T> MyT;

public:
  CMFCProp() {}
  CMFCProp(const T& other) { m_data = other; }
  CMFCProp(const MyT& other) { m_data = other.m_data; }

public:
  T& operator=(const MyT& other) { m_data = other.m_data; return get(); }
  T& operator=(const T& other) { m_data = other; return get(); }
  bool operator==(const T& other) { m_data == other; }

public:

  T& get()
  {
    return m_data;
  }

private:
  T        m_data;
};

template <typename T>
class CMFCProp<T*>
{
  typedef CMFCProp<T*> MyT;

public:
  CMFCProp() {}
  CMFCProp(T* other) { m_data = other; }
  CMFCProp(const MyT& other) { m_data = other.m_data; }

public:
  T* operator=(const MyT& other) { m_data = other.m_data; return get(); }
  T* operator=(T* other) { m_data = other; return get(); }
  bool operator==(T* other) { m_data == other; }

public:
	T& operator*() const
	{
	  return (*get());
	}

	T *operator->() const
	{
	  return (get());
	}

  T *get() const
  {
    return m_data;
  }

private:
  T*       m_data;
};

class IMFCPropertyGridHelperProp
{
public:
  virtual void onSet() = 0;
};

class CMFCPropertyGridClient
{
protected:
  //inline void _prop_grid_addItem(std::stack<void*>& stack, IMFCPropertyGridHelperProp* item);

private:
  std::vector<IMFCPropertyGridHelperProp*> m_vecRootProps;
};

//void
//CMFCPropertyGridClient::_prop_grid_addItem(std::stack<void*>& stack, IMFCPropertyGridHelperProp* item)
//{
//  if (stack.empty())
//    m_vecRootProps.push_back(item);
//
//  stack.push(item);
//}

class CMFCPropertyGridHelperCategory
{

};

# define PROP_GRID_ROOT(ClassName)                                                                \
  ClassName::_prop_grid_init(/*CMFCPropertyGridCtrl* grid*/) {

# define PROP_GRID_ROOT_                                                                          \
  }

//# define PROP_GRID_CATEGORY(


CPropertyGrid::CPropertyGrid(CMFCPropertyGridCtrl* propGrid)
{
  if (!propGrid)
    throw std::exception("propGrid is NULL");

  m_propGrid = propGrid;
}

