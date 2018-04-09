// TreePage.cpp : implementation file
//

#include "stdafx.h"
#include "MFCControls.h"
#include "TreePage.h"
#include "afxdialogex.h"

CMyTreeCtrl::CMyTreeCtrl() : m_bDragging(FALSE)
{
}

CMyTreeCtrl::~CMyTreeCtrl()
{
}

BOOL CMyTreeCtrl::TransferItem(HTREEITEM hItemDrag, HTREEITEM hItemDrop)
{
    TV_INSERTSTRUCT tvstruct;
    TCHAR szBuffer[50];
    HTREEITEM hNewItem, hFirstChild;

	tvstruct.item.hItem = hItemDrag;
	tvstruct.item.cchTextMax = 49;
	tvstruct.item.pszText = szBuffer;
	tvstruct.item.mask = TVIF_CHILDREN | TVIF_HANDLE | TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_TEXT;
	GetItem(&tvstruct.item);
	
    tvstruct.hParent = hItemDrop;
	tvstruct.hInsertAfter = TVI_SORT;
	tvstruct.item.mask = TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_TEXT;
    hNewItem = InsertItem(&tvstruct);

    if((hFirstChild = GetChildItem(hItemDrag)) != NULL)
    {
        TransferItem(hFirstChild, hNewItem);
    }

    /*while((hFirstChild = GetChildItem(hItemDrag)) != NULL)
    {
        TransferItem(hFirstChild, hNewItem);
        DeleteItem(hFirstChild);
    }*/
    
    DeleteItem(hItemDrag);

    return TRUE;
}

void CMyTreeCtrl::OnTvnBegindrag(NMHDR *pNMHDR, LRESULT *pResult)
{
    LPNMTREEVIEW pNMTreeView = reinterpret_cast<LPNMTREEVIEW>(pNMHDR);
    // TODO: Add your control notification handler code here
    *pResult = 0;

    CPoint ptAction;
    UINT nFlags;

    GetCursorPos(&ptAction);
    ScreenToClient(&ptAction);

    //ASSERT(!m_bDragging);
    m_bDragging = TRUE;
    m_hItemDrag = HitTest(ptAction, &nFlags);
    m_hItemDrop = NULL;

    ASSERT(m_pImageList == NULL);
    m_pImageList = CreateDragImage(m_hItemDrag);
    m_pImageList->DragShowNolock(TRUE);
    m_pImageList->SetDragCursorImage(0, CPoint(0, 0));
    m_pImageList->BeginDrag(0, CPoint(0, 0));
    m_pImageList->DragMove(ptAction);
    m_pImageList->DragEnter(this, ptAction);
    SetCapture();

    TRACE(_T("OnTvnBegindrag\n"));
}


void CMyTreeCtrl::OnLButtonUp(UINT nFlags, CPoint point)
{
    // TODO: Add your message handler code here and/or call default
    if(m_bDragging)
    {
        ASSERT(m_pImageList != NULL);
        m_pImageList->DragLeave(this);
        m_pImageList->EndDrag();
        delete m_pImageList;
        m_pImageList = NULL;

        if (m_hItemDrag != m_hItemDrop)
        {
            TransferItem(m_hItemDrag, m_hItemDrop);
            //DeleteItem(m_hItemDrag);
        }
        else
            MessageBeep(0);

        ReleaseCapture();
        m_bDragging = FALSE;
        SelectDropTarget(NULL);
    }

    CTreeCtrl::OnLButtonUp(nFlags, point);
}


void CMyTreeCtrl::OnMouseMove(UINT nFlags, CPoint point)
{
    // TODO: Add your message handler code here and/or call default
    UINT nHitFlags;
    HTREEITEM hItem;

    if(m_bDragging)
    {
        ASSERT(m_pImageList != NULL);
        m_pImageList->DragMove(point);

        if((hItem = HitTest(point, &nHitFlags)) != NULL)
        {
            m_pImageList->DragLeave(this);
            SelectDropTarget(hItem);
            m_hItemDrop = hItem;
            m_pImageList->DragEnter(this, point);
        }
    }

    CTreeCtrl::OnMouseMove(nFlags, point);
}

BEGIN_MESSAGE_MAP(CMyTreeCtrl, CTreeCtrl)
    ON_NOTIFY_REFLECT(TVN_BEGINDRAG, &CMyTreeCtrl::OnTvnBegindrag)
    ON_WM_LBUTTONUP()
    ON_WM_MOUSEMOVE()
END_MESSAGE_MAP()

// CTreePage dialog

IMPLEMENT_DYNAMIC(CTreePage, CPropertyPage)

CTreePage::CTreePage(CWnd* pParent /*=NULL*/)
	: CPropertyPage(IDD_TREE_PAGE)
{

}

CTreePage::~CTreePage()
{
}

void CTreePage::DoDataExchange(CDataExchange* pDX)
{
    CPropertyPage::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_TREE1, m_treeCtrl);
}


BEGIN_MESSAGE_MAP(CTreePage, CPropertyPage)
    ON_NOTIFY(NM_CLICK, IDC_TREE1, &CTreePage::OnClickTree1)
END_MESSAGE_MAP()


// CTreePage message handlers


BOOL CTreePage::OnInitDialog()
{
    CPropertyPage::OnInitDialog();

    // TODO:  Add extra initialization here
    InitTreeCtrl();

    return FALSE;  // return TRUE unless you set the focus to a control
                  // EXCEPTION: OCX Property Pages should return FALSE
}

void CTreePage::InitTreeCtrl()
{
    // Insert Tree Item 
	CString             strItems[12];
    strItems[0]= _T("Dogs");
	strItems[1]= _T("German Shepherd");
	strItems[2]= _T("Dalmatian");
	strItems[3]= _T("Great Dane");
	strItems[4]= _T("Birds");
	strItems[5]= _T("Hummingbird");
	strItems[6]= _T("Pigeon");
	strItems[7]= _T("Eagle");
	strItems[8]= _T("Fish");
	strItems[9]= _T("Snapper");
	strItems[10]= _T("Sole");
	strItems[11]= _T("Salmon");

	int                 iItem;
	TV_INSERTSTRUCT     curTreeItem;

	for (iItem = 0; iItem < 12; iItem ++)
	{
		curTreeItem.hParent = (iItem % 4 == 0)? NULL : m_rghItem[iItem / 4 * 4];
		curTreeItem.hInsertAfter = TVI_SORT;
		curTreeItem.item.iImage = iItem / 4 * 2;
		curTreeItem.item.iSelectedImage = curTreeItem.item.iImage + 1;
		curTreeItem.item.pszText = strItems[iItem].GetBuffer(strItems[iItem].GetLength());
		curTreeItem.item.mask = TVIF_IMAGE | TVIF_SELECTEDIMAGE | TVIF_TEXT;
		m_rghItem[iItem] = m_treeCtrl.InsertItem(&curTreeItem);
	}

    // Set ImageList
    CImageList* pImageList = new CImageList();
    pImageList->Create(32, 16, ILC_MASK, 6, 4);

    CBitmap bitmap;

    for(int nID = IDB_BIRD; nID <= IDB_FISH_SEL; nID++)
    {
        bitmap.LoadBitmap(nID);
        pImageList->Add(&bitmap, (COLORREF)0xFFFFFF);
        bitmap.DeleteObject();
    }

    m_treeCtrl.SetImageList(pImageList, TVSIL_NORMAL);

    // Set Tree Style
    DWORD dwStyle;
    dwStyle = GetWindowLong(m_treeCtrl.GetSafeHwnd(), GWL_STYLE);
    dwStyle &= ~TVS_DISABLEDRAGDROP;
    SetWindowLong(m_treeCtrl.GetSafeHwnd(), GWL_STYLE, dwStyle);

}

void CTreePage::OnClickTree1(NMHDR *pNMHDR, LRESULT *pResult)
{
    // TODO: Add your control notification handler code here
    *pResult = 0;

    // CheckBox Event
    UINT nFlags;
    CPoint point;

    GetCursorPos(&point);
    m_treeCtrl.ScreenToClient(&point);

    HTREEITEM hItem = m_treeCtrl.HitTest(point, &nFlags);
    if((hItem != NULL) && (TVHT_ONITEMSTATEICON & nFlags))
    {
        CString strText = m_treeCtrl.GetItemText(hItem);
        TRACE(_T("%s Item: %s\n"), strText, m_treeCtrl.GetCheck(hItem) ? _T("Checked") : _T("UnChecked"));

        // PostMessage to CMyTreeCtrl because value is not sync
    }
    else if((hItem != NULL) && (TVHT_ONITEMLABEL & nFlags))
    {
        CString strText = m_treeCtrl.GetItemText(hItem);
        TRACE(_T("%s Item: Labeled\n"), strText);
    }
}
