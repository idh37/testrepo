#pragma once

#include "PromotionBase.h"
#include <EventCommon.h>

class UPG_GAMEBASE_EXPORT CPromotionJackpotRelease : public CPromotionBase
{
public:

	CPromotionJackpotRelease( CPromotionManager *pManager, int nType );
	virtual~ CPromotionJackpotRelease();


protected:

	NMBASE::GRAPHICGDI::xSprite	m_sprEvent;
	
	CRect m_rtInfo;	

	bool m_bEventShow;
	int m_AniIndex;		//��Ȳ�� ��Ƽ�� �ε���
	int m_AniIndex_DP;	

	int m_JackPokWinnerEft;	//���� ��÷�ڰ� �ִ�. ( 0 : ����Ʈ ������ ����, 1 : ����Ʈ ������, 2 : ����Ʈ ���� )

	//
	BOOL m_bReleaseInfo;
	DWORD m_dwRelaseInfoTime;

	//���콺 ������
	BOOL m_bMouseOver;
	DWORD m_dwMouseTimer;

	jackpot::JP_EVENT_GIFTTIME m_Jackpot_Info;

public:	

	void SetEventInfo();

	virtual void OnStartGame();
	virtual void Accept_OutRoom();
	virtual void Accept_CreateRoom();

	virtual BOOL OnMouseMoveBottom(int &x , int &y);

	virtual BOOL OnPacketNotify(char* pName, int &Signal, int &TotSize, char *lpData);

	virtual void DrawBottom(CDC *pDC, NMBASE::GRAPHICGDI::CPage *pPage);
	virtual void DrawMiddle(CDC *pDC, NMBASE::GRAPHICGDI::CPage *pPage);
	virtual void DrawTop(CDC *pDC, NMBASE::GRAPHICGDI::CPage *pPage);

	virtual const bool IsShowTab(void);

};
