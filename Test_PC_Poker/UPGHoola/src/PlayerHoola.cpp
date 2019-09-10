// PlayerHoola.cpp: implementation of the PLAYER7POKER class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "PlayerHoola.h"
#include "GlobalGame.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////


CPlayerHoola::CPlayerHoola()
{	
	m_nTotalCardCount = 0;

	ServPNum = 0;

	PNum = -1;

	Clear();
}

CPlayerHoola::~CPlayerHoola()
{
}

void CPlayerHoola::Clear()
{
	ZeroMemory(&UI, sizeof(UI));
	ZeroMemory(&m_cRoomChangeInfo, sizeof(CHANGE_USERINFO_ROOM));
	Reset();
	bFold = FALSE;

	MyIpCheck = -1;
	nSndFxKind = 0;
	nRoundPrevMoney = 0;	
}

void CPlayerHoola::SetPlayerNum(int pnum)
{
	PNum = pnum;
}

void CPlayerHoola::SetNewPlayer(USERINFO *pUI)
{	
	Clear();
	CPlayer::SetNewPlayer(pUI);
	InitRoomData();
	Avatar.SetAvatar(UI.AvatarInfo);
}

void CPlayerHoola::Reset()
{
	JoinState = EGAME_JOINOUT;
	PlayState = EPLAYER_GAMEEND;

	bFold = FALSE;

	m_n64ResultMoney = 0;
	nRoundPrevMoney  = 0;


	m_nSDelay = -1;	
	m_nTotalCardCount = 0;


	//�̹��� ��ο� ���� ����Ʈ 
	m_cPlayerEffectList.Destroy();	

	m_bRegist = FALSE;
	m_dwRegist = 0;	
}


//�濡 �����Ҷ� �ѹ� �ʱ�ȭ �ϴ°͵� 
void CPlayerHoola::InitRoomData()
{
	Reset();

	ENUM_POSTYPE nPosType = EPT_UNKNOWN;
	switch(PNum)
	{
	case 0:	// �ڽ�
		{			
			nPosType = EPT_ME;
		} break;
	case 1:	//����
	case 2:
		{
			nPosType = EPT_LEFT;		
		} break;
	case 3:	// ����
	case 4:
		{
			nPosType = EPT_RIGHT;
		} break;
	default:
		return;
	}

	CPoint pt;
	pt.x = 12;
	pt.y = 0;

	m_ptAvatarStart = g_cUIData.m_listAvatarPos[PNum];
}

void CPlayerHoola::OnTimer()
{
	m_cPlayerEffectList.ProcessAll();	
}

void CPlayerHoola::OnSetChip(INT64 money)
{
	if(PlayState != EPLAYER_GAMEPLAYING && JoinState != EGAME_JOININ && bFold)
	{
		return;		
	}

}

// ī��� �������� �׸��°ͺ��� �Ʒ� �ִ�
void CPlayerHoola::OnDraw_Bottom(CDC *pDC)
{

	m_cPlayerEffectList.BottomDrawAll(pDC);	
}

void CPlayerHoola::OnDraw_Middle(CDC *pDC)
{
	m_cPlayerEffectList.MiddleDrawAll(pDC);
}

//�� m_cPlayer �� ������ �� �̹����� 
void CPlayerHoola::OnDraw_Top(CDC *pDC)
{	
	if(GetCurrentWhere() != IDX_GAMEWHERE_GAME) return;
	m_cPlayerEffectList.TopDrawAll(pDC);	

	//ȭ�鿡 ǥ�õǴ� [���]��������Ʈ�� �׸���
	//�ڽ��� ��� ������ ������ ǥ�õȴ�.

	//����� ǥ�õǴ� ��ǥ���� - ���� UIInfoData�� �ű�ų� Define �ؼ� ����ϵ��� ����.
	int xy2[MAX_PLAYER][2] = { 322, 549, 92, 340, 92, 150, 856, 150, 856, 340 };

	if( strlen(UI.ID) == 0 )
		return;

	if ( JoinState == EGAME_JOININ && PlayState == EPLAYER_GAMEPLAYING  /*|| m_bGameOver*/)
	{
		if (m_bRegist)
		{
			if ( PNum == 0 )
				g_pGameView->Page.DrawMultiScene(xy2[PNum][0], xy2[PNum][1], &g_sprRegOk, 2, timeGetTime() - m_dwRegist );
			else
				g_pGameView->Page.DrawMultiScene(xy2[PNum][0], xy2[PNum][1], &g_sprRegOk, 0, timeGetTime() - m_dwRegist );
		}
	}
	
}

