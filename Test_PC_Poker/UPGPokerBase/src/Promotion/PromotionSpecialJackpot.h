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


	NMBASE::GRAPHICGDI::xSprite	*m_pSprEffect;					//이펙트 spr
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

	//실제로 이펙트를 초기화한다.
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

	//시간에 따른 프레임의 이미지를 보여준다.
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

			//// 사운드 파일 쿼리
			//NMBASE::GRAPHICGDI::QUERYMSCENE_SOUND sndquery;
			//sndquery.Reset();
			//while(m_pSprEffect->QueryMSceneSound(&sndquery, m_nEffectNumber, dwPreTime - m_dwEffectStartTime, dwNowTime - m_dwEffectStartTime, TRUE))
			//{
			//	// 등록되지 않은 사운드 파일이면 등록
			//	if(!NMBASE::SOUND::g_Sound.IsRegistered(sndquery.SndFile)) 
			//		NMBASE::SOUND::g_Sound.RegistWav(sndquery.SndFile, NMBASE::SOUND::SNDPRIORITY_MID, 1, TRUE);

			//	// 사운드 연주
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
		State_JPTimeReady_Start, // 잭팟 타임 준비.
		State_JPTimeStart_notCount, // 잭팟 타임은 시작됬지만, 이전 방 미 종료.
		State_JPTimeStart_Count, // 잭팟 타임 시작후 첫 판 시작. 하프카운트 시작.
		State_JPTimeEnd_Count,  // 잭팟 타임 종료 됬지만, 이전 방 미 종료.
		State_JPTimeEnd_notCount, // 잭팟 타임 종료 후 판 종료. 
		State_JPRoomChoice, // 방선정 대기 중.
		State_JPRoomChoiceEnd, // 방 선정됨. 상태에 따라 잭팟방 인지 아닌지 노출.
		State_JPRoom_Start, // 선정된 잭팟판 시작.
		State_JPRoom_End, // 잭팟판 종료. 당첨자 금액 노출.
		State_JPRoomNoMyChannel, //잭팟방 선정 자체가 안된 채널.

		State_Max,
	};
public:
	CPromotionSpecialJackpot(CPromotionManager *pManager, int nType);
	~CPromotionSpecialJackpot(void);


protected:
	NMBASE::GRAPHICGDI::xSprite m_sprJackpotBack;			//스프라이트
	NMBASE::GRAPHICGDI::xSprite m_sprJackpotInform;			//스프라이트
	NMBASE::GRAPHICGDI::xSprite m_sprJackpotNum;			//스프라이트
	
	promotion::_HALFJACKPOT_ALARM m_Jackpot_Info;
	promotion::HALFJACKPOT_MONEY m_Jackpot_MoneyInfo ;

	////Inner Class
	CEffectSpecialJackpot m_cBackEffect;
	CEffectSpecialJackpot m_cAlertEffect;

	bool m_bIsJackpotRoom ;
	bool m_bisShowWinner ;

	bool m_bIsShowJackpotmoney ; // 현재 잭팟 머니가 보이고 있는지..

	bool m_bIsShowRewardJPMoneyEffect ; // 잭팟 머니 지급 알림 이펙트 

	bool m_bIsHalfCountStart ;
	bool m_bIsShowStartEffect ;
	bool m_bIsShowEndEffect ;

	bool m_bIsShowJPRoomEndEffect ;

	bool m_bIsShowEndText; // 현재 잭팟 당첨자 선정 후 Text 노출 중인지..

	DWORD	m_dwStartEndTextTime ;
	DWORD	m_dwNowEndTextTime ;

	int  m_nShowEndTextCount ;

	int  m_nBetRuleHalfCountCheck ; // 자동 배팅이 xx1, xx2 일 경우 그만큼은 카운트 증가 안시킴.

	int  m_nRewardJPMoneyCount ;

	int	 m_nJPTimeState ;

	int	 m_nPlayTime ; // 현재 남은 시간 ( 준비 시간이거나 현재 하트잭팟 타임이거나 )
	int  m_nPreTime ; // 분이 바뀔 때를 계산하기 위한 이전 시간.

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

	char m_szMyChannelID[NICK_LEN] ; // ID 길이보다 닉네임 길이가 더 길기 때문에 닉네임기준으로..
	char m_szMyChannel[16] ; // 현재 채널을 문자열로 변환.
	char m_szJackpotMoney[128] ;

	//버튼이벤트 관련
	BOOL CPromotionSpecialJackpot::OnLButtonDownBottom(int &x , int &y);
	BOOL CPromotionSpecialJackpot::OnMouseMoveBottom(int &x , int &y);

	void Reset() ;
	
	// 게임방내 준비 배경 이미지가 매 분 마다 바뀌도록 설정
	void setRoomReadyBackState(int nTime) ;

	// 게임방내 잭팟 타임 배경 이미지가 매 분 마다 바뀌도록 설정
	void setRoomJackpotTimeBackState(int nTime) ;

	void setJackpotMoneyInRoom() ;
	void setJackpotMoney(promotion::HALFJACKPOT_MONEY* pMoney) ;
	INT64 ChangejackpotMoney(INT64 jackpotMoney) ;

	////이펙트 함수
	// 잭팟 타임 시작과 끝 이펙트
	void ShowJackpotTimeStartEffect();
	void ShowJackpotTimeEndEffect();

	// 잭팟 타임 이후 선정된 잭팟 방 시작과 끝 이펙트.
	void ShowJackpotGameStartEffect();
	void ShowJackpotGameEndEffect();

	// 잭팟 방 종료 후 각 플레이어에게 머니 지급을 알리는 이펙트.
	void ShowRewardJPMoneyEffect();
	void HideRewardJPMoneyEffect();

	// 잭팟 방 선정.
	void SetJackpotRoom(promotion::HALFJACKPOT_ROOM* pRoom ) ;

	// 잭팟 당첨 iD 보호를 위해 앞의 4자리만 노출.
	void convertIDtoProtect(char* str) ;

	// 현재 어떤 채널인지 변환하는 문자열.
	void setChannelString(INT64 LimitEnterMoney) ;

	void convertJPMoneytoStr(INT64 jackpotmoney);

public:

	virtual void Accept_CreateRoom();
	virtual void Accept_EnterRoom();
	virtual void Accept_OutRoom();
	virtual BOOL OnPacketNotify(char* pName, int &Signal, int &TotSize, char *lpData);
	
	void Clear_Info();	
	void SetEventInfo( promotion::_HALFJACKPOT_ALARM *pInfo );	//판 진행시 마다
	
	virtual const bool IsShowTab(void) { return false; }
	virtual void DrawTop(CDC *pDC, NMBASE::GRAPHICGDI::CPage *pPage);
	
	void OnTimer();

	void setHalfCountPlus() ;

	virtual void OnStartGame();
	virtual void OnEndGame();	//게임이 끝나고 리셋 될때.

	// 승자 번호 저장.
	void SetWinnerNum(int WinnerNum) 
	{ 
		m_nWinnerNum = WinnerNum; 
	}
};



