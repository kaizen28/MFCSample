// OthersPage.cpp : implementation file
//

#include "stdafx.h"
#include "MFCControls.h"
#include "OthersPage.h"
#include "afxdialogex.h"

void CColorListBox::MeasureItem(LPMEASUREITEMSTRUCT lpMIS)
{
    lpMIS->itemHeight = 20;
}

void CColorListBox::DrawItem(LPDRAWITEMSTRUCT lpDIS)
{
	CDC* pDC = CDC::FromHandle(lpDIS->hDC);
	COLORREF cr = (COLORREF)lpDIS->itemData; // RGB in item data

	if (lpDIS->itemAction & ODA_DRAWENTIRE)
	{
		// Paint the color item in the color requested
		CBrush br(cr);
		pDC->FillRect(&lpDIS->rcItem, &br);
	}

	if ((lpDIS->itemState & ODS_SELECTED) &&
		(lpDIS->itemAction & (ODA_SELECT | ODA_DRAWENTIRE)))
	{
		// item has been selected - hilite frame
		COLORREF crHilite = RGB(255-GetRValue(cr),
						255-GetGValue(cr), 255-GetBValue(cr));
		CBrush br(crHilite);
		pDC->FrameRect(&lpDIS->rcItem, &br);
	}

	if (!(lpDIS->itemState & ODS_SELECTED) &&
		(lpDIS->itemAction & ODA_SELECT))
	{
		// Item has been de-selected -- remove frame
		CBrush br(cr);
		pDC->FrameRect(&lpDIS->rcItem, &br);
	}
}

int CColorListBox::CompareItem(LPCOMPAREITEMSTRUCT lpCIS)
{
	COLORREF cr1 = (COLORREF)lpCIS->itemData1;
	COLORREF cr2 = (COLORREF)lpCIS->itemData2;
	if (cr1 == cr2)
		return 0;       // exact match

	// first do an intensity sort, lower intensities go first
	int intensity1 = GetRValue(cr1) + GetGValue(cr1) + GetBValue(cr1);
	int intensity2 = GetRValue(cr2) + GetGValue(cr2) + GetBValue(cr2);
	if (intensity1 < intensity2)
		return -1;      // lower intensity goes first
	else if (intensity1 > intensity2)
		return 1;       // higher intensity goes second

	// if same intensity, sort by color (blues first, reds last)
	if (GetBValue(cr1) > GetBValue(cr2))
		return -1;
	else if (GetGValue(cr1) > GetGValue(cr2))
		return -1;
	else if (GetRValue(cr1) > GetRValue(cr2))
		return -1;
	else
		return 1;
}

void CColorListBox::AddColorItem(COLORREF color)
{
    AddString((LPCTSTR)color);
}

// COthersPage dialog

IMPLEMENT_DYNAMIC(COthersPage, CPropertyPage)

COthersPage::COthersPage(CWnd* pParent /*=NULL*/)
	: CPropertyPage(IDD_OTHERS_PAGE)
{

}

COthersPage::~COthersPage()
{
}

void COthersPage::DoDataExchange(CDataExchange* pDX)
{
    CPropertyPage::DoDataExchange(pDX);
    //DDX_Control(pDX, IDC_MYIMAGE, m_bnMyImage);
}


BEGIN_MESSAGE_MAP(COthersPage, CPropertyPage)
END_MESSAGE_MAP()


// COthersPage message handlers


BOOL COthersPage::OnInitDialog()
{
    CPropertyPage::OnInitDialog();

    // CBitmapButton
    m_bnMyImage.AutoLoad(IDC_MYIMAGE, this);

    // Custom ListBox
    m_colors.SubclassDlgItem(IDC_LIST1, this);

    for (int red = 0; red <= 255; red += 255)
	    for (int green = 0; green <= 255; green += 255)
		    for (int blue = 0; blue <= 255; blue += 255)
			    m_colors.AddColorItem(RGB(red, green, blue));
    return TRUE;  // return TRUE unless you set the focus to a control
                  // EXCEPTION: OCX Property Pages should return FALSE
}