void CPlayerHoola::SetRegist()
{
	if ( FALSE == m_bRegist )
	{
		m_dwRegist = timeGetTime();
	}
	m_bRegist = TRUE;
}

/////////////////ī�� ����///////////////////////////////

// ���̾� ������ ī��� ���η� ���� �׸���.
void CPlayerHoola::OnCardDraw(NMBASE::GRAPHICGDI::CPage *pPage) 
{
	if(GetCurrentWhere() != IDX_GAMEWHERE_GAME)
	{
		return;
	}

	if(pPage ==NULL)
	{
		return;
	}

}

BOOL CPlayerHoola::IsCardFront(int index)
{
	return FALSE;
}

// ī���� �ո��� ���̰� �Ѵ�.
void CPlayerHoola::SetCardFrontShow(int index)
{
	if(index < 0 || index >= m_nTotalCardCount)
		return;
	return;
}

// ī���� �޸��� ���̰� �Ѵ�. 
void CPlayerHoola::SetCardBackShow(int index)
{
	if(index < 0 || index >= m_nTotalCardCount) 
		return;	
}

void CPlayerHoola::ShowCard(int index , BOOL bshow)
{
	if(index < 0 || index >= m_nTotalCardCount) 
		return;	
}

void CPlayerHoola::SetResultFace(int index, BOOL bresult)
{
	if(index < 0 || index >= m_nTotalCardCount) 
		return;
}

// 0 ~ 6 �� 7��
int CPlayerHoola::GetCardNum(int index)
{
	if(index < 0 || index >= m_nTotalCardCount) 
		return 0;
	return m_nTotalCardCount;
}





// ����Ű ó���� �Ѵ�.
void CPlayerHoola::GameKeyProcess(int nVKey, int nExtend)
{
	if( g_RI.State == EGAME_PLAYING &&  g_Poker.CurPlayer == ServPNum && JoinState == EGAME_JOININ && PlayState == EPLAYER_GAMEPLAYING && PNum == 0 && !g_pGameView->m_cPlayerDrawManager.IsFlyWindCard())
	{
		
	}	
}


void CPlayerHoola::OnLButtonDown(int xp, int yp)
{

}

void CPlayerHoola::OnLButtonUp(int xp, int yp)
{

}

void CPlayerHoola::OnMouseMove(int xp, int yp)
{
	
}


//����Ʈ ȿ�� �����
void CPlayerHoola::ClearEffectSpr(ENUM_ANIFILEID kind, BOOL ballclear)
{
	if( ballclear == TRUE )
	{
		m_cPlayerEffectList.Destroy();
	}
	else
	{
		m_cPlayerEffectList.DelObject(kind);
	}
}

BOOL CPlayerHoola::OnGameKey(int nVKey, int nExtend, int repeat)
{	
	if(GetCurrentWhere() != IDX_GAMEWHERE_GAME)
	{
		return FALSE;
	}

	if( g_GAMEPROCESS() == NULL) 
	{
		return FALSE;
	}
	
	if( PNum != 0 || g_ObserverMan.IsMyObserver() || JoinState == EGAME_JOINOUT || bFold == TRUE ) 
	{
		return FALSE;	
	}

	// ���ӿ��� ����ϴ� ����Ű ó��
	GameKeyProcess(nVKey, nExtend);

	
	return TRUE;	
}

void CPlayerHoola::DrawEft_CompleteMadeValue(int oldpoint, bool bForce )
{
	if ( g_ObserverMan.IsMyObserver() || PNum != 0  || bFold == TRUE || PlayState != 1 )
	{
		return;
	}
}

void CPlayerHoola::SetCardCount( int nCardCount )
{
// #ifdef _DEBUG
// 	if( ( g_RI.State == EGAME_PLAYING ) && ( nCardCount == 0 ) && ( PNum != 0 ) )
// 	{
// 		_asm int 3;
// 	}
// #endif
	m_nTotalCardCount = nCardCount;
}

//max�濡�� ����ݾ� ������Ʈ�� ���� �Լ� �߰�
INT64 CPlayerHoola::GetNowMoney()
{
	if( g_RI.bIsMaxRoom )
	{
		return UI.GetInMoney();
	}

	return UI.GetMoney();
}

void CPlayerHoola::SetNowMoney( INT64 nMoney )
{
	if( g_RI.bIsMaxRoom )
	{
		UI.SetInMoney( nMoney );
	}
	else
	{
		UI.SetMoney( nMoney );
	}
}