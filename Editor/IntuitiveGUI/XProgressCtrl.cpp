#include "stdafx.h"
#include "XProgressCtrl.h"

// CXProgressCtrl

IMPLEMENT_DYNAMIC(CXProgressCtrl, CProgressCtrl)

CXProgressCtrl::CXProgressCtrl() : m_clrTextColor(RGB(0, 0, 0)),
m_clrTextBkColor(RGB(255, 255, 255)),
m_clrBarColor(RGB(128, 128, 255)),
m_clrBarBkColor(RGB(235, 235, 235)),
m_AlignText(ALIGN_CENTER), 
m_bShowPercent(TRUE)
{
    
}

CXProgressCtrl::~CXProgressCtrl()
{
    if(m_font.GetSafeHandle())
    {
        m_font.DeleteObject();
    }
}


BEGIN_MESSAGE_MAP(CXProgressCtrl, CProgressCtrl)
    ON_WM_ERASEBKGND()
    ON_WM_PAINT()
END_MESSAGE_MAP()

// CXProgressCtrl message handlers
BOOL CXProgressCtrl::OnEraseBkgnd(CDC *pDC)
{
    return TRUE;
}

void CXProgressCtrl::OnPaint()
{
    CPaintDC paintDC(this);

    int nMin = 0;
    int nMax = 0;

    GetRange(nMin, nMax);
    ASSERT(nMin <= nMax);

    int nPos = GetPos();
    ASSERT(nPos >= nMin && nPos <= nMax);

    DWORD dwStyle = GetStyle();

    BOOL bVertical = FALSE;
    if(dwStyle & PBS_VERTICAL)
    {
        bVertical = TRUE;
    }

    CDC dc;
    dc.CreateCompatibleDC(&paintDC);
    ASSERT(dc.GetSafeHdc());

    CRect rect;
    GetClientRect(&rect);

    CBitmap bmp;
    bmp.CreateCompatibleBitmap(&paintDC, rect.Width(), rect.Height());
    ASSERT(bmp.GetSafeHandle());

    CBitmap* pOldBitmap = (CBitmap*)dc.SelectObject(&bmp);

    CFont* pOldFont = NULL;
    CWnd* pParent = GetParent();    
    ASSERT(pParent);

    CFont* pFont = pParent->GetFont();
    ASSERT(pFont);
    if(bVertical)
    {
        if(NULL == m_font.GetSafeHandle())
        {
            LOGFONT lf = {0};
            pFont->GetLogFont(&lf);
            lf.lfEscapement = 900;
            m_font.CreateFontIndirect(&lf);
        }
        ASSERT(m_font.GetSafeHandle());
        pOldFont = (CFont*)dc.SelectObject(&m_font);            
    }
    else
    {
        pOldFont = (CFont*)dc.SelectObject(pFont);
    }

    double dPercent = (double)(nPos - nMin) / ((double)(nMax - nMin));

    dc.DrawEdge(rect, EDGE_SUNKEN, BF_RECT | BF_FLAT);

    CRect rc(rect);
    rc.DeflateRect(CSize(2, 2));
    dc.FillSolidRect(&rc, m_clrBarBkColor);

    CString strText(_T(""));
    GetWindowText(strText);

    if(m_bShowPercent)
    {
        strText.AppendFormat(_T("%d%% "), static_cast<int>((dPercent * 100.0) + 0.5));
    }

    dc.SetBkMode(TRANSPARENT);
    dc.SetTextColor(m_clrTextColor);

    CPoint pt(0, 0);
    CSize size = dc.GetOutputTextExtent(strText);

    if(!bVertical)
    {
        switch(m_AlignText)
        {
        case ALIGN_LEFT:
            pt.x = rc.left;
            break;

        case ALIGN_RIGHT:
            pt.x = rc.right - size.cx;
            break;

        case ALIGN_CENTER:
        default:
            pt.x = rc.left + (rc.Width() - size.cx) / 2;
            break;
        }
        pt.y = rc.top + (rc.Height() - size.cy) / 2;

        CRect rcPos(rc);

        rcPos.right = rcPos.left + (int)(dPercent * rcPos.Width());
        dc.FillSolidRect(rcPos, m_clrBarColor);

        dc.SetTextColor(m_clrTextColor);
        dc.ExtTextOut(pt.x, pt.y, ETO_OPAQUE, rcPos, strText, NULL);

        dc.SetTextColor(m_clrTextBkColor);
        dc.ExtTextOut(pt.x, pt.y, ETO_CLIPPED, &rcPos, strText, NULL);
    }
    else
    {
        switch(m_AlignText)
        {
        case ALIGN_LEFT:
            pt.y = rc.bottom;
            break;

        case ALIGN_RIGHT:
            pt.y = rc.top + size.cx;
            break;

        case ALIGN_CENTER:
        default:
            pt.y = rc.bottom - (rc.Height() - size.cx) / 2;
            break;
        }
        pt.x = rc.left + (rc.Width() - size.cy) / 2;

        CRect rcPos(rc);

        rcPos.top = rcPos.bottom - (int)(dPercent * rcPos.Height());
        dc.FillSolidRect(rcPos, m_clrBarColor);

        dc.SetTextColor(m_clrTextColor);
        dc.ExtTextOut(pt.x, pt.y, ETO_OPAQUE, rcPos, strText, NULL);

        dc.SetTextColor(m_clrTextBkColor);
        dc.ExtTextOut(pt.x, pt.y, ETO_CLIPPED, &rcPos, strText, NULL);
    }
    
    paintDC.BitBlt(rect.left, rect.top ,rect.Width(), rect.Height(), &dc, 0, 0, SRCCOPY);

    dc.SelectObject(pOldFont);
    dc.SelectObject(pOldBitmap);
    bmp.DeleteObject();

    dc.DeleteDC();
}  