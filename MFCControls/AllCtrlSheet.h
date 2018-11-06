#pragma once
#include "afxdlgs.h"
#include "TreePage.h"
#include "ListPage.h"
#include "OthersPage.h"
#include "ToolBarPage.h"

class CAllCtrlSheet :
    public CPropertySheet
{
    DECLARE_DYNAMIC(CAllCtrlSheet)
public:
    CAllCtrlSheet(LPCTSTR pszCaption, CWnd* pParent = NULL, UINT nSelPage = 0);
    ~CAllCtrlSheet();

    CTreePage m_treePage;
    CListPage m_listPage;
    COthersPage m_othPage;
    CToolBarPage m_tbPage;

protected:
    void AddControlPages();
    HICON m_hIcon;
};

