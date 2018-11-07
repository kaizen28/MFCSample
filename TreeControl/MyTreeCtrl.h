#pragma once
#include "stdafx.h"

class CMyTreeCtrl : public CTreeCtrl
{
public:
	CMyTreeCtrl();
	~CMyTreeCtrl();

	virtual void RClick(NMHDR* pNMHDR, LRESULT* pResult) {};

	DECLARE_MESSAGE_MAP()
	afx_msg void OnRClick(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
};
