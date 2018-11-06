#include "stdafx.h"
#include "AllCtrlSheet.h"
#include "MFCControls.h"

IMPLEMENT_DYNAMIC(CAllCtrlSheet, CPropertySheet)

CAllCtrlSheet::CAllCtrlSheet(LPCTSTR pszCaption, CWnd* pParent, UINT nSelPage)
    : CPropertySheet(pszCaption, pParent, nSelPage)
{
    AddControlPages();
}

CAllCtrlSheet::~CAllCtrlSheet()
{
}

void CAllCtrlSheet::AddControlPages()
{
    m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
    m_psh.dwFlags |= PSP_USEHICON;
    m_psh.hIcon = m_hIcon;
    m_psh.dwFlags |= PSH_NOAPPLYNOW;
    m_psh.dwFlags &= ~PSH_HASHELP;

    AddPage(&m_treePage);
    AddPage(&m_listPage);
    AddPage(&m_othPage);
    AddPage(&m_tbPage);
}