// PlayerSutda.h: interface for the PlayerSutda class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PlayerSutda_H__83E277C5_E483_47C5_BA5A_E89EE2F1F912__INCLUDED_)
#define AFX_PlayerSutda_H__83E277C5_E483_47C5_BA5A_E89EE2F1F912__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "Define_Sutda.h"
#include "MyCard.h"
#include "AnimationDefine.h"
#include "SevenRuleMan.h"

// 캐릭터에 표현되는 타입
enum ECHARTYPE
{
	EHOST_TYPE  = 0,	// 일반방장
	ESHOST_TYPE = 1,	// 슈퍼방장
	EDEALER_TYPE= 2,	// 딜러(7Poker에서 사용 안함)
	EBOSS_TYPE  = 3,	// 보스
};


#define CARD_TOTALCOUNT_BUF		(3+4)	//$$$$$// 총 7장을 받지만 처음에 한장을 버린다.(카드딜링시 스프라이트 Index계산 연산에 사용을 한다.)
#define CARD_TOTALCOUNT   (g_RI.FormKind==SD_2CARD?2:3)

#define MAX_MY_SUTDA_CARD			(5)	//섯다작업


class CPlayerSutda  : public CPlayer
{
public:

	enum 
	{
		Banish_Vote=0,
		Banish_Force,
	};

	CPoint				m_ptAvatarStart;				// 아바타 시작 위치
	CPoint				m_ptCardStart;					// 카드 시작 위치

	int					m_nMyIpCheck;					// 중복된 IP
	
	INT64				m_n64ResultMoney;				// 결과머니
	
	int					m_nSDelay;						// 사운드 딜레이	

	BOOL				m_bLastHalf;					// 마지막에 하프를 쳤냐

	int					m_nCardChoiceLimtCnt;			// 카드 선택을 자동으로 몇번 햇는지 3번 이상이면 강퇴 한다.

	CMyBetChip			m_cMyChip;						// 내 배팅칩 던지는 애니

	CObjectList			m_cPlayerEffectList;			// 스프라이트, 이미지를 등록해서 처리 및 보여주는 역할

	CMyCard				m_cMyCard[CARD_TOTALCOUNT_BUF];		// 실제 플레이에 사용되는 카드 / ###[8포커]###			
	CRect				m_cCardRectEventEx[CARD_TOTALCOUNT_BUF];
	int					m_nTotalCardCount;

private:

	CString				m_stJokboName;					// 족보명

	BOOL				m_bResultCardOpenMove;			// 결과에서 카드 이동 플래그 

	int					m_nCardTotalCount;

	int					m_nOpenCardTotalCount;

	int					m_nThrowCardNum;				// 초이스 모드일때 버린카드 정보

	BOOL				m_bHiddenScene;					
	DWORD				m_dwHiddenSceneDrawTime;

	BOOL				m_bResultCardOpenAni;		//결과시 카드 펼치는 애니메이션이 진행중인가? (모든유저의 카드 오픈 애니가 끝나면 서버에 알려주기위해)
public:
	CPlayerSutda();
	virtual ~CPlayerSutda();	

	void GameKeyProcess(int nVKey, int nExtend);

	void Clear();
	void SetPlayerNum(int pnum);
	void SetNewPlayer(USERINFO *pUI);
	void Reset();
	void OnTimer();
	void InitRoomData();

	// 베팅 관련 
	void OnSetChip(INT64 money);

	// 카드 관련
	void OnCardDraw(NMBASE::GRAPHICGDI::CPage *pPage);	

	void SetResultFace(int index, BOOL bresult = FALSE);

	void SetAddCard(int nCardNum );							// 카드를 하나씩 받으때만다 호출한다.(카드추가)

	int  GetCardNum(int index);								// 카드 번호
	BOOL IsCardFront(int index);							// 카드 앞면인가?
	void SetCardFrontShow(int index);						// 카드 앞면을 보이게 한다.
	void SetCardBackShow(int index);						// 카드 뒷면을 보이게 한다.
	void ShowCard(int index , BOOL bshow = TRUE);			// 카드를 보이도록 한다.
	

	void SetGameResultCardOpen(int delay);					// 카드 오픈 (애니 처리 설정) 결과때
	void SetChioceCardMove(int face = 0);					// 초이스일때 face 0 : 일반 1은 나인데 뒷면 2는 나 인데 앞면 

	BOOL IsGameOverPtInCardRect(int xp,int yp);				// 게임종료시 카드 영역을 클릭 했는지 
	int  EventOverPtInCardRect(CPoint pt);					// 카드 영역에 있는지
	
	
	//바꿀 카드 선택
	BOOL OnGameKey(int nVKey, int nExtend, int repeat=0);
	void OnLButtonDown(int xp, int yp);
	void OnLButtonUp(int xp, int yp);

	void OnMouseMove(int xp, int yp);	

	//이팩트 효과 지우기
	void ClearEffectSpr(ENUM_ANIFILEID kind, BOOL ballclear = FALSE);

	void OnDraw_Bottom(CDC *pDC);		
	void OnDraw_Middle(CDC *pDC);
	void OnDraw_Top(CDC *pDC);
	
	void SetCardResult();				// 카드를 저장하여 룰 계산 한다	

	void Draw_DarkCard();


	
		
	int GetCardTotal(){return m_nCardTotalCount;}
	void SetCardTotal(int nCount)
	{
		m_nCardTotalCount = nCount;

		/*if(m_nCardTotalCount == 3)
		{
			int aaa = 0;
		}

		char szTemp1[256] = {0, };
		sprintf(szTemp1, "------ CPlayerSutda::SetCardTotal => TotalCardCount:%d , OpenCardCont:%d \n", m_nCardTotalCount, m_nOpenCardTotalCount);
		OutputDebugString(szTemp1);*/
	}

	int GetOpenCardCount(){return m_nOpenCardTotalCount;}
	void SetOpenCardCount(int nCount){m_nOpenCardTotalCount = nCount;}

	void SetThrowCardNum(int nCardNum){m_nThrowCardNum = nCardNum;}

	void DrawEft_CompleteMadeValue(int oldpoint=0, bool bForce=false );

	void HiddenOpne();
	int GetHiddenCardMoveWay( int xp, int yp );


//섯다용 추가

protected:
	int	m_nTurnLimitCnt;		//베팅 클릭을 시간제한으로 몇번 햇는지 3번 이상이면 강퇴 한다.
	int m_nJokboPoint;	//유저의 족보 정보
	int m_nJokboAniPoint;	//유저의 족보 애니 정보
	CString m_strJokboName; //족보명
	vector<int> m_vecJokboIndex; //족보 인덱스 모음(2장일경우 2개, 3장일 경우 3개);
	int m_nOrignalCard[CARD_TOTALCOUNT_BUF];

public:
	INT64 m_llPenaltyMoney;	//땡값
	PENALTY_STATE m_ePenaltyState;	//땡값 종류 

public:


	void IncTurnLimitCnt()
	{
		++m_nTurnLimitCnt;
	}

	int GetTurnLimitCnt()
	{
		return m_nTurnLimitCnt;
	}

	void ResetTurnLimitCnt()
	{
		m_nTurnLimitCnt = 0;
	}

	CPoint GetCardPos(int index);
	void  SetCardPos(int index, CPoint ptPos);
	void SwapHiddenCard();//히든카드가오면 바로 앞카드와 위치를 바꾼다(쪼으기를 위해)
	void SetJokboPoint(int nJokboPoint)  { m_nJokboPoint = nJokboPoint; }
	int GetJokboPoint()  { return m_nJokboPoint; }
	void SetJokboAniPoint(int nJokboAniPoint)  { m_nJokboAniPoint = nJokboAniPoint; }
	int GetJokboAniPoint()  { return m_nJokboAniPoint; }

	CString GetJokboName() { return m_strJokboName; }
	vector<int> &GetJokboIndex() { return m_vecJokboIndex; }
	
	int GetHiddenCardIndex();	
	void CalcJokbo();

	void BackupOriginalCard();
	void ResetOrginalCard();	//처음 받았던 카드로 다시 셋팅한다.

	void SetOrginalCard(char nCard[], int nTotCard);
	void SetOrginalCard( int nIndex, int nCard );

	int GetOrginalCard(int nCardIndex);

	void CardResultGather( BOOL bLastPlayer, int nDelay );
	void CardResultOpen( BOOL bLastPlayer );
	void CardResultOpen_FoldOpen();

	void GenealogyEffect();

	void SetCardTornOut(BOOL bFlag);			//카드 찢기 효과를 준다.
	int GetCardMonthValue(int index);			

	void SetCardBlinking(int nTime); //카드 깜빡거리는 효과


	CPoint GetStartCardPt();

	CPoint GetFinalCardPt(int nMaxCard, int nCardIndex);			//카드장수에 따른 카드 위치 

	void CompleteJJogi(BOOL bNotifyServer);
	CPoint GetHiddenCardFinalPos();

	int GetSndFxKind();

	BOOL DoesHaveGoodCard();		//첫번째 라운드에서 좋은 카드가 들어왔는지?(일, 삼, 팔) 

	void SetResultCardOpenAni(BOOL bResultCardOpenAni) { m_bResultCardOpenAni = bResultCardOpenAni; }
	BOOL IsResultCardOpenAni()  { return m_bResultCardOpenAni; }

	//max방에서 보우금액 업데이트를 위한 함수 추가
	INT64 GetNowMoney();
	void SetNowMoney( INT64 nMoney );
};

#endif // !defined(AFX_PlayerSutda_H__83E277C5_E483_47C5_BA5A_E89EE2F1F912__INCLUDED_)