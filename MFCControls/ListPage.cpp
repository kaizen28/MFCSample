// ListPage.cpp : implementation file
//

#include "stdafx.h"
#include "MFCControls.h"
#include "ListPage.h"
#include "afxdialogex.h"

CMyListCtrl::CMyListCtrl() : m_pDragImages(NULL)
{
}

CMyListCtrl::~CMyListCtrl()
{
}

BEGIN_MESSAGE_MAP(CMyListCtrl, CMFCListCtrl)
    ON_NOTIFY_REFLECT(LVN_BEGINDRAG, &CMyListCtrl::OnLvnBegindrag)
    ON_WM_MOUSEMOVE()
    ON_WM_LBUTTONUP()
END_MESSAGE_MAP()

BOOL CMyListCtrl::MoveRow(int from, int to)
{
	//Can't move to the same place, or from or to a negative index
	if(from == to || from < 0 || to < 0)
		return FALSE;
	//First Copy the row to the new location
	if(CopyRow(from, to))
	{
		//If we have just inserted a row before
		//this one in the list, we need to increment
		//our index.
		if(from > to)
			DeleteItem(from + 1);
		else
			DeleteItem(from);
		return TRUE;
	}
	else
		return FALSE;
}
BOOL CMyListCtrl::CopyRow(int from, int to)
{	
	//Can't move to the same place, or from or to a negative index
	if(from == to || from < 0 || to < 0)
		return FALSE;
	//Copy the row to the new index
	InsertItem(to, GetItemText(from, 0));
	//If row has been inserted before original
	//increment the original
	if(from > to)
		from++;
	//Loop through subitems
	for(int i = 1; i < 2; i++)
	{
		SetItemText(to, i, GetItemText(from, i));
	}
	return TRUE;
}

COLORREF CMyListCtrl::OnGetCellTextColor(int nRow, int nColum)
{
	//return CMFCListCtrl::OnGetCellTextColor(nRow, nColum);
    return RGB(255, 0, 0);
}

COLORREF CMyListCtrl::OnGetCellBkColor(int nRow, int nColum)
{
	//return CMFCListCtrl::OnGetCellBkColor(nRow, nColum);
    return RGB(0, 255, 0);
}

HFONT CMyListCtrl::OnGetCellFont(int nRow, int nColum, DWORD dwData)
{
		CFont* pFont = GetFont();
		LOGFONT logFont = {0,};

		pFont->GetLogFont(&logFont);

        logFont.lfStrikeOut = TRUE;
		//logFont.lfWeight = pHighlight->m_bBold ? FW_BOLD : 0;

		if(m_font.m_hObject != NULL)
			m_font.DeleteObject();

		BOOL bFont = m_font.CreateFontIndirect(&logFont);
		return (HFONT)m_font;
}

void CMyListCtrl::OnLvnBegindrag(NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
    // TODO: Add your control notification handler code here
    *pResult = 0;

    ASSERT(!m_bDragging);
    m_bDragging = TRUE;
    m_nItemDrag = pNMLV->iItem;

    CPoint ptAction, ptItem, ptImage;

    ptAction = pNMLV->ptAction;
    GetItemPosition(m_nItemDrag, &ptItem);
    GetOrigin(&m_ptOrigin);

    ASSERT(m_pDragImages == NULL);
    m_pDragImages = CreateDragImage(m_nItemDrag, &ptImage);
    m_sizeDelta = ptAction - ptImage;
    m_ptHotSpot = ptAction - ptItem + m_ptOrigin;
    m_pDragImages->DragShowNolock(TRUE);
    m_pDragImages->SetDragCursorImage(0, CPoint(0, 0));
    m_pDragImages->BeginDrag(0, CPoint(0, 0));

    ptAction -= m_sizeDelta;
    m_pDragImages->DragEnter(this, ptAction);
    m_pDragImages->DragMove(ptAction);
    m_nItemDrop = -1;
    SetCapture();
}


void CMyListCtrl::OnMouseMove(UINT nFlags, CPoint point)
{
    // TODO: Add your message handler code here and/or call default
    LV_ITEM lvItem;
    if(m_bDragging)
    {
        ASSERT(m_pDragImages != NULL);
        m_pDragImages->DragMove(point - m_sizeDelta);

        int iItem;
        if((iItem = HitTest(point)) != -1)
        {
            lvItem.mask = LVIF_STATE;
            lvItem.stateMask = LVIS_DROPHILITED;
            if(m_nItemDrop != -1)
            {
                m_pDragImages->DragLeave(this);
                lvItem.iItem = m_nItemDrop;
                lvItem.iSubItem = 0;
                lvItem.state = 0;

                SetItem(&lvItem);
            }

            m_nItemDrop = iItem;
            m_pDragImages->DragLeave(this);
            lvItem.iItem = iItem;
            lvItem.iSubItem = 0;
            lvItem.state = LVIS_DROPHILITED;
            SetItem(&lvItem);
            UpdateWindow();
        }

        point -= m_sizeDelta;
        m_pDragImages->DragEnter(this, point);
    }

    CListCtrl::OnMouseMove(nFlags, point);
}


void CMyListCtrl::OnLButtonUp(UINT nFlags, CPoint point)
{
    // TODO: Add your message handler code here and/or call default
    if(m_bDragging)
    {
        m_bDragging = FALSE;
        ASSERT(m_pDragImages != NULL);
        m_pDragImages->DragLeave(this);
        SetItemState(m_nItemDrop, 0, LVIS_DROPHILITED);
        m_pDragImages->EndDrag();

        delete m_pDragImages;
        m_pDragImages = NULL;

        //MoveRow(m_nItemDrag, m_nItemDrop);    // only move in folder

        ::ReleaseCapture();
    }

    CListCtrl::OnLButtonUp(nFlags, point);
}

// CListPage dialog

IMPLEMENT_DYNAMIC(CListPage, CPropertyPage)

CListPage::CListPage()
	: CPropertyPage(IDD_LIST_PAGE)
{

}

CListPage::~CListPage()
{
}

void CListPage::DoDataExchange(CDataExchange* pDX)
{
    CPropertyPage::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_LIST1, m_lcList);
}


BEGIN_MESSAGE_MAP(CListPage, CPropertyPage)
    ON_BN_CLICKED(IDC_BUTTON1, &CListPage::OnClickedBtnShow)
    ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST1, &CListPage::OnItemchangedList1)
END_MESSAGE_MAP()


// CListPage message handlers

void CListPage::InitImageList()
{
    m_pImageHdrSmall = new CImageList();
    m_pImageList = new CImageList();
    m_pImageListSmall = new CImageList();

    CWinApp* pApp = AfxGetApp();

    m_pImageHdrSmall->Create(16, 16, ILC_MASK, 2, 2);
    m_pImageHdrSmall->Add(pApp->LoadIcon(IDI_HDR_ICON1));
    m_pImageHdrSmall->Add(pApp->LoadIcon(IDI_HDR_ICON2));

    m_pImageList->Create(32, 32, ILC_MASK, 4, 4);
    m_pImageList->Add(pApp->LoadIcon(IDI_ICON1));
    m_pImageList->Add(pApp->LoadIcon(IDI_ICON2));
    m_pImageList->Add(pApp->LoadIcon(IDI_ICON3));
    m_pImageList->Add(pApp->LoadIcon(IDI_ICON4));

    m_pImageListSmall->Create(32, 32, ILC_MASK, 4, 4);
    m_pImageListSmall->Add(pApp->LoadIcon(IDI_ICON1));
    m_pImageListSmall->Add(pApp->LoadIcon(IDI_ICON2));
    m_pImageListSmall->Add(pApp->LoadIcon(IDI_ICON3));
    m_pImageListSmall->Add(pApp->LoadIcon(IDI_ICON4));

    m_lcList.SetImageList(m_pImageList, LVSIL_NORMAL);
    m_lcList.SetImageList(m_pImageListSmall, LVSIL_SMALL);
}

void CListPage::InitItems()
{
    CRect rect;
    m_lcList.GetWindowRect(&rect);
    m_lcList.InsertColumn(0, _T("Item"), LVCFMT_LEFT, rect.Width() * 1/ 3, 0);
    m_lcList.InsertColumn(1, _T("SubItem"), LVCFMT_LEFT, rect.Width() * 2 / 3, 1);

    LV_ITEM lvItem;
    CString strItem[4], strSubItem[4];
    strItem[0] = _T("Item0");
    strItem[1] = _T("Item1");
    strItem[2] = _T("Item2");
    strItem[3] = _T("Item3");
    strSubItem[0] = _T("SubItem0");
    strSubItem[1] = _T("SubItem1");
    strSubItem[2] = _T("SubItem2");
    strSubItem[3] = _T("SubItem3");

    for(int nItem = 0; nItem < 4; nItem++)
    {
        for(int nSubItem = 0; nSubItem < 2 ; nSubItem++)
        {
            lvItem.mask = LVIF_TEXT | (nSubItem == 0 ? LVIF_IMAGE : 0);
            lvItem.iItem = nItem;
            lvItem.iImage = nItem;
            lvItem.iSubItem = nSubItem;
            lvItem.pszText = (nSubItem == 0) ? strItem[nItem].GetBuffer() : strSubItem[nItem].GetBuffer();
            if (nSubItem == 0)
            {
                m_lcList.InsertItem(&lvItem);
                m_lcList.SetCheck(nItem, TRUE);
            }
            else
                m_lcList.SetItem(&lvItem);

        }
    }
}

BOOL CListPage::OnInitDialog()
{
    CPropertyPage::OnInitDialog();

    // TODO:  Add extra initialization here
    m_lcList.SetExtendedStyle(m_lcList.GetExtendedStyle() | LVS_EX_CHECKBOXES | LVS_EX_FULLROWSELECT);
    SetWindowTheme(m_lcList.GetSafeHwnd(), _T("Explorer"), NULL);

    InitImageList();
    InitItems();

    return TRUE;  // return TRUE unless you set the focus to a control
                  // EXCEPTION: OCX Property Pages should return FALSE
}


void CListPage::OnClickedBtnShow()
{
    // TODO: Add your control notification handler code here
    POSITION pos = m_lcList.GetFirstSelectedItemPosition();
    int nIndex = 0;
    CString strShow;

    for(; pos != NULL;)
    {
        nIndex = m_lcList.GetNextSelectedItem(pos);
        strShow += m_lcList.GetItemText(nIndex, 0) + _T("\n");
    }

    AfxMessageBox(strShow);
}


void CListPage::OnItemchangedList1(NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);
    // TODO: Add your control notification handler code here
    *pResult = 0;
    TRACE(_T("Selected Count(%d)\n"), m_lcList.GetSelectedCount());
}
