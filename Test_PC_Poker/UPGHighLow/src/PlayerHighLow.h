// PlayerHighLow.h: interface for the PlayerHighLow class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PlayerHighLow_H__83E277C5_E483_47C5_BA5A_E89EE2F1F912__INCLUDED_)
#define AFX_PlayerHighLow_H__83E277C5_E483_47C5_BA5A_E89EE2F1F912__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "UserStruct.h"
#include "MyCard.h"
#include "AnimationDefine.h"
#include "SevenRuleMan.h"

enum EHIGHLOWSWINGCASE;

// 캐릭터에 표현되는 타입
enum ECHARTYPE
{
	EHOST_TYPE  = 0,	// 일반방장
	ESHOST_TYPE = 1,	// 슈퍼방장
	EDEALER_TYPE= 2,	// 딜러(7Poker에서 사용 안함)
	EBOSS_TYPE  = 3,	// 보스
};


#define CARD_TOTALCOUNT		7	// 총 7장을 받지만 처음에 한장을 버린다.(카드딜링시 스프라이트 Index계산 연산에 사용을 한다.)


class CPlayerHighLow  : public CPlayer
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

	CMyCard				m_cMyCard[CARD_TOTALCOUNT];		// 실제 플레이에 사용되는 카드 / ###[8포커]###			
	CRect				m_cCardRectEventEx[CARD_TOTALCOUNT];
	int					m_nTotalCardCount;

private:

	CString				m_stJokboName;					// 족보명

	BOOL				m_bResultCardOpenMove;			// 결과에서 카드 이동 플래그 

	int					m_nCardTotalCount;

	int					m_nOpenCardTotalCount;

	int					m_nThrowCardNum;				// 초이스 모드일때 버린카드 정보

	BOOL				m_bHiddenScene;					
	DWORD				m_dwHiddenSceneDrawTime;

	//하이로우 전용
	EHIGHLOWSWINGCASE	m_eCaseSelect;						//유저들이 선택한 하이 / 로우 / 스윙
	EHIGHLOWSWINGCASE	m_eCaseResult;							// 결과:  1 하이 2 로우 3 스윙 0패배or폴드
public:
	CPlayerHighLow();
	virtual ~CPlayerHighLow();	



	void GameKeyProcess(int nVKey, int nExtend);

	void set_CardArray();								// 카드 배열 정렬 기본형,일자형
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
	
	void OnDrawThrowCard(CDC *pDC);							// 버린카드를 그린다.

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

	
	void DrawDummyHiddenCard(CDC *pDC);
		
	int GetCardTotal(){return m_nCardTotalCount;}
	void SetCardTotal(int nCount)
	{
		m_nCardTotalCount = nCount;

		/*if(m_nCardTotalCount == 3)
		{
			int aaa = 0;
		}

		char szTemp1[256] = {0, };
		sprintf(szTemp1, "------ CPlayerHighLow::SetCardTotal => TotalCardCount:%d , OpenCardCont:%d \n", m_nCardTotalCount, m_nOpenCardTotalCount);
		OutputDebugString(szTemp1);*/
	}

	int GetOpenCardCount(){return m_nOpenCardTotalCount;}
	void SetOpenCardCount(int nCount){m_nOpenCardTotalCount = nCount;}

	void SetThrowCardNum(int nCardNum){m_nThrowCardNum = nCardNum;}

	void DrawEft_CompleteMadeValue(int oldpoint=0, bool bForce=false );

	void HiddenOpne();
	int GetHiddenCardMoveWay( int xp, int yp );

// 	BOOL IsDieFlag(){return bFold;}
// 	void SetDieFlag(BOOL bFlagInput){bFold = bFlagInput;}

	//하이로우 전용
	void SetCaseSelect(EHIGHLOWSWINGCASE eCaseSelect)
	{
		m_eCaseSelect = eCaseSelect;	//유저가 선택한 하이/로우/스윙
	}
	EHIGHLOWSWINGCASE GetCaseSelect()
	{
		return m_eCaseSelect;	//유저가 선택한 하이/로우/스윙
	}

	void SetCaseResult(EHIGHLOWSWINGCASE eCaseResult)
	{
		m_eCaseResult = eCaseResult;	//유저결과 하이/로우/스윙
	}
	EHIGHLOWSWINGCASE GetCaseResult()
	{
		return m_eCaseResult;	//유저가 선택한 하이/로우/스윙
	}

	//max방에서 보우금액 업데이트를 위한 함수 추가
	INT64 GetNowMoney();
	void SetNowMoney( INT64 nMoney );
};

#endif // !defined(AFX_PlayerHighLow_H__83E277C5_E483_47C5_BA5A_E89EE2F1F912__INCLUDED_)