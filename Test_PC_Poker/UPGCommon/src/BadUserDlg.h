#pragma once

#include "resource.h"
#include "HideChatList.h"
#include "UPGDlgBase.h"

typedef	struct
{
	char	szID[16];		// 사용자 아이디
	char	szNick[20]; // 닉네임수정
}IDLIST_ITEM;

enum REPORT_KIND
{
	REPORT_KIND_NULL=0,
	REPORT_KIND_SWEAR,		// 욕설 및 불건전대화
	REPORT_KIND_MONEYDEAL,	//게임머니 거래 및 상업성 광고신고
	REPORT_KIND_SCAM,		//짜고치기
	REPORT_KIND_ONLY_SCAM,		// 짜고치기 신고 버튼 클릭시
};

class UPG_COMMON_EXPORT CBadUserDlg : public LSY::CLSYLibDialogBase
{
public:
	CBadUserDlg(CWnd* pParent = NULL);
	virtual ~CBadUserDlg();

	static int	ms_nSubmitCnt;
	CString		m_strTargetID;
	CString		m_strTargetNickName;
	CString		m_strSubmitReason;
	REPORT_KIND m_nKind;
	bool		m_bEditNoClick;

	std::list< IDLIST_ITEM > m_vIDList;
	
	void	AddUser( char *szID, char *szNick );
	CString	FindID( CString szNick );
	void	ClearIDList();

	virtual bool OnCreatedProject(void);
	void SetReasonTextCount(int nCount);

	LRESULT OnClickButton(LSY::CMessage *pMsg);
	LRESULT OnFocusIn(LSY::CMessage *pMsg);
	LRESULT OnSelChange(LSY::CMessage *pMsg);
	LRESULT OnKeyProc(LSY::CMessage *pMsg);

	virtual INT_PTR DoModal();
protected:
	virtual void OnOK();
	virtual void OnCancel();

	DECLARE_MESSAGE_MAP()

private:

	void _LoadCommonTypeDlgData();
	void _LoadOnlyScamTypeDlgData();
};