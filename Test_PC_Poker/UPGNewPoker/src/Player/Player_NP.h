// Player.h: interface for the CPlayer_NP class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PLAYER_NP_H__A4DF4943_11B0_11D4_97A2_0050BF0FBE67__INCLUDED_)
#define AFX_PLAYER_NP_H__A4DF4943_11B0_11D4_97A2_0050BF0FBE67__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <../src/Player.h>

#include "UserStruct.h"
#include "MyCard.h"

#define CARD_TOTAL 8

#define LR		((DWORD)1)		// 0000000000000001(b) (좌우반전)
#define UD		((DWORD)2)		// 0000000000000010(b) (상하반전)
#define HB		((DWORD)4)		// 0000000000000100(b) (반투명효과)

#define 		MONEY_CHIP_NONE				((DWORD)1)
#define 		MONEY_CHANGED				((DWORD)2)
#define 		CHIP_CHANGED				((DWORD)4)

typedef struct  {
	char	m_strVSID[16];

	int     m_nRound;
	int		m_nWinNum;
	int		m_nLooseNum;
	BOOL    m_bNewUser;

	void Clear(){
		ZeroMemory(m_strVSID , sizeof(m_strVSID));
		m_nWinNum = 0;
		m_nLooseNum = 0;
		m_bNewUser = FALSE;
		m_nRound = 0;
	}

	void SetVsUserID(char *id){
		ZeroMemory(m_strVSID,sizeof(m_strVSID));
		strcpy_s(m_strVSID , sizeof(m_strVSID), id);
		SetNewUserFlg();
	}
	void ADD_WinNum(){
		m_nWinNum += 1;
	}
	void ADD_LooseNum(){
		m_nLooseNum += 1;
	}
	void ADD_RoundNum(){
		m_nRound ++;
	}
	int GetWinInfo(){
		return m_nWinNum;
	}
	int GetLooseInfo(){
		return m_nLooseNum;
	}
	int GetRoundNum(){
		return m_nRound;
	}
	BOOL IsNewUser(){
		return m_bNewUser;
	}
	void SetNewUserFlg(){
		m_bNewUser = TRUE;
	}
	void ClearNewUserFlg(){
		m_bNewUser = FALSE;
	}
	int  GetVS_IDLength(){
		return strlen(m_strVSID);
	}
}VSRESULTINFO;	//vsmode일때 승패 정보 방에서 같이 플레이 하는 사람에 대한 정보다


class CPlayer_NP  : public CPlayer
{
public:
	int				m_nCardTotal;
	int				m_nOpenTotal;

	INT64			m_i64MyJackpatMoney;	// 결과가 늦어져서 나중에 게임 결과때 추가로 넣어 준다.	

	int				m_nFoldRound;
	BOOL			m_bHiddenOpen;
	
	int				m_nThrowAwayCardNo;		//버린카드

	CMyBetChip		m_cMyChip;

	/////////////////카드 관련
	CMyCard			m_listCard[8];		   // 카드저장 클래스 // ###[8포커]###			
	CRect			m_listCardRectEventEx[8];	
	
	BOOL			m_bCardOPenMove;		// 결과에서 카드 이동 플래그 
	BOOL			m_bChoiceCardMove;		// 결과에서 카드 이동 플래그 

	int				m_nSDelay;				// 사운드 딜레이	
	int				m_nTotalCardNum;

	BOOL			m_bLastHalf;			// 마지막에 하프를 쳤냐
	CPoint			m_ptPos;

	INT64			m_i64ResultMoney;

	//2006.3.20
	int					m_nCardChoiceLimtCnt;	//카드 선택을 자동으로 몇번 햇는지 3번 이상이면 강퇴 한다.

	// 잭팟이나 게임내 충전 이벤트성 골든칩을 받는 경우 m_GameOverResultData 에 의해 충전 전 데이타로 돌아갈수 있음.
	int					m_nFlagMoneyChipChanged;

	//히든카드관련
	BOOL					m_bHiddenScene;					
	DWORD					m_dwHiddenSceneDrawTime;

	CPlayer_NP();
	virtual ~CPlayer_NP();

	virtual void Clear();
	virtual void SetNewPlayer(USERINFO *pUI);
	virtual void Reset();

	void OnTimer();
	void InitRoomData();
	// 베팅 관련 
public:	
	void OnSetChip(INT64 money);
	void OnReSetBetDraw();
//	void OnDraw(CDC *pDC = NULL);

	// 카드 관련
public:		
	void OnCardDraw(NMBASE::GRAPHICGDI::CPage *pPage, BOOL bFold = FALSE, BOOL bFoldOpen = FALSE);
	void SetNewCard(int cardNum , BOOL bbomb = FALSE, BOOL bForceYP = FALSE);
	
	void SetResultFace(int index,BOOL bresult = FALSE);
	
	int  GetCardNo(int index);
	BOOL GetFace(int index);
	void SetFaceUp(int index);
	void SetFaceDown(int index);

	void OnDraw_Bottom(CDC *pDC);		
	void OnDraw_Middle(CDC *pDC);
	void OnDraw_Top(CDC *pDC);

	void DrawDummyHiddenCard(CDC *pDC);

	void SetGameResultCardOpen(int delay);	// 카드 오픈 (애니 처리 설정) 결과때
	void SetChioceCardMove(BOOL bdirect , int face = 0); // 초이스일때 face 0 : 일반 1은 나인데 뒷면 2는 나 인데 앞면 

	BOOL OverPtInCardRect(int xp,int yp);// 카드 영역을 클릭 했는지 
	int  EventOverPtInCardRect(CPoint pt);// 카드 영역에 있는지
	
	void SetFlag_Money_Chip( int nFlag );
	BOOL IsFlag_Money_Chip( int nFlag );

	void DrawEft_CompleteMadeValue(int oldpoint=0, bool bForce=false );

	//히든카드관련
	void HiddenOpne();
	int GetHiddenCardMoveWay( int xp, int yp );

	//max방에서 보우금액 업데이트를 위한 함수 추가
	INT64 GetNowMoney();
	void SetNowMoney( INT64 nMoney );
};

#endif // !defined(AFX_PLAYER_H__A4DF4943_11B0_11D4_97A2_0050BF0FBE67__INCLUDED_)
