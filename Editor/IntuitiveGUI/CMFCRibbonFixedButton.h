#ifndef CMFC_RIBBON_FIXED_BUTTON_H
# define CMFC_RIBBON_FIXED_BUTTON_H

class CMFCRibbonFixedButton : public CMFCRibbonButton
{
protected:
    bool m_bSmallMode;

public:
    CMFCRibbonFixedButton(void) : m_bSmallMode(false)
    {}
    CMFCRibbonFixedButton(UINT nID, LPCTSTR lpszText, int nSmallImageIndex=-1, int nLargeImageIndex=-1, BOOL bAlwaysShowDescription=FALSE)
        : m_bSmallMode(false), CMFCRibbonButton(nID, lpszText, nSmallImageIndex, nLargeImageIndex, bAlwaysShowDescription)
    {}
    CMFCRibbonFixedButton(UINT nID, LPCTSTR lpszText, HICON hIcon, BOOL bAlwaysShowDescription=FALSE, HICON hIconSmall=NULL, BOOL bAutoDestroyIcon=FALSE, BOOL bAlphaBlendIcon=FALSE)
        : m_bSmallMode(false), CMFCRibbonButton(nID, lpszText, hIcon, bAlwaysShowDescription, hIconSmall, bAutoDestroyIcon, bAlphaBlendIcon)
    {}

public:
    void SetSmallMode(void)
    { m_bSmallMode = true; }
    void SetLargeMode(void)
    { m_bSmallMode = false; }

public:
    virtual BOOL HasLargeMode() const
    {
        if (m_bSmallMode)
            return FALSE;
        return __super::HasLargeMode();
    }
};

#endif // !CMFC_RIBBON_FIXED_BUTTON_H