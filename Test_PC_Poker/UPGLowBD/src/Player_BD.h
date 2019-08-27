// Player_Base.h: interface for the CPlayer_BD class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_PLAYER_BASE_H__83E277C5_E483_47C5_BA5A_E89EE2F1F912__INCLUDED_)
#define AFX_PLAYER_BASE_H__83E277C5_E483_47C5_BA5A_E89EE2F1F912__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "UserStruct.h"
#include "NMyCard.h"
#include "AnimationDefine.h"	//UPG작업 추가
#include "BadugiRule.h"

#define NCARD_TOTAL 7

// 캐릭터에 표현되는 타입
enum ECHARTYPE
{
	EHOST_TYPE  = 0,	// 일반방장
	ESHOST_TYPE = 1,	// 슈퍼방장
	EDEALER_TYPE= 2,	// 딜러
	EBOSS_TYPE  = 3,	// 보스
};

typedef struct  
{
	// 0 하이(세븐,바둑포함) 1 로우 3 스윙
	int		nValueKind;
	// 0 동일 족보 요구가 아님 1 동일 족보를 요구 
	BOOL	bValueEqual;
	BOOL	bValueEqual_low;
	// HI 에서는 1 하이 2 로우 3 스윙 하이로우에서만 쓰임
	int		nWinCase;

	// TRUE 승 FALSE 패
	int     bWin;

	void Clear(){
		nValueKind = 0;
		// 0 동일 족보 요구가 아님 1 동일 족보를 요구 
		bValueEqual = FALSE;
		// 0 승자 1 패자 
		nWinCase = FALSE;

		bValueEqual_low = FALSE;

		bWin = FALSE;
	}
}VALUEDATA;

typedef struct  
{
	CPoint		PT_Avatar;				//아바타 
	CPoint		PT_Card;				//카드 시작 위치

} PLAYERPOISTIONDATA;


typedef struct  
{
	int		m_arrCutCount[ 3 ];
	bool	m_arrbShow[ 3 ];
	
	void Clear()
	{
		for( int i = 0 ; i < 3 ; i ++ ) 
		{
			m_arrCutCount[ i ] = -1;
			m_arrbShow[ i ] = false;
		}		
	}	

} CUTCOUNT_INFO;



class CPlayer_BD  : public CPlayer //UGPCommon 작업 추가
{
public:
	CPlayer_BD();
	virtual ~CPlayer_BD();

	PLAYERPOISTIONDATA	m_sUIPos;//좌표 정보
	
	// 게임 정보
	int					m_nCardTotal;
	int					m_nOpenTotal;
	INT64				m_llResultMoney;

	/////////////////카드 관련
	CNMyCard			m_cMyCard[ NCARD_TOTAL ];		   // 카드저장 클래스 // ###[8포커]###			
	int					m_nTotalCardNum;
	bool				m_bCardOPenMove;			// 결과에서 카드 이동 플래그 
	int					m_nSDelay;				// 사운드 딜레이	
	bool				m_bShowValue;			//족보를 보여줌
	BOOL				m_bSelectCard;			//z카드 선택 하였는지 (처음 3 or 4 장 받고)
	bool				m_bDontSelectCard;		//더이상 카드 선택을 못한다.

	CMyBetChip			m_cMyChip;				// 내 배팅칩 던지는 애니
		
	bool	IsValid();

	void OnChangeSelect(CPoint pt = NULL, int index = -1);		//바꿀때 마우스 이벤트
	void ReSetCardPostion();
	void ReSetCardPostionOnly();
	//컷팅할 카드를 몇장 선택 했는지. 이걸로 바꾸기 버튼,패스 버튼 활성화 기준을 삼는다.
	int IsCutCardSelectedNum();

	void DrawEft_CompleteMadeValue(int oldpoint = 0, bool bForce = false );
	void FoldUser_CardClose();			//다이 유저 카드 닫음 (본인 제외)
	void FoldUser_CardOpen();			

	void	set_CardArray();					//카드 배열 정렬 기본형,일자형


	virtual void Destroy_Text();
	virtual void Clear();
	virtual	void SetPlayerNum(int pnum);
	virtual void SetNewPlayer(USERINFO *pUI);
	virtual	void Reset();
	virtual void OnTimer();
	virtual void InitRoomData();
	// 베팅 관련 
	virtual void OnSetChip(INT64 money);
	virtual void OnSetBetDrawMoney(INT64 money,BOOL bPlus = FALSE,int betflg = -1);

	
	// 카드 관련
	virtual void SetResultFace(int index, bool bresult = FALSE);
	
	virtual int  GetCardNo(int index);
	virtual void SetCardNo(int index, int nCardNo );
	virtual bool GetFace(int index);
	virtual void SetFaceUp(int index);
	virtual void SetFaceDown(int index);
	virtual void ShowCard(int index , bool bshow = true);

	virtual void SetGameResultCardOpen(int delay);	// 카드 오픈 (애니 처리 설정) 결과때
	virtual void SetChioceCardMove(BOOL bdirect , int face = 0); // 초이스일때 face 0 : 일반 1은 나인데 뒷면 2는 나 인데 앞면 

	virtual bool MouseOver_PtInCardRect(int xp,int yp);// 카드 영역을 클릭 했는지 
	virtual int  Select_PtInCardRect(CPoint pt);// 카드 영역에 있는지

	virtual void OnLButtonDown(int xp, int yp);
	virtual void OnLButtonUp(int xp, int yp);
	virtual bool OnMouseMove(int xp, int yp);	

	//이팩트 효과 지우기
	virtual void ClearObjectEffect( OBJECT_TYPE kind, bool ballclear = false );
	

//////////////////////////////////////////////////////////////////////////
//  꼭 함수 호출
	virtual void SetNewCard(int cardNum );
	virtual void SetCardResult();	//카드를 저장하여 룰 계산 한다	
	
	virtual const char*   GetValueString(VALUEDATA *data = NULL);
	virtual void    Draw_DarkCard();

	CBadugiRule* GetMyLowBDRule()
	{
		TPASSERT( NULL != m_pMyLowBDRule );
		return m_pMyLowBDRule;
	}

	inline bool IsVaild_CardIndex( int nIndex )
	{
		if ( nIndex < 0 || nIndex >= m_nTotalCardNum )
		{
			return false;
		}

		return true;
	}
	
	CObjectList* GetEffectObjectList() const
	{
		TPASSERT( m_pEffectObjectList != NULL );
		return m_pEffectObjectList;
	}

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

	CUTCOUNT_INFO& GetCutInfo()
	{
		return m_stCutInfo;
	}

	//max방에서 보우금액 업데이트를 위한 함수 추가
	INT64 GetNowMoney();
	void SetNowMoney( INT64 nMoney );

protected:
	bool				m_bFirst;
	std::string			m_strValueName;			//족보

	CBadugiRule*		m_pMyLowBDRule;			//새로운 바둑이룰이당
	CObjectList*		m_pEffectObjectList;	//유저별 이펙트관리
	int					m_nTurnLimitCnt;		//베팅 클릭을 시간제한으로 몇번 햇는지 3번 이상이면 강퇴 한다.
	CUTCOUNT_INFO		m_stCutInfo;			//커팅 횟수 저장
};

#endif // !defined(AFX_PLAYER_BASE_H__83E277C5_E483_47C5_BA5A_E89EE2F1F912__INCLUDED_)


