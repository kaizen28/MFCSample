#pragma once
#include "afxwin.h"

class CColorListBox : public CListBox
{
public:
    virtual void MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct);
    virtual void DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);
    virtual int CompareItem(LPCOMPAREITEMSTRUCT lpCompareItemStruct);

    void AddColorItem(COLORREF color);
};

// COthersPage dialog

class COthersPage : public CPropertyPage
{
	DECLARE_DYNAMIC(COthersPage)

public:
	COthersPage(CWnd* pParent = NULL);   // standard constructor
	virtual ~COthersPage();

// Dialog Data
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_OTHERS_PAGE };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
    CBitmapButton m_bnMyImage;
    CColorListBox m_colors;

    virtual BOOL OnInitDialog();
};
