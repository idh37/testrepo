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

#define LR		((DWORD)1)		// 0000000000000001(b) (�¿����)
#define UD		((DWORD)2)		// 0000000000000010(b) (���Ϲ���)
#define HB		((DWORD)4)		// 0000000000000100(b) (������ȿ��)

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
}VSRESULTINFO;	//vsmode�϶� ���� ���� �濡�� ���� �÷��� �ϴ� ����� ���� ������


class CPlayer_NP  : public CPlayer
{
public:
	int				m_nCardTotal;
	int				m_nOpenTotal;

	INT64			m_i64MyJackpatMoney;	// ����� �ʾ����� ���߿� ���� ����� �߰��� �־� �ش�.	

	int				m_nFoldRound;
	BOOL			m_bHiddenOpen;
	
	int				m_nThrowAwayCardNo;		//����ī��

	CMyBetChip		m_cMyChip;

	/////////////////ī�� ����
	CMyCard			m_listCard[8];		   // ī������ Ŭ���� // ###[8��Ŀ]###			
	CRect			m_listCardRectEventEx[8];	
	
	BOOL			m_bCardOPenMove;		// ������� ī�� �̵� �÷��� 
	BOOL			m_bChoiceCardMove;		// ������� ī�� �̵� �÷��� 

	int				m_nSDelay;				// ���� ������	
	int				m_nTotalCardNum;

	BOOL			m_bLastHalf;			// �������� ������ �Ƴ�
	CPoint			m_ptPos;

	INT64			m_i64ResultMoney;

	//2006.3.20
	int					m_nCardChoiceLimtCnt;	//ī�� ������ �ڵ����� ��� �޴��� 3�� �̻��̸� ���� �Ѵ�.

	// �����̳� ���ӳ� ���� �̺�Ʈ�� ���Ĩ�� �޴� ��� m_GameOverResultData �� ���� ���� �� ����Ÿ�� ���ư��� ����.
	int					m_nFlagMoneyChipChanged;

	//����ī�����
	BOOL					m_bHiddenScene;					
	DWORD					m_dwHiddenSceneDrawTime;

	CPlayer_NP();
	virtual ~CPlayer_NP();

	virtual void Clear();
	virtual void SetNewPlayer(USERINFO *pUI);
	virtual void Reset();

	void OnTimer();
	void InitRoomData();
	// ���� ���� 
public:	
	void OnSetChip(INT64 money);
	void OnReSetBetDraw();
//	void OnDraw(CDC *pDC = NULL);

	// ī�� ����
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

	void SetGameResultCardOpen(int delay);	// ī�� ���� (�ִ� ó�� ����) �����
	void SetChioceCardMove(BOOL bdirect , int face = 0); // ���̽��϶� face 0 : �Ϲ� 1�� ���ε� �޸� 2�� �� �ε� �ո� 

	BOOL OverPtInCardRect(int xp,int yp);// ī�� ������ Ŭ�� �ߴ��� 
	int  EventOverPtInCardRect(CPoint pt);// ī�� ������ �ִ���
	
	void SetFlag_Money_Chip( int nFlag );
	BOOL IsFlag_Money_Chip( int nFlag );

	void DrawEft_CompleteMadeValue(int oldpoint=0, bool bForce=false );

	//����ī�����
	void HiddenOpne();
	int GetHiddenCardMoveWay( int xp, int yp );

	//max�濡�� ����ݾ� ������Ʈ�� ���� �Լ� �߰�
	INT64 GetNowMoney();
	void SetNowMoney( INT64 nMoney );
};

#endif // !defined(AFX_PLAYER_H__A4DF4943_11B0_11D4_97A2_0050BF0FBE67__INCLUDED_)
