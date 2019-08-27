#pragma once
#include "ChanceButton.h"
#include "../ChanceTooltip/ChanceTooltips.h"

class UPG_LOBBY_EXPORT CTabGame : public LSY::CTabControl
{
protected:
	LSY::CButton							*m_pBtnPokerShop;
	LSY::CButton							*m_pBtnCreateRoom;
	LSY::CButton							*m_pBtnShocking8;
	CChanceButton							*m_pBtnMade;
	CChanceButton							*m_pBtnRegen;
	   
//	LSY::CSpriteWindows						*m_pWindowsChance;//신규 프로모션 스프라이트 적용(신평호)

	
	LSY::CTextButton						*m_pTxtBtnChance;

	std::map<IDX_GAME, LSY::CRadio *>		m_listGameTab;
	std::map<IDX_GAME, bool>				m_listGameMaintenance;
	bool									m_bAdmin;
	
	std::map<IDX_GAME, vector<int> >		m_listGameTabStatusIndex;

	//규제안 : 이용제한 버튼
	LSY::CButton							*m_pBtnUseLimit;
	//yoo 2015.01 규제안
	LSY::CImage								*m_pUseLimit;
	//shisy 2018.02 설 이벤트
	LSY::CButton							*m_pBtnGoldBack;

	LSY::CButton* m_pChuSeokAttendance;

	//int										m_nRewardItemCode;
	//COleDateTime							m_timeExpire;
	//DWORD									m_dwLastUpdateTime;
	//BOOL									m_bEventChanceActive;

	
public:
	CTabGame();
	virtual ~CTabGame();

	virtual void OnCreateChild(LSY::CObjectBase *pObject);
	virtual void OnCreated(void);

	void SetGameTab(IDX_GAME nGame);
	bool ChangeLimitGameTab();
	void ChangeLimitGameBtns(bool bState);

	//게임텝에 점검중 표시해주기
	virtual void SetMaintenance(IDX_GAME nGame, bool bMaintenance, bool bAdmin);
	virtual bool IsMaintenance(IDX_GAME nGame);

	void SetChanceInfo(CRewardItemMan::REWARD_TYPE eRewardType, int nRewardItemCode, COleDateTime &timeExpire );	
	void ClearChance(CRewardItemMan::REWARD_TYPE eRewardType);
	LRESULT OnClickButton(LSY::CMessage *msg);
	LRESULT OnMouseOver(LSY::CMessage *msg);
	LRESULT OnMouseLeave(LSY::CMessage *msg);
	LRESULT OnSelChangeGame(LSY::CMessage *msg);
	LRESULT OnShowChuseokAttendanceBtn(WPARAM &wParam, LPARAM &lParam);

	//신평호 프로모션용 테스트
	void SetEventChance(BOOL sender);

	//규제안 : 입장제한 버튼 활성화 여부 함수
	void SetShowUseLimitBtn(bool bShow);
	
	//규제안 : 입장제한 금액 안내 팝업
	void ShowUseLimitDlg();
	void ShowUseLimitOverDlg();

	void SetShowGoldBackBtn(bool bShow);

	IDX_GAME		m_nCurrentTab;
};