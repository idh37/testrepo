// OwnLimit.h: interface for the COwnLimit class.
//
//////////////////////////////////////////////////////////////////////
#pragma once

#include "OwnLimitDlg.h"

struct LimitMoneyInfo
{
	
	INT64 llMoney;
	COleDateTime Expire;

	BOOL IsExpired()
	{
		if (Expire.GetStatus()==COleDateTime::invalid)
			return TRUE;

		return (g_GetSyncTime()>Expire);
	}

	BOOL Set(INT64 money, SYSTEMTIME *pExpire)
	{
		if (IsExpired())	
		{
			llMoney=money;
			Expire = *pExpire;
			return TRUE;
		}
		else
		{
			if (llMoney<money)
			{
				llMoney=money;			
				Expire = *pExpire;
				return TRUE;
			}
		}	
		return FALSE;
	}

	LimitMoneyInfo()
	{	
		llMoney=0;	
	}
};

class CUIInfoData;

class UPG_GAMEBASE_EXPORT COwnLimit  
{
protected:
	class EFFECT_PLAY_INFO
	{
	public:
		DWORD	m_dwStartTime;
		DWORD	m_dwEndTime;
	public:
		EFFECT_PLAY_INFO():m_dwStartTime(0),m_dwEndTime(0){}
		virtual ~EFFECT_PLAY_INFO(){}
	};

	INT64			m_i64MaxPremiumMoneyWithOption;				//���ӿ��� ������ ���� �� �ִ� �ִ�Ӵ� (�ְ��йи� + �ɼǰ���)
	LimitMoneyInfo	m_stOverMoneyInfo;
	LimitMoneyInfo	m_stRevisionMoneyInfo;
	INT64			m_i64MaxPremiumGoldWithOption;				//���ӿ��� ������ ���� �� �ִ� �ִ��� (�ְ��йи� + �ɼǰ���)
	LimitMoneyInfo	m_stOverGoldInfo;
	LimitMoneyInfo	m_stRevisionGoldInfo;
	INT64			m_i64MaxPremiumGoldBankWithOption;				//���ӿ��� ������ ���� �� �ִ� �ִ��� ������ (�ְ��йи� + �ɼǰ���)
	LimitMoneyInfo	m_stOverGoldBankInfo;
	LimitMoneyInfo	m_stRevisionGoldBankInfo;

	map<CString, LimitMoneyInfo>	m_mapOtherUserInfo;			//�ٸ� ������ ����
	map<CString, LimitMoneyInfo>	m_mapOtherUserInfoGold;		//�ٸ� ������ ����
	map<int, EFFECT_PLAY_INFO>		m_mapEffectPlayInfo;
	NMBASE::GRAPHICGDI::xSprite							m_sprHandoEffect;
	NMBASE::GRAPHICGDI::xSprite							m_sprHando;
	bool							m_bShowMark;
	bool							m_bShowMarkGold;
	bool							m_bShowMarkGoldBank;

	CUIInfoData						*m_pUIInfoData;
	CGameProcess					*m_pGameProcess;
	CGameViewBase					*m_pGameView;
	NMBASE::GRAPHICGDI::CPage							*m_pPage;
	bool							m_bMouseOver;
	CString							m_strMoney;
	CString							m_strGold;
	CRect							m_rtMark;


	COwnLimitDlg m_dlgOwnLimit;
public:
	COwnLimit();
	virtual ~COwnLimit();

	virtual void LoadImage(LPCTSTR lpEffectPath);

	virtual void SetMaxPremiumMoneyWithOption(INT64 llMoney)
	{
		m_i64MaxPremiumMoneyWithOption=llMoney;
	}
	virtual void SetMaxPremiumGoldWithOption(INT64 llGold)
	{
		m_i64MaxPremiumGoldWithOption=llGold;
	}
	virtual void SetMaxPremiumGoldBankWithOption(INT64 llGold)
	{
		m_i64MaxPremiumGoldBankWithOption=llGold;
	}

	virtual void Init(CUIInfoData *pInfoData, CGameProcess *pProcess);
	virtual void SetOverMoney(INT64 llMoney, int chipKind, SYSTEMTIME *pExpire);
	virtual void SetRevisionMoney(INT64 llMoney, int chipKind, SYSTEMTIME *pExpire);
	virtual void SetOtherUserOverMoney(const char *pID, INT64 llMoney, int chipKind, SYSTEMTIME *pExpire);

	virtual void ShowPopup();
	virtual void ShowAniEffect(int p);
	virtual void CheckExpire();
	virtual void ResetNotToday(const char *pMyID);

	virtual void DrawBottom(void);
	virtual void DrawTop(CDC *pDC);

	virtual void OnEnterRoom(CUIInfoData *pInfoData, CGameProcess *pProcess);
	virtual void OnExitRoom(void);
	virtual void OnGameOver(void);

	virtual BOOL OnMouseMove(int x , int y);

	BOOL OnPacketNotify(char* pName, int Signal, int TotSize, char *lpData);


	BOOL OnReceiveUserInfo(int TotSize, char *lpData);					//���� ����â�� ���ӹ�/���� �����ϰ� �����ʿ� �������� ������ �����ʿ��� ����.

	BOOL OnReceiveNotifyChangeOverMoney(int TotSize, char *lpData);
};