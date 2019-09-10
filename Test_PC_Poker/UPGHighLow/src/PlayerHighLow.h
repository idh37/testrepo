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

// ĳ���Ϳ� ǥ���Ǵ� Ÿ��
enum ECHARTYPE
{
	EHOST_TYPE  = 0,	// �Ϲݹ���
	ESHOST_TYPE = 1,	// ���۹���
	EDEALER_TYPE= 2,	// ����(7Poker���� ��� ����)
	EBOSS_TYPE  = 3,	// ����
};


#define CARD_TOTALCOUNT		7	// �� 7���� ������ ó���� ������ ������.(ī������� ��������Ʈ Index��� ���꿡 ����� �Ѵ�.)


class CPlayerHighLow  : public CPlayer
{
public:

	enum 
	{
		Banish_Vote=0,
		Banish_Force,
	};

	CPoint				m_ptAvatarStart;				// �ƹ�Ÿ ���� ��ġ
	CPoint				m_ptCardStart;					// ī�� ���� ��ġ

	int					m_nMyIpCheck;					// �ߺ��� IP
	
	INT64				m_n64ResultMoney;				// ����Ӵ�
	
	int					m_nSDelay;						// ���� ������	

	BOOL				m_bLastHalf;					// �������� ������ �Ƴ�

	int					m_nCardChoiceLimtCnt;			// ī�� ������ �ڵ����� ��� �޴��� 3�� �̻��̸� ���� �Ѵ�.

	CMyBetChip			m_cMyChip;						// �� ����Ĩ ������ �ִ�

	CObjectList			m_cPlayerEffectList;			// ��������Ʈ, �̹����� ����ؼ� ó�� �� �����ִ� ����

	CMyCard				m_cMyCard[CARD_TOTALCOUNT];		// ���� �÷��̿� ���Ǵ� ī�� / ###[8��Ŀ]###			
	CRect				m_cCardRectEventEx[CARD_TOTALCOUNT];
	int					m_nTotalCardCount;

private:

	CString				m_stJokboName;					// ������

	BOOL				m_bResultCardOpenMove;			// ������� ī�� �̵� �÷��� 

	int					m_nCardTotalCount;

	int					m_nOpenCardTotalCount;

	int					m_nThrowCardNum;				// ���̽� ����϶� ����ī�� ����

	BOOL				m_bHiddenScene;					
	DWORD				m_dwHiddenSceneDrawTime;

	//���̷ο� ����
	EHIGHLOWSWINGCASE	m_eCaseSelect;						//�������� ������ ���� / �ο� / ����
	EHIGHLOWSWINGCASE	m_eCaseResult;							// ���:  1 ���� 2 �ο� 3 ���� 0�й�or����
public:
	CPlayerHighLow();
	virtual ~CPlayerHighLow();	



	void GameKeyProcess(int nVKey, int nExtend);

	void set_CardArray();								// ī�� �迭 ���� �⺻��,������
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

	void SetAddCard(int nCardNum );							// ī�带 �ϳ��� ���������� ȣ���Ѵ�.(ī���߰�)

	int  GetCardNum(int index);								// ī�� ��ȣ
	BOOL IsCardFront(int index);							// ī�� �ո��ΰ�?
	void SetCardFrontShow(int index);						// ī�� �ո��� ���̰� �Ѵ�.
	void SetCardBackShow(int index);						// ī�� �޸��� ���̰� �Ѵ�.
	void ShowCard(int index , BOOL bshow = TRUE);			// ī�带 ���̵��� �Ѵ�.

	void SetGameResultCardOpen(int delay);					// ī�� ���� (�ִ� ó�� ����) �����
	void SetChioceCardMove(int face = 0);					// ���̽��϶� face 0 : �Ϲ� 1�� ���ε� �޸� 2�� �� �ε� �ո� 

	BOOL IsGameOverPtInCardRect(int xp,int yp);				// ��������� ī�� ������ Ŭ�� �ߴ��� 
	int  EventOverPtInCardRect(CPoint pt);					// ī�� ������ �ִ���
	
	void OnDrawThrowCard(CDC *pDC);							// ����ī�带 �׸���.

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
	
	void SetCardResult();				// ī�带 �����Ͽ� �� ��� �Ѵ�	

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

	//���̷ο� ����
	void SetCaseSelect(EHIGHLOWSWINGCASE eCaseSelect)
	{
		m_eCaseSelect = eCaseSelect;	//������ ������ ����/�ο�/����
	}
	EHIGHLOWSWINGCASE GetCaseSelect()
	{
		return m_eCaseSelect;	//������ ������ ����/�ο�/����
	}

	void SetCaseResult(EHIGHLOWSWINGCASE eCaseResult)
	{
		m_eCaseResult = eCaseResult;	//������� ����/�ο�/����
	}
	EHIGHLOWSWINGCASE GetCaseResult()
	{
		return m_eCaseResult;	//������ ������ ����/�ο�/����
	}

	//max�濡�� ����ݾ� ������Ʈ�� ���� �Լ� �߰�
	INT64 GetNowMoney();
	void SetNowMoney( INT64 nMoney );
};

#endif // !defined(AFX_PlayerHighLow_H__83E277C5_E483_47C5_BA5A_E89EE2F1F912__INCLUDED_)