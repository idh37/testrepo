#pragma once

#include "afxwin.h"
//#include "GlobalBase.h"

//버튼 활성 상태 플래그
#define UPG_PROFILE_FLAG_MESSEAGE		0x00000001
#define UPG_PROFILE_FLAG_WISPHER		0x00000002
#define UPG_PROFILE_FLAG_INVITE_CLUB	0x00000004
#define UPG_PROFILE_FLAG_BANISHMENT		0x00000008
#define UPG_PROFILE_FLAG_TOGETHER		0x00000010
#define UPG_PROFILE_FLAG_SAMEROOM		0x00000020
#define UPG_PROFILE_FLAG_VIP			0x00000040
#define UPG_PROFILE_FLAG_ADD_BUDDY		0x00000080
#define UPG_PROFILE_FLAG_ADD_BLACKLIST	0x00000100
#define UPG_PROFILE_FLAG_INVITE_CAFE	0x00000200
#define UPG_PROFILE_FLAG_ASK_CAFE		0x00000400
#define UPG_PROFILE_FLAG_INVITE_UNION	0x00000800
#define UPG_PROFILE_FLAG_ASK_UNION		0x00001000

class UPG_COMMON_EXPORT CProfileDlg : public LSY::CLSYLibDialogBase
{
protected:
	UINT		m_uButtonFlag;
	USERINFO	m_cUserInfo;
	bool		m_bFirst;
public:
	CProfileDlg(CWnd* pParent = NULL);
	virtual ~CProfileDlg(){}

	void Init(USERINFO *pUI);
	void CheckButtonFlag(void);
public:
	virtual bool OnCreatedProject(void);
	virtual void SetGameScore(IDX_GAME nGame);

	void OnChangeUser(SMALLUSERINFO *pSUI);

	LRESULT OnClickButton(LSY::CMessage *pMsg);
	LRESULT OnSelChange(LSY::CMessage *pMsg);
	LRESULT OnTabScore(LSY::CMessage *pMsg);
	void HandleResponse(int Signal, int TotSize, char *lpData);

	virtual INT_PTR DoModal(CWnd *pParent);
protected:
	DECLARE_MESSAGE_MAP()
};