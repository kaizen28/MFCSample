#include "MyTreeCtrl.h"

CMyTreeCtrl::CMyTreeCtrl()
{
}

CMyTreeCtrl::~CMyTreeCtrl()
{
}

BEGIN_MESSAGE_MAP(CMyTreeCtrl, CTreeCtrl)
	//ON_NOTIFY_REFLECT(NM_RCLICK, OnRClick)
	ON_WM_RBUTTONDOWN()
END_MESSAGE_MAP()

void CMyTreeCtrl::OnRClick(NMHDR* pNMHDR, LRESULT* pResult)
{
}

void CMyTreeCtrl::OnRButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	CPoint ptClient = point;
	ScreenToClient(&ptClient);

	HTREEITEM hHitItem;
	if ((hHitItem = HitTest(ptClient)) != NULL)
		SelectItem(hHitItem);

	CTreeCtrl::OnRButtonDown(nFlags, point);
}
