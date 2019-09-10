#pragma once
#include "PromotionBase.h"
#include <EventCommon.h>


class CEffectSpecialJackpot
{
public:
	CEffectSpecialJackpot()
	{
		m_bShowEffect=FALSE;
		m_bEffectMulti=FALSE;
		m_dwEffectStartTime=0;
		m_dwEffectTotalPlayTime=0;
		m_pSprEffect=NULL;

		m_nPosX = 0;
		m_nPosY = 0;

	}
	virtual ~CEffectSpecialJackpot()
	{

	}

public:
	DWORD m_dwEffectStartTime;	
	DWORD m_dwEffectTotalPlayTime;	

protected:
	BOOL m_bShowEffect, m_bEffectMulti;


	NMBASE::GRAPHICGDI::xSprite	*m_pSprEffect;					//����Ʈ spr
	int m_nEffectNumber;
	int m_nPosX;
	int m_nPosY;

	DWORD dwPreTime ; 
	DWORD dwNowTime ;

public:

	void SetSprite( NMBASE::GRAPHICGDI::xSprite *pSprEffect)
	{
		m_pSprEffect = pSprEffect;
	}

	void SetPos(int nPosX, int nPosY)
	{
		m_nPosX = nPosX;
		m_nPosY = nPosY;
	}

	//������ ����Ʈ�� �ʱ�ȭ�Ѵ�.
	void ShowEffect(int nEffectNumber, BOOL bEffectMulti = FALSE )
	{
		m_bShowEffect=TRUE;
		m_bEffectMulti = bEffectMulti ;
		m_dwEffectStartTime=timeGetTime();
		dwPreTime = dwNowTime = m_dwEffectStartTime ;
		m_nEffectNumber = nEffectNumber;

		if(m_bEffectMulti == TRUE)
			m_dwEffectTotalPlayTime = m_pSprEffect->GetMScenePlayTime(m_nEffectNumber);
		else if(m_bEffectMulti == FALSE)
			m_dwEffectTotalPlayTime = m_pSprEffect->GetScenePlayTime(m_nEffectNumber);
	}

	void ResetEffect()
	{
		m_bShowEffect=FALSE;
		m_dwEffectStartTime=timeGetTime();
		m_dwEffectTotalPlayTime = 0;
		m_nEffectNumber = 0;
	}

	//�ð��� ���� �������� �̹����� �����ش�.
	void Draw(CDC *pDC, NMBASE::GRAPHICGDI::CPage *pPage)
	{
		if (m_pSprEffect==NULL || m_bShowEffect==FALSE)
			return;

		dwPreTime = dwNowTime ;
		dwNowTime = timeGetTime();
		DWORD dwElapsed = dwNowTime - m_dwEffectStartTime;
		if (m_dwEffectTotalPlayTime < dwElapsed)
		{
			ResetEffect();
			return;
		}

		if(m_bEffectMulti == TRUE ){
			pPage->DrawMultiScene(0, 0, m_pSprEffect, m_nEffectNumber, dwElapsed, FALSE);

			//// ���� ���� ����
			//NMBASE::GRAPHICGDI::QUERYMSCENE_SOUND sndquery;
			//sndquery.Reset();
			//while(m_pSprEffect->QueryMSceneSound(&sndquery, m_nEffectNumber, dwPreTime - m_dwEffectStartTime, dwNowTime - m_dwEffectStartTime, TRUE))
			//{
			//	// ��ϵ��� ���� ���� �����̸� ���
			//	if(!NMBASE::SOUND::g_Sound.IsRegistered(sndquery.SndFile)) 
			//		NMBASE::SOUND::g_Sound.RegistWav(sndquery.SndFile, NMBASE::SOUND::SNDPRIORITY_MID, 1, TRUE);

			//	// ���� ����
			//	NMBASE::SOUND::g_Sound.PlayWav(sndquery.SndFile, FALSE, sndquery.Volume);
			//}
		}		
		else if(m_bEffectMulti == FALSE ){
			pPage->DrawScene(m_nPosX, m_nPosY, m_pSprEffect, m_nEffectNumber, dwElapsed, FALSE);
		}

	}

	BOOL IsShow()
	{
		return m_bShowEffect;
	}
};

struct WINNERDATA
{
	int nChannel ;
	int nRoomNum ;
	char szMyChannelID[NICK_LEN] ;
};



class CPromotionSpecialJackpot :	public CPromotionBase
{
	enum JackpotTimeState
	{
		State_NoJPTime = 0,
		State_JPTimeReady_Start, // ���� Ÿ�� �غ�.
		State_JPTimeStart_notCount, // ���� Ÿ���� ���ۉ�����, ���� �� �� ����.
		State_JPTimeStart_Count, // ���� Ÿ�� ������ ù �� ����. ����ī��Ʈ ����.
		State_JPTimeEnd_Count,  // ���� Ÿ�� ���� ������, ���� �� �� ����.
		State_JPTimeEnd_notCount, // ���� Ÿ�� ���� �� �� ����. 
		State_JPRoomChoice, // �漱�� ��� ��.
		State_JPRoomChoiceEnd, // �� ������. ���¿� ���� ���̹� ���� �ƴ��� ����.
		State_JPRoom_Start, // ������ ������ ����.
		State_JPRoom_End, // ������ ����. ��÷�� �ݾ� ����.
		State_JPRoomNoMyChannel, //���̹� ���� ��ü�� �ȵ� ä��.

		State_Max,
	};
public:
	CPromotionSpecialJackpot(CPromotionManager *pManager, int nType);
	~CPromotionSpecialJackpot(void);


protected:
	NMBASE::GRAPHICGDI::xSprite m_sprJackpotBack;			//��������Ʈ
	NMBASE::GRAPHICGDI::xSprite m_sprJackpotInform;			//��������Ʈ
	NMBASE::GRAPHICGDI::xSprite m_sprJackpotNum;			//��������Ʈ
	
	promotion::_HALFJACKPOT_ALARM m_Jackpot_Info;
	promotion::HALFJACKPOT_MONEY m_Jackpot_MoneyInfo ;

	////Inner Class
	CEffectSpecialJackpot m_cBackEffect;
	CEffectSpecialJackpot m_cAlertEffect;

	bool m_bIsJackpotRoom ;
	bool m_bisShowWinner ;

	bool m_bIsShowJackpotmoney ; // ���� ���� �Ӵϰ� ���̰� �ִ���..

	bool m_bIsShowRewardJPMoneyEffect ; // ���� �Ӵ� ���� �˸� ����Ʈ 

	bool m_bIsHalfCountStart ;
	bool m_bIsShowStartEffect ;
	bool m_bIsShowEndEffect ;

	bool m_bIsShowJPRoomEndEffect ;

	bool m_bIsShowEndText; // ���� ���� ��÷�� ���� �� Text ���� ������..

	DWORD	m_dwStartEndTextTime ;
	DWORD	m_dwNowEndTextTime ;

	int  m_nShowEndTextCount ;

	int  m_nBetRuleHalfCountCheck ; // �ڵ� ������ xx1, xx2 �� ��� �׸�ŭ�� ī��Ʈ ���� �Ƚ�Ŵ.

	int  m_nRewardJPMoneyCount ;

	int	 m_nJPTimeState ;

	int	 m_nPlayTime ; // ���� ���� �ð� ( �غ� �ð��̰ų� ���� ��Ʈ���� Ÿ���̰ų� )
	int  m_nPreTime ; // ���� �ٲ� ���� ����ϱ� ���� ���� �ð�.

	int  m_nBestHalfCount ;
	int  m_nMyhalfCount ;

	int	 m_nBackSpriteCount ;
	int  m_nMyChannelRoomNum ;

	int m_nWinnerNum ;
	int m_nJPRoomNotiCount ;

	INT64	m_llhalfJackpotMoney ;

	int  m_nJackpotMoney[19] ;

	int  m_nChannel ;

	int  m_nRoomNum[7][MAX_TOP_HALF_ROOM] ;
	int  m_nWinDataCount ;
	WINNERDATA  m_stWinnerData[46] ;

	char m_szMyChannelID[NICK_LEN] ; // ID ���̺��� �г��� ���̰� �� ��� ������ �г��ӱ�������..
	char m_szMyChannel[16] ; // ���� ä���� ���ڿ��� ��ȯ.
	char m_szJackpotMoney[128] ;

	//��ư�̺�Ʈ ����
	BOOL CPromotionSpecialJackpot::OnLButtonDownBottom(int &x , int &y);
	BOOL CPromotionSpecialJackpot::OnMouseMoveBottom(int &x , int &y);

	void Reset() ;
	
	// ���ӹ泻 �غ� ��� �̹����� �� �� ���� �ٲ�� ����
	void setRoomReadyBackState(int nTime) ;

	// ���ӹ泻 ���� Ÿ�� ��� �̹����� �� �� ���� �ٲ�� ����
	void setRoomJackpotTimeBackState(int nTime) ;

	void setJackpotMoneyInRoom() ;
	void setJackpotMoney(promotion::HALFJACKPOT_MONEY* pMoney) ;
	INT64 ChangejackpotMoney(INT64 jackpotMoney) ;

	////����Ʈ �Լ�
	// ���� Ÿ�� ���۰� �� ����Ʈ
	void ShowJackpotTimeStartEffect();
	void ShowJackpotTimeEndEffect();

	// ���� Ÿ�� ���� ������ ���� �� ���۰� �� ����Ʈ.
	void ShowJackpotGameStartEffect();
	void ShowJackpotGameEndEffect();

	// ���� �� ���� �� �� �÷��̾�� �Ӵ� ������ �˸��� ����Ʈ.
	void ShowRewardJPMoneyEffect();
	void HideRewardJPMoneyEffect();

	// ���� �� ����.
	void SetJackpotRoom(promotion::HALFJACKPOT_ROOM* pRoom ) ;

	// ���� ��÷ iD ��ȣ�� ���� ���� 4�ڸ��� ����.
	void convertIDtoProtect(char* str) ;

	// ���� � ä������ ��ȯ�ϴ� ���ڿ�.
	void setChannelString(INT64 LimitEnterMoney) ;

	void convertJPMoneytoStr(INT64 jackpotmoney);

public:

	virtual void Accept_CreateRoom();
	virtual void Accept_EnterRoom();
	virtual void Accept_OutRoom();
	virtual BOOL OnPacketNotify(char* pName, int &Signal, int &TotSize, char *lpData);
	
	void Clear_Info();	
	void SetEventInfo( promotion::_HALFJACKPOT_ALARM *pInfo );	//�� ����� ����
	
	virtual const bool IsShowTab(void) { return false; }
	virtual void DrawTop(CDC *pDC, NMBASE::GRAPHICGDI::CPage *pPage);
	
	void OnTimer();

	void setHalfCountPlus() ;

	virtual void OnStartGame();
	virtual void OnEndGame();	//������ ������ ���� �ɶ�.

	// ���� ��ȣ ����.
	void SetWinnerNum(int WinnerNum) 
	{ 
		m_nWinnerNum = WinnerNum; 
	}
};



