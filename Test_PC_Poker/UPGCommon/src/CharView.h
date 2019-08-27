#if !defined(AFX_CHARVIEW_H__75B97AA2_F8E7_11D3_97A2_0050BF0FBE67__INCLUDED_)
#define AFX_CHARVIEW_H__75B97AA2_F8E7_11D3_97A2_0050BF0FBE67__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CharacterView.h : header file
//

#include "UserStruct.h"

/////////////////////////////////////////////////////////////////////////////
// CCharView window

class UPG_COMMON_EXPORT CCharView : public CWnd
{
public:
	NMBASE::GRAPHICGDI::CPage Page;
	int CharNum;
	int UseMode;
	USERINFO UI;
	BOOL bNowSeleted;

	NMBASE::AVATAR::CAvatar Avatar;

	BOOL	bEvent;

	WORD m_colorBackGround;
	

	CCharView();

	void Create(CWnd *pParentWnd, CRect &rect, int UseMode, int nID);
	//void SetCharacter(int charnum);
	void SetCharacter(USERINFO *pUI);
	void SetBGColor(WORD color)
	{
		m_colorBackGround=color;
	}

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CCharView)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CCharView();

	// Generated message map functions
protected:
	//{{AFX_MSG(CCharView)
	afx_msg void OnPaint();
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT nIDEvent);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_CHARVIEW_H__75B97AA2_F8E7_11D3_97A2_0050BF0FBE67__INCLUDED_)
