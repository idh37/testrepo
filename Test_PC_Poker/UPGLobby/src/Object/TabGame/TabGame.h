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
	   
//	LSY::CSpriteWindows						*m_pWindowsChance;//�ű� ���θ�� ��������Ʈ ����(����ȣ)

	
	LSY::CTextButton						*m_pTxtBtnChance;

	std::map<IDX_GAME, LSY::CRadio *>		m_listGameTab;
	std::map<IDX_GAME, bool>				m_listGameMaintenance;
	bool									m_bAdmin;
	
	std::map<IDX_GAME, vector<int> >		m_listGameTabStatusIndex;

	//������ : �̿����� ��ư
	LSY::CButton							*m_pBtnUseLimit;
	//yoo 2015.01 ������
	LSY::CImage								*m_pUseLimit;
	//shisy 2018.02 �� �̺�Ʈ
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

	//�����ܿ� ������ ǥ�����ֱ�
	virtual void SetMaintenance(IDX_GAME nGame, bool bMaintenance, bool bAdmin);
	virtual bool IsMaintenance(IDX_GAME nGame);

	void SetChanceInfo(CRewardItemMan::REWARD_TYPE eRewardType, int nRewardItemCode, COleDateTime &timeExpire );	
	void ClearChance(CRewardItemMan::REWARD_TYPE eRewardType);
	LRESULT OnClickButton(LSY::CMessage *msg);
	LRESULT OnMouseOver(LSY::CMessage *msg);
	LRESULT OnMouseLeave(LSY::CMessage *msg);
	LRESULT OnSelChangeGame(LSY::CMessage *msg);
	LRESULT OnShowChuseokAttendanceBtn(WPARAM &wParam, LPARAM &lParam);

	//����ȣ ���θ�ǿ� �׽�Ʈ
	void SetEventChance(BOOL sender);

	//������ : �������� ��ư Ȱ��ȭ ���� �Լ�
	void SetShowUseLimitBtn(bool bShow);
	
	//������ : �������� �ݾ� �ȳ� �˾�
	void ShowUseLimitDlg();
	void ShowUseLimitOverDlg();

	void SetShowGoldBackBtn(bool bShow);

	IDX_GAME		m_nCurrentTab;
};