#pragma once

#include "UserStruct.h"
#include "UPGDlgBase.h"

class UPG_COMMON_EXPORT CRecvInviteDlg : public CUPGDlgBase
{
public:
 	USERINFO AskedUI;
 	SMALLROOMINFO RoomInfo;

	NMBASE::SKINGDI::CMyBitmap Back;

	CRecvInviteDlg(CWnd* pParent = NULL);   // standard constructor

	enum { IDD = IDD_RECVINVITE_DIALOG };
	NMBASE::SKINGDI::CBmpCheckButton	m_NoInviteCheckBtn;
	NMBASE::SKINGDI::CMyButton	m_CancelBtn;
	NMBASE::SKINGDI::CMyButton	m_OkBtn;	

	void DrawBack(CDC &DC);

public:	
	virtual int DoModal(int tarunum, char *tarid, int roomnum, char *strmsg);
	virtual int DoModal(USERINFO *pAskedUI, SMALLROOMINFO* pSRI);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

protected:
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnCancel();
	afx_msg void OnPaint();

	DECLARE_MESSAGE_MAP()
};