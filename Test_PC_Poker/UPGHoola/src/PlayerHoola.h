// PLAYER7POKER.h: interface for the PLAYER7POKER class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PLAYER7POKER_H__83E277C5_E483_47C5_BA5A_E89EE2F1F912__INCLUDED_)
#define AFX_PLAYER7POKER_H__83E277C5_E483_47C5_BA5A_E89EE2F1F912__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "UserStruct.h"
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


#define CARD_TOTALCOUNT		7	// 총 7장을 받지만 처음에 한장을 버린다.(카드딜링시 스프라이트 Index계산 연산에 사용을 한다.)
#define MAX_CARDCOUNT		8


class CPlayerHoola  : public CPlayer
{
public:	
	enum 
	{
		Banish_Vote=0,
		Banish_Force,
	};

	CPoint				m_ptAvatarStart;				// 아바타 시작 위치
	int					m_nMyIpCheck;					// 중복된 IP
	INT64				m_n64ResultMoney;				// 결과머니
	
	CObjectList			m_cPlayerEffectList;			// 스프라이트, 이미지를 등록해서 처리 및 보여주는 역할
	int					m_nSDelay;						// 사운드 딜레이	

	//훌라에서 사용하는 코드
	int					m_nTotalCardCount;

	int					m_nPlayerCard[MAX_CARDCOUNT];

public:	
	void				SetCardCount( int nCardCount );
	int					GetCardCount(){ return m_nTotalCardCount; }

	BOOL				m_bRegist;
	DWORD				m_dwRegist;

	void				SetRegist();

	
public:
	CPlayerHoola();
	virtual ~CPlayerHoola();	

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

	
	int  GetCardNum(int index);								// 카드 번호
	BOOL IsCardFront(int index);							// 카드 앞면인가?
	void SetCardFrontShow(int index);						// 카드 앞면을 보이게 한다.
	void SetCardBackShow(int index);						// 카드 뒷면을 보이게 한다.
	void ShowCard(int index , BOOL bshow = TRUE);			// 카드를 보이도록 한다.
	


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
	

	void DrawEft_CompleteMadeValue(int oldpoint=0, bool bForce=false );

	//max방에서 보우금액 업데이트를 위한 함수 추가
	INT64 GetNowMoney();
	void SetNowMoney( INT64 nMoney );
};

#endif // !defined(AFX_PLAYER7POKER_H__83E277C5_E483_47C5_BA5A_E89EE2F1F912__INCLUDED_)