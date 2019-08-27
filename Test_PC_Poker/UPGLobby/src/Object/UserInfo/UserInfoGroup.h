#pragma once

class CUserInfoDetailDlg;
class UPG_LOBBY_EXPORT CUserInfoGroup : public LSY::CGroup
{
protected:
	LSY::CAvatar		*m_pAvatar;
	LSY::CText			*m_pNickName;
	LSY::CImageNumber	*m_pHaveMoney;
	LSY::CTextNumber	*m_pHaveGold;
	LSY::CImage			*m_pPopup;

	LSY::CImage			*m_pCommunityDisconnect;
	LSY::CImage			*m_pImgCommunityMemoCount;
	LSY::CButton		*m_pBtnMemoCount;
	LSY::CTextNumber	*m_pTxtMemoCount;

	LSY::CImage			*m_pCommunityDisconnectBuddy;
	LSY::CImage			*m_pImgCommunityConnectBuddyCount;
	LSY::CButton		*m_pBtnConnectBuddyCount;
	LSY::CTextNumber	*m_pTxtConnectBuddyCount;


	LSY::CTextNumber	*m_pHaveTotalMoney;
	LSY::CTextNumber	*m_pChangeMoney;
	LSY::CText			*m_pMyScore;
	LSY::CButton		*m_pBtnCharge;
	LSY::CTextButton	*m_pBtnGoldText;
	LSY::CButton		*m_pBtnDetailOpen;
	LSY::CButton		*m_pBtnEvent;
	LSY::CButton		*m_pBtnCafe;
	LSY::CButton		*m_pBtnWithdraw;
	LSY::CButton		*m_pBtnGoldChange;
	LSY::CButton		*m_pDailyGold;
	LSY::CImage			*m_pDailyGoldAlarm;
	LSY::CButton		*m_pBtnClub;
	CUserInfoDetailDlg	*m_pDetailInfo;

	INT64				m_i64LoginMoney;
	USERINFO			*m_pUserInfo;
	bool				m_bConnect;
	
public:
	CUserInfoGroup();
	virtual ~CUserInfoGroup();

	bool				m_bRefresh;

	virtual void OnCreateChild(LSY::CObjectBase *pObject);
	virtual void OnCreated(void);

	LRESULT OnClickButton(LSY::CMessage *msg);
	LRESULT OnMoneyMouseOver(LSY::CMessage *msg);
	LRESULT OnMoneyMouseLeave(LSY::CMessage *msg);
	void	SetMyUserInfo(USERINFO *pMyInfo, BOOL bLogin);
	void	SetMemoCount(int nMemoCount);
	void	SetConnectBuddyCount(int nBuddyCount);
	void	SetComunityConnect(bool bConnect);

	LRESULT	StartTimer(WPARAM &wParam, LPARAM &lParam);
	void	GetMemoCnt();
	LRESULT OnEndCall(WPARAM &wParam, LPARAM &lParam);
	LRESULT OnShowSendMessageDlg(WPARAM &wParam, LPARAM &lParam);
	LRESULT OnSetMoneyNum(WPARAM &wParam, LPARAM &lParam);
	LRESULT OnSetLobbyButtonEnable(WPARAM &wParam, LPARAM &lParam);

	void	ShowRoomList(bool bShow);
	void	SetEnableBtnDetailOpen(bool bEnable);
};