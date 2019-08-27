#pragma once

class UPG_COMMON_EXPORT CChattingRoomAddUserDlg : public LSY::CLSYLibDialogBase
{
protected:
	MAPSTRING		m_mapIDAndNickName;
	bool			m_bFirst;
	int				m_nID;

	LSY::CRichText	*m_pInviteUser;
public:
	CChattingRoomAddUserDlg(CWnd* pParent = NULL);
	virtual ~CChattingRoomAddUserDlg(){}

	virtual INT_PTR DoModal(CWnd *pParent);

	virtual bool OnCreatedProject(void);
	LRESULT OnClickButton(LSY::CMessage *pMsg);
	LRESULT OnRichEditLButtonDown(LSY::CMessage *pMsg);

	LRESULT OnEditReturn(LSY::CMessage *pMsg);
	LRESULT OnFocusIn(LSY::CMessage *pMsg);

	const MAPSTRING &GetIDAndNickName(void){return m_mapIDAndNickName;}

	virtual void OnOK();
	virtual void OnCancel();
protected:
	DECLARE_MESSAGE_MAP()

	void OnAddIDAndNickName(const std::string &strID, const std::string &strNickName);
	void OnResetRichEdit(void);
public:
	void OnNotFind(void);
	void OnFind(const std::string &strID, const std::string &strNickName);
};


UPG_COMMON_EXPORT extern  CChattingRoomAddUserDlg *GetIDToChattingRoomAddUserDlg(const int &nID);