#include "StdAfx.h"
#include "PlayerDrawManager_BD.h"

CPlayerDrawManager_BD::CPlayerDrawManager_BD(int nMaxPlayer)
:	CPlayerDrawManager(nMaxPlayer)
{
	ResetLowBDWinnerPNum();
}

CPlayerDrawManager_BD::~CPlayerDrawManager_BD(void)
{
}

void CPlayerDrawManager_BD::Draw_CardAll( CDC* pDC )
{
	static_cast< CPlayerDraw_BD* >( m_listPlayer[ 2 ] )->DrawCard( pDC );
	static_cast< CPlayerDraw_BD* >( m_listPlayer[ 3 ] )->DrawCard( pDC );
	static_cast< CPlayerDraw_BD* >( m_listPlayer[ 4 ] )->DrawCard( pDC );
	static_cast< CPlayerDraw_BD* >( m_listPlayer[ 1 ] )->DrawCard( pDC );
	static_cast< CPlayerDraw_BD* >( m_listPlayer[ 0 ] )->DrawCard( pDC );

	// 카드 이동
	g_GAME_BD()->GetMovingCardMgr()->Draw( pDC, &m_pGameView->Page );
}


void CPlayerDrawManager_BD::AddLowBDWinnerPNum(int nPNum) 
{
	for(int i=0; i<MAX_PLAYER; i++)
	{
		if (m_nLowBDWinner[i] < 0)
		{
			m_nLowBDWinner[i] = nPNum;
			break;
		}
	}

}
bool CPlayerDrawManager_BD::IsLowBDWinnerPNum(int nPNum)
{
	for(int i=0; i<MAX_PLAYER; i++)
	{
		if (m_nLowBDWinner[i] == nPNum)
			return true;
	}
	return false;
}
void CPlayerDrawManager_BD::ResetLowBDWinnerPNum() 
{
	for(int i=0; i<MAX_PLAYER; i++)
	{
		m_nLowBDWinner[i] = -1;
	}

}


void CPlayerDrawManager_BD::Reset(void)
{
	CPlayerDrawManager::Reset();

	ResetLowBDWinnerPNum();


}

void CPlayerDrawManager_BD::Reset(int nPNum)
{
	CPlayerDrawManager::Reset(nPNum);
}