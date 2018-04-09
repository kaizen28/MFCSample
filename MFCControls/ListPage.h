#pragma once
#include "afxcmn.h"

// function
// 1. CheckBox
// 2. Full Row Selection
// 3. Multi Selection

class CMyListCtrl : public CListCtrl
{
public:
    CMyListCtrl();
    virtual ~CMyListCtrl();

    CImageList* m_pDragImages;
    BOOL m_bDragging;
    int m_nItemDrag;
    int m_nItemDrop;
    CPoint m_ptOrigin;
    CPoint m_ptHotSpot;
    CSize m_sizeDelta;

    DECLARE_MESSAGE_MAP()
    afx_msg void OnLvnBegindrag(NMHDR *pNMHDR, LRESULT *pResult);
    afx_msg void OnMouseMove(UINT nFlags, CPoint point);
    afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
};

// CListPage dialog

class CListPage : public CPropertyPage
{
	DECLARE_DYNAMIC(CListPage)

public:
	CListPage();
	virtual ~CListPage();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_LIST_PAGE };
#endif

private:
    void InitImageList();
    void InitItems();

    CImageList* m_pImageHdrSmall;
    CImageList* m_pImageList;
    CImageList* m_pImageListSmall;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
    CMyListCtrl m_lcList;
    virtual BOOL OnInitDialog();
    afx_msg void OnClickedBtnShow();
    afx_msg void OnItemchangedList1(NMHDR *pNMHDR, LRESULT *pResult);
};
