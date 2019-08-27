#pragma once

class UPG_LOBBY_EXPORT CUserListGroup : public LSY::CGroup
{
protected:
	LSY::CCombo				*m_pComboGame;
	LSY::CCombo				*m_pComboMoney;
	LSY::CGroup				*m_pHeader;
	LSY::CList				*m_pList;
	LSY::CSprite			*m_pSprLoading;
	LSY::CEdit				*m_pEditFindUser;
	LSY::CButton			*m_pBtnFindUser;
	LSY::CEdit				*m_pEditFindRoomNumber;
	LSY::CButton			*m_pBtnFindRoomNumber;
	LSY::CButton			*m_pBtnHeaderNickName;
	LSY::CButton			*m_pBtnHeaderHaveMoney;
	LSY::CButton			*m_pBtnHeaderPosition;

	std::vector<MONEYRANGE>	m_stMoneyRange;

	int						m_nCurSortType;
	std::map<int, bool>		m_mapSortTypeIncrement;
	std::list<CAddUserSelectDlg *> m_listAddUserSelectDlg;

	//CMenu					m_cUserMenu;
	SMALLUSERINFO			m_stSelectUserInfo;
	std::string				m_strSelectID;
	std::string				m_strSelectNickName;
	int						m_nSelectRoomNum;

	DWORD					m_dwTime_refresh; // 카페 목록 갱신 시간

public:
	CUserListGroup();
	virtual ~CUserListGroup();

	virtual void OnCreateChild(LSY::CObjectBase *pObject);
	virtual void OnCreated(void);

	LRESULT OnClickButton(LSY::CMessage *msg);
	LRESULT OnSelChangeGame(LSY::CMessage *msg);
	LRESULT OnSelChangeMoney(LSY::CMessage *msg);
	LRESULT OnReturnEdit(LSY::CMessage *msg);
	LRESULT OnListItemDbClick(LSY::CMessage *msg);
	LRESULT OnListItemRClick(LSY::CMessage *msg);
	LRESULT OnSelChangeTabUser(LSY::CMessage *msg);
	LRESULT OnSelChangeTabFriend(LSY::CMessage *msg);

	LRESULT OnClickMenu(LSY::CMessage *pMsg);

	void ResetGameComboBox(IDX_GAME nGame, INT64 i64MyMoney);
	void ResetMoneyComboBox(IDX_GAME nGame, INT64 i64MyMoney);

	LRESULT OnAddUserSelectDlg(WPARAM &wParam, LPARAM &lParam);
	LRESULT OnSubUserSelectDlg(WPARAM &wParam, LPARAM &lParam);
	LRESULT OnIsShowLoadingUserList(WPARAM &wParam, LPARAM &lParam);
	LRESULT OnAddUserSelectDlgSelChangeGame(WPARAM &wParam, LPARAM &lParam);
	LRESULT OnAddUserSelectDlgSelChangeMoney(WPARAM &wParam, LPARAM &lParam);

	MONEYRANGE *GetCurrentUserListMoneyRange(void);
	IDX_GAME GetCurrentUserListGame();

	void ShowLoadingUser(bool bShow);
	void RemoveAll(void);
	void AddItem(SMALLUSERINFO *pUserInfo);
	void DelItem(SMALLUSERINFO *pUserInfo);
	void ChangeItem(SMALLUSERINFO *pUserInfo);

	void SetFocusFindUser(void);
	void SetFocusFindRoomNumber(void);

	SMALLUSERINFO *GetSelectedUserInfo(void);
	const std::string &GetSelectID(void){return m_strSelectID;}
	const std::string &GetSelectNickName(void){return m_strSelectNickName;}
	int GetSelectRoomNum(void){return m_nSelectRoomNum;}
	void SetSelectID(const std::string &strID);
	void SetSelectNickName(const std::string &strNickName);
	void SetSelectRoomNum(int nRoomNum); // 방 번호

protected:
	void Sort(int nSortType);
	void SortDefault(void);
	void SortShuffle(void);
	void FindUser(void);
	void FindRoomNumber(void);
};