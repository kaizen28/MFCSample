#pragma once
#include "afxcmn.h"

class CMyTreeCtrl : public CTreeCtrl
{
public:
    CMyTreeCtrl();
    virtual ~CMyTreeCtrl();
protected:
    DECLARE_MESSAGE_MAP()
private:
    BOOL TransferItem(HTREEITEM hItemDrag, HTREEITEM hItemDrop);

    BOOL m_bDragging;
    HTREEITEM m_hItemDrag;
    HTREEITEM m_hItemDrop;
    CImageList* m_pImageList;
public:
    afx_msg void OnTvnBegindrag(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);
};

// CTreePage dialog

class CTreePage : public CPropertyPage
{
	DECLARE_DYNAMIC(CTreePage)

public:
	CTreePage(CWnd* pParent = NULL);   // standard constructor
	virtual ~CTreePage();

    virtual BOOL OnInitDialog();
// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_TREE_PAGE };
#endif

    //CMyTreeCtrl m_treeCtrl;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
private:
    void InitTreeCtrl();

	HTREEITEM   m_rghItem[12];

public:
    CMyTreeCtrl m_treeCtrl;
    afx_msg void OnClickTree1(NMHDR *pNMHDR, LRESULT *pResult);
};
