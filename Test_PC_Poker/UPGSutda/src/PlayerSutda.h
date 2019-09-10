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

// ĳ���Ϳ� ǥ���Ǵ� Ÿ��
enum ECHARTYPE
{
	EHOST_TYPE  = 0,	// �Ϲݹ���
	ESHOST_TYPE = 1,	// ���۹���
	EDEALER_TYPE= 2,	// ����(7Poker���� ��� ����)
	EBOSS_TYPE  = 3,	// ����
};


#define CARD_TOTALCOUNT_BUF		(3+4)	//$$$$$// �� 7���� ������ ó���� ������ ������.(ī������� ��������Ʈ Index��� ���꿡 ����� �Ѵ�.)
#define CARD_TOTALCOUNT   (g_RI.FormKind==SD_2CARD?2:3)

#define MAX_MY_SUTDA_CARD			(5)	//�����۾�


class CPlayerSutda  : public CPlayer
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

	CMyCard				m_cMyCard[CARD_TOTALCOUNT_BUF];		// ���� �÷��̿� ���Ǵ� ī�� / ###[8��Ŀ]###			
	CRect				m_cCardRectEventEx[CARD_TOTALCOUNT_BUF];
	int					m_nTotalCardCount;

private:

	CString				m_stJokboName;					// ������

	BOOL				m_bResultCardOpenMove;			// ������� ī�� �̵� �÷��� 

	int					m_nCardTotalCount;

	int					m_nOpenCardTotalCount;

	int					m_nThrowCardNum;				// ���̽� ����϶� ����ī�� ����

	BOOL				m_bHiddenScene;					
	DWORD				m_dwHiddenSceneDrawTime;

	BOOL				m_bResultCardOpenAni;		//����� ī�� ��ġ�� �ִϸ��̼��� �������ΰ�? (��������� ī�� ���� �ִϰ� ������ ������ �˷��ֱ�����)
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


//���ٿ� �߰�

protected:
	int	m_nTurnLimitCnt;		//���� Ŭ���� �ð��������� ��� �޴��� 3�� �̻��̸� ���� �Ѵ�.
	int m_nJokboPoint;	//������ ���� ����
	int m_nJokboAniPoint;	//������ ���� �ִ� ����
	CString m_strJokboName; //������
	vector<int> m_vecJokboIndex; //���� �ε��� ����(2���ϰ�� 2��, 3���� ��� 3��);
	int m_nOrignalCard[CARD_TOTALCOUNT_BUF];

public:
	INT64 m_llPenaltyMoney;	//����
	PENALTY_STATE m_ePenaltyState;	//���� ���� 

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
	void SwapHiddenCard();//����ī�尡���� �ٷ� ��ī��� ��ġ�� �ٲ۴�(�����⸦ ����)
	void SetJokboPoint(int nJokboPoint)  { m_nJokboPoint = nJokboPoint; }
	int GetJokboPoint()  { return m_nJokboPoint; }
	void SetJokboAniPoint(int nJokboAniPoint)  { m_nJokboAniPoint = nJokboAniPoint; }
	int GetJokboAniPoint()  { return m_nJokboAniPoint; }

	CString GetJokboName() { return m_strJokboName; }
	vector<int> &GetJokboIndex() { return m_vecJokboIndex; }
	
	int GetHiddenCardIndex();	
	void CalcJokbo();

	void BackupOriginalCard();
	void ResetOrginalCard();	//ó�� �޾Ҵ� ī��� �ٽ� �����Ѵ�.

	void SetOrginalCard(char nCard[], int nTotCard);
	void SetOrginalCard( int nIndex, int nCard );

	int GetOrginalCard(int nCardIndex);

	void CardResultGather( BOOL bLastPlayer, int nDelay );
	void CardResultOpen( BOOL bLastPlayer );
	void CardResultOpen_FoldOpen();

	void GenealogyEffect();

	void SetCardTornOut(BOOL bFlag);			//ī�� ���� ȿ���� �ش�.
	int GetCardMonthValue(int index);			

	void SetCardBlinking(int nTime); //ī�� �����Ÿ��� ȿ��


	CPoint GetStartCardPt();

	CPoint GetFinalCardPt(int nMaxCard, int nCardIndex);			//ī������� ���� ī�� ��ġ 

	void CompleteJJogi(BOOL bNotifyServer);
	CPoint GetHiddenCardFinalPos();

	int GetSndFxKind();

	BOOL DoesHaveGoodCard();		//ù��° ���忡�� ���� ī�尡 ���Դ���?(��, ��, ��) 

	void SetResultCardOpenAni(BOOL bResultCardOpenAni) { m_bResultCardOpenAni = bResultCardOpenAni; }
	BOOL IsResultCardOpenAni()  { return m_bResultCardOpenAni; }

	//max�濡�� ����ݾ� ������Ʈ�� ���� �Լ� �߰�
	INT64 GetNowMoney();
	void SetNowMoney( INT64 nMoney );
};

#endif // !defined(AFX_PlayerSutda_H__83E277C5_E483_47C5_BA5A_E89EE2F1F912__INCLUDED_)