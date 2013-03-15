#ifndef CPROPERTY_GRID_CUSTOM_CTRL
# define CPROPERTY_GRID_CUSTOM_CTRL





class CIntProp : public CMFCPropertyGridProperty
{
public:
	CIntProp(const CString& strName, int n, int min, int max, LPCTSTR lpszDescr = NULL, DWORD dwData = 0);
  
	virtual BOOL OnUpdateValue();
  bool checkBounds(int& i);
  void setValue(int i);

public:
  Event1<int>         eventUpdate;

private:
	int m_iMinValue;
	int m_iMaxValue;
};


class CBoolProp : public CMFCPropertyGridProperty
{
public:
	CBoolProp(const CString& strName, bool b, LPCTSTR lpszDescr = NULL, DWORD dwData = 0);
  
	virtual BOOL OnUpdateValue();
  void setValue(bool b);

public:
  Event1<bool>         eventUpdate;

private:
  bool m_bIgnore;
};


class CFloatProp : public CMFCPropertyGridProperty
{
public:
	CFloatProp(const CString& strName, float f, float min, float max, LPCTSTR lpszDescr = NULL, DWORD dwData = 0);
  
	virtual BOOL OnUpdateValue();
  bool checkBounds(float& i);
  void setValue(float f);

public:
  Event1<float>        eventUpdate;

private:
	float m_fMinValue;
	float m_fMaxValue;
};


class CStringProp : public CMFCPropertyGridProperty
{
public:
	CStringProp(const CString& strName, const std::string& sStr, LPCTSTR lpszDescr = NULL, DWORD dwData = 0);
  
	virtual BOOL OnUpdateValue();
  void setValue(const std::string& sStr);

public:
  Event1<const char*>        eventUpdate;
};


//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////

class CCheckBoxProp : public CMFCPropertyGridProperty
{
public:
	CCheckBoxProp(const CString& strName, BOOL bCheck, LPCTSTR lpszDescr = NULL, DWORD dwData = 0);

protected:
	virtual BOOL OnEdit(LPPOINT /*lptClick*/) { return FALSE; }
	virtual void OnDrawButton(CDC* /*pDC*/, CRect /*rectButton*/) {}
	virtual void OnDrawValue(CDC* /*pDC*/, CRect /*rect*/) {}
	virtual BOOL HasButton() const { return FALSE; }

	virtual BOOL PushChar(UINT nChar);
	virtual void OnDrawCheckBox(CDC * pDC, CRect rectCheck, BOOL bChecked);
	virtual void OnDrawName(CDC* pDC, CRect rect);
	virtual void OnClickName(CPoint point);
	virtual BOOL OnDblClk(CPoint point);

protected:
	CRect m_rectCheck;
};


//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////

class CPasswordProp : public CMFCPropertyGridProperty
{
public:
	CPasswordProp(const CString& strName, const CString& strPassword, LPCTSTR lpszDescr = NULL, DWORD dwData = 0);

protected:
	virtual CWnd* CreateInPlaceEdit(CRect rectEdit, BOOL& bDefaultFormat);
	virtual CString FormatProperty();
};


//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////

class CSliderProp : public CMFCPropertyGridProperty
{
public:
	CSliderProp(const CString& strName, long nValue, LPCTSTR lpszDescr = NULL, DWORD dwData = 0);

	virtual BOOL OnUpdateValue();

protected:
	virtual CWnd* CreateInPlaceEdit(CRect rectEdit, BOOL& bDefaultFormat);
	virtual BOOL OnSetCursor() const { return FALSE; /* Use default */ }
};


//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////

class CNtupleProp : public CMFCPropertyGridProperty
{
public:
	CNtupleProp(const CString& strGroupName, DWORD_PTR dwData = 0, BOOL bIsValueList = FALSE);

	virtual BOOL OnUpdateValue();
};

class CNtupleNumberSubProp : public CMFCPropertyGridProperty
{
public:
	CNtupleNumberSubProp(const CString& strName, float varValue, int index, LPCTSTR lpszDescr = NULL, float min = -std::numeric_limits<float>::max(), float max = std::numeric_limits<float>::max());

	virtual BOOL OnUpdateValue();

  bool checkBounds(float& i);
  void checkUpdate(float f);
  void setValue(float f);

public:
  Event3<float, int, bool>      eventUpdate;

private:
	float m_fMinValue;
	float m_fMaxValue;
  int m_iIndex;
};


//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
// CPropSliderCtrl window

class CPropSliderCtrl : public CSliderCtrl
{
// Construction
public:
	CPropSliderCtrl(CSliderProp* pProp, COLORREF clrBack);

// Attributes
protected:
	CBrush m_brBackground;
	COLORREF m_clrBack;
	CSliderProp* m_pProp;

// Implementation
public:
	virtual ~CPropSliderCtrl();

protected:
	//{{AFX_MSG(CPropSliderCtrl)
	afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
	//}}AFX_MSG
	afx_msg void HScroll(UINT nSBCode, UINT nPos);

	DECLARE_MESSAGE_MAP()
};


//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////

class CIconListProp : public CMFCPropertyGridProperty
{
public:
	CIconListProp(const CString& strName, const CImageList& imageListIcons, int nSelectedIcon, CStringList* plstIconNames = NULL, LPCTSTR lpszDescr = NULL, DWORD dwData = 0);

protected:
	virtual CWnd* CreateInPlaceEdit(CRect rectEdit, BOOL& bDefaultFormat);
	virtual CComboBox* CreateCombo(CWnd* pWndParent, CRect rect);
	virtual void OnDrawValue(CDC* pDC, CRect rect);

	CMFCToolBarImages m_imageListIcons;
	CStringList m_lstIconNames;
};


//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////
// CIconCombo window

class CIconCombo : public CComboBox
{
// Construction
public:
	CIconCombo(CMFCToolBarImages& imageListIcons, CStringList& lstIconNames);

// Attributes
protected:
	CMFCToolBarImages& m_imageListIcons;
	CStringList& m_lstIconNames;

// Implementation
public:
	virtual ~CIconCombo();

protected:
	//{{AFX_MSG(CIconCombo)
	afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
	afx_msg void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
};


//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////

class CComboBoxExProp : public CMFCPropertyGridProperty
{
public:
	CComboBoxExProp(const CString& strName, const CString& strValue, LPCTSTR lpszDescr = NULL, DWORD dwData = 0, CImageList* pImageList = NULL);
  
	virtual BOOL OnUpdateValue();
	BOOL AddOption(LPCTSTR lpszOption, int nIcon = -1, int nIndent = 0);

  template <typename T>
  void update(T fGet);

public:
  Event2<const std::string&, int>      eventUpdate;

protected:
	virtual CComboBox* CreateCombo(CWnd* pWndParent, CRect rect);
	virtual BOOL OnEdit(LPPOINT lptClick);

	CImageList* m_pImageList;
	CArray<int, int> m_lstIcons;
	CArray<int, int> m_lstIndents;
};

template <typename T>
void
CComboBoxExProp::update(T fGet)
{
  CMFCPropertyGridProperty::RemoveAllOptions();

  const char* pText = NULL;

	for (int i = 0; (pText = fGet(i)) != NULL; i++)
    AddOption(fGet(i));

  SetValue(fGet(-1));
}


//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////

class COwnerDrawDescrProp : public CMFCPropertyGridProperty
{
public:
	COwnerDrawDescrProp(const CString& strName, const COleVariant& varValue);

protected:
	virtual void OnDrawDescription(CDC* pDC, CRect rect);
};

//////////////////////////////////////////////////////////////////////////////////
//////////////////////////////////////////////////////////////////////////////////

class CCustomDlgProp : public CMFCPropertyGridProperty
{
public:
	CCustomDlgProp(const CString& strName, const COleVariant& varValue);

protected:
	virtual BOOL HasButton() const { return TRUE; }
	virtual void OnClickButton(CPoint point);
};

#endif // !CPROPERTY_GRID_CUSTOM_CTRL