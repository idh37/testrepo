#pragma once

#include "resource.h"
#include "UPGDlgBase.h"

class UPG_COMMON_EXPORT CGameGradeDlg : public CUPGDlgBase
{
public:
	CGameGradeDlg(CWnd* pParent = NULL);   // standard constructor

	BOOL Create(CWnd* pParentWnd);
	BOOL Create(CWnd *pParentWnd, int x, int y, int UseMode);

	void MoveDlg(int x, int y);
	void DoModal(CWnd* pParent, int Mode);

	CWnd*		m_pParent;
	NMBASE::SKINGDI::CMyBitmap	m_Back;
	int			m_UseMode;

	int m_PosX;
	int m_PosY;

	enum { IDD = IDD_DIALOG_GAME_GRADE };

public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

protected:
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnPaint();
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnDestroy();

	DECLARE_MESSAGE_MAP()
};