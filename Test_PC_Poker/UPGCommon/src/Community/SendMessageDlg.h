#pragma once

class CDontPasteEdit ;
class UPG_COMMON_EXPORT CSendMessageDlg : public LSY::CLSYLibDialogBase
{
public:
	std::string							m_strNickName;
	std::string							m_strID;
	std::map<std::string, std::string>	m_mapIDAndNickName;

	LSY::CTextNumber	*m_pTotalByte;
	LSY::CEdit			*m_pMessage;
	LSY::CRichText		*m_pSendUser;

	int					m_nID;
	bool				m_bFirst;
public:
	CSendMessageDlg(CWnd* pParent = NULL);
	virtual ~CSendMessageDlg(){}

	virtual bool OnCreatedProject(void);

	LRESULT OnClickButton(LSY::CMessage *pMsg);
	LRESULT OnKeyProc(LSY::CMessage *pMsg);
	virtual INT_PTR DoModal(std::string &strID, std::string &strNickName, CWnd *pParent);

	LRESULT OnRichEditLButtonDown(LSY::CMessage *pMsg);
	LRESULT OnEditReturn(LSY::CMessage *pMsg);
	LRESULT OnFocusIn(LSY::CMessage *pMsg);

	bool OnSendMessage(void);
protected:
	DECLARE_MESSAGE_MAP()

	void OnAddIDAndNickName(const std::string &strID, const std::string &strNickName);
	void OnResetRichEdit(void);

public:
	void OnNotFind(const int &nID);
	void OnFind(const int &nID, const std::string &strID, const std::string &strNickName);
};