// NMovingCard.h: interface for the CNMovingCard class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NMOVINGCARD_H__F18CA7B7_BF95_4EDE_BBC7_AAFAAF76325B__INCLUDED_)
#define AFX_NMOVINGCARD_H__F18CA7B7_BF95_4EDE_BBC7_AAFAAF76325B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "NMovement.h"
//#include "MovementEx.h"
//
enum CARD_MOVE_DIRECT
{
	DIRECT_MOVE_UP = 0,
	DIRECT_MOVE_DOWN,
	DIRECT_MOVE_PASS,
};

typedef struct _tagSOUNDDATA
{
	bool bSoundUse;
	int  nCuttingNum;
	
	void Clear()
	{
		bSoundUse = FALSE;
		nCuttingNum = -1;
	}
}SOUNDDATA;

typedef struct _tagNCARDMOVEDATA
{
	int   m_nPlayNum;
	int	  m_nIndex;
	float m_fSpeed;
	int	  m_nDelay;

	int m_nFirstCmd;
	SOUNDDATA m_sFirstSound;
	POINT m_ptStartPos;
	POINT m_ptDestPos;

	int   m_nSecondCmd;
	SOUNDDATA m_sSecondSound;
	POINT m_ptStartPos_2nd;
	POINT m_ptDestPos_2nd;

	int  m_nNewIndex;//< CYS 101201 >

	void Clear()
	{
		m_nPlayNum =-1;
		m_nIndex =-1;
		m_fSpeed =0.0f;
		m_nDelay =0;

		m_nFirstCmd =0;
		m_sFirstSound.Clear();
		m_ptStartPos.x =0;
		m_ptStartPos.y = 0;		
		m_ptDestPos.x = 0;
		m_ptDestPos.y = 0;	

		m_nSecondCmd =0;	
		m_sSecondSound.Clear();
		m_ptStartPos_2nd.x = 0;
		m_ptStartPos_2nd.y = 0;
		m_ptDestPos_2nd.x = 0;
		m_ptDestPos_2nd.y = 0;	
		m_nNewIndex = -1;

	}
}NCARDMOVEDATA;

typedef struct _tagMOVECARD_LIST
{
	CNMovement	  m_cCardMove;
	NCARDMOVEDATA	m_sCardMoveData;

	void Clear()
	{
		m_cCardMove.ReSet();	
		m_sCardMoveData.Clear();
	}
}MOVECARD_LIST;


#define DEALINGCARDGAP_BD	4		// �ٵ��̿� ������ ����ī�� �ִϰ� �� 8���� �ִ�.


typedef void (*PFN_CARDEVENT)( LPVOID pData, LPVOID pValue );

class CNMovingCard  
{
public:
	CNMovingCard();
	virtual ~CNMovingCard();

	int m_nCardCnt;
	int m_nDelay;
	int m_nDelayCount;
	int	m_nDirect;		//�̵� ���� 0 up 1 down;	

	int	nPNum;			//ī�带 ���� ���� �ε���

	int nDebugCnt;

	std::list< MOVECARD_LIST >	m_ltMoveCardList;	// �̵� ī�� ����Ʈ

	NCARDMOVEDATA m_sCardMoveData[ TOTAL_CARD_BD ];
	

	void Initialize( NCARDMOVEDATA *pData, int nCardcnt, int nMainDelayTime, int movedirect = DIRECT_MOVE_DOWN, bool bUseSceneMoving = false, PFN_CARDEVENT pEventCB = NULL );	// ī�� ����
	void Clear();			// ��� Ŭ����
	void ReSetMovement();
	void OnTimer();	

	// ��ǥ ��
	void Draw( NMBASE::GRAPHICGDI::CPage *pPage);
	void SetMoveUp();
	void SetMoveDown();
	void SetMovePass();


	// �ִϻ������
	void SetUseSceneMoving( bool bUseSceneMoving )
	{
		m_bUseSceneMoving =  bUseSceneMoving;
	}

	bool IsDestroyMovingCard()
	{
		return m_bDestroy;
	}

	int GetLastCMD()
	{
		return m_nLastChangerCmd;
	}

	void SetLastCMD( int nCmd )
	{
		m_nLastChangerCmd = nCmd;
	}

	int GetLastCMD_User()
	{
		return m_nLastChangerCmdPNum;
	}

	void SetLastCMD_User( int nPNum )
	{
		m_nLastChangerCmdPNum = nPNum;
	}

	void SetF4CardUpdate( bool bUpdate )
	{
		m_bF4CardUpdate = bUpdate;
	}

	bool IsF4CardUpdate()
	{
		return m_bF4CardUpdate;
	}

protected:
	PFN_CARDEVENT m_pFuncCardEvent;
	bool m_bUseSceneMoving;

	DWORD m_dwStartTime;
	bool  m_bDestroy;

	int m_nLastChangerCmd;	   // ������ ī�忡 �̺�Ʈ ���		
	int m_nLastChangerCmdPNum; // ������ ī�忡 �̺�Ʈ ����� ����

	bool m_bF4CardUpdate;
};


#endif // !defined(AFX_NMOVINGCARD_H__F18CA7B7_BF95_4EDE_BBC7_AAFAAF76325B__INCLUDED_)
