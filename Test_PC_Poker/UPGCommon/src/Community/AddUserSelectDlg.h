#pragma once

class UPG_COMMON_EXPORT CAddUserSelectDlg : public LSY::CLSYLibDialogBase
{
protected:
	LSY::CList		*m_pListBuddy;
	LSY::CCheckBox	*m_pCheckAllBuddy;

	LSY::CList		*m_pListConnectUser;
	LSY::CCheckBox	*m_pCheckAllConnectUser;
	LSY::CSprite	*m_pSprLoding;

	LSY::CCombo		*m_pComboGame;
	LSY::CCombo		*m_pComboMoney;

	LSY::CWindows	*m_pWndBack;

	std::vector<MONEYRANGE>	m_stMoneyRange;

	int						m_nCurSortType;
	std::map<int, bool>		m_mapSortTypeIncrement;

	bool					m_bShowBuddy;
	MAPSTRING				m_mapIDAndNickName;
public:
	CAddUserSelectDlg(const bool &bShowBuddy, CWnd* pParent = NULL);
	virtual ~CAddUserSelectDlg();

	virtual bool OnCreatedProject(void);
	virtual LSY::CObjectBase *OnCallCreateObject(const int &nID, const std::string &strName);

	LRESULT OnClickButton(LSY::CMessage *pMsg);
	LRESULT OnCheckAllChange(LSY::CMessage *pMsg);
	LRESULT OnSelChangeGame(LSY::CMessage *pMsg);
	LRESULT OnSelChangeMoney(LSY::CMessage *pMsg);

	void ClearAllConnectUserCheck() ;
	void ClearAllBuddyCheck() ;

	void Sort(int nSortType);
	void SortDefault(void);

	virtual INT_PTR DoModal(CWnd *pParent);

	void SetComboSynchronization(IDX_GAME nSelGame, int nSelectMoney);

	void ResetGameComboBox(IDX_GAME nGame, INT64 i64MyMoney);
	void ResetMoneyComboBox(IDX_GAME nGame, INT64 i64MyMoney);

	void RemoveAllConnectUser(void);
	void ShowLoading(bool bShow);
	void AddConnectUser(SMALLUSERINFO *pUserInfo);
	void DelConnectUser(SMALLUSERINFO *pUserInfo);
	void ChangeConnectUser(SMALLUSERINFO *pUserInfo);

	void SetCurConnectUserList(void);

	bool IsCheckBuddyAllSelectItem(void);
	void SetCheckBuddyAllSelectItem(bool bCheck);

	bool IsCheckConnectUserAllSelectItem(void);
	void SetCheckConnectUserAllSelectItem(bool bCheck);

	const MAPSTRING &GetSelectIDAndNickName(void){return m_mapIDAndNickName;}
protected:
	DECLARE_MESSAGE_MAP()

	void OnCheckSelectNickName(void);
	void OnAddIDAndNIckName(const std::string &strID, const std::string &strNickName);
};