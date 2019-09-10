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

// ĳ���Ϳ� ǥ���Ǵ� Ÿ��
enum ECHARTYPE
{
	EHOST_TYPE  = 0,	// �Ϲݹ���
	ESHOST_TYPE = 1,	// ���۹���
	EDEALER_TYPE= 2,	// ����(7Poker���� ��� ����)
	EBOSS_TYPE  = 3,	// ����
};


#define CARD_TOTALCOUNT		7	// �� 7���� ������ ó���� ������ ������.(ī������� ��������Ʈ Index��� ���꿡 ����� �Ѵ�.)
#define MAX_CARDCOUNT		8


class CPlayerHoola  : public CPlayer
{
public:	
	enum 
	{
		Banish_Vote=0,
		Banish_Force,
	};

	CPoint				m_ptAvatarStart;				// �ƹ�Ÿ ���� ��ġ
	int					m_nMyIpCheck;					// �ߺ��� IP
	INT64				m_n64ResultMoney;				// ����Ӵ�
	
	CObjectList			m_cPlayerEffectList;			// ��������Ʈ, �̹����� ����ؼ� ó�� �� �����ִ� ����
	int					m_nSDelay;						// ���� ������	

	//�Ƕ󿡼� ����ϴ� �ڵ�
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

	// ���� ���� 
	void OnSetChip(INT64 money);

	// ī�� ����
	void OnCardDraw(NMBASE::GRAPHICGDI::CPage *pPage);	

	void SetResultFace(int index, BOOL bresult = FALSE);

	
	int  GetCardNum(int index);								// ī�� ��ȣ
	BOOL IsCardFront(int index);							// ī�� �ո��ΰ�?
	void SetCardFrontShow(int index);						// ī�� �ո��� ���̰� �Ѵ�.
	void SetCardBackShow(int index);						// ī�� �޸��� ���̰� �Ѵ�.
	void ShowCard(int index , BOOL bshow = TRUE);			// ī�带 ���̵��� �Ѵ�.
	


	//�ٲ� ī�� ����
	BOOL OnGameKey(int nVKey, int nExtend, int repeat=0);
	void OnLButtonDown(int xp, int yp);
	void OnLButtonUp(int xp, int yp);

	void OnMouseMove(int xp, int yp);	

	//����Ʈ ȿ�� �����
	void ClearEffectSpr(ENUM_ANIFILEID kind, BOOL ballclear = FALSE);

	void OnDraw_Bottom(CDC *pDC);		
	void OnDraw_Middle(CDC *pDC);
	void OnDraw_Top(CDC *pDC);
	

	void DrawEft_CompleteMadeValue(int oldpoint=0, bool bForce=false );

	//max�濡�� ����ݾ� ������Ʈ�� ���� �Լ� �߰�
	INT64 GetNowMoney();
	void SetNowMoney( INT64 nMoney );
};

#endif // !defined(AFX_PLAYER7POKER_H__83E277C5_E483_47C5_BA5A_E89EE2F1F912__INCLUDED_)