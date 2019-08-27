#include "StdAfx.h"
#include "PlayerDrawManager_SD.h"
#include "PlayerSutda.h"
#include "GlobalGame.h"
#include "PlayerDraw_SD.h"

CPlayerDrawManager_SD::CPlayerDrawManager_SD( int nMaxPlayer )
:	CPlayerDrawManager(nMaxPlayer)
{

	
}

CPlayerDrawManager_SD::~CPlayerDrawManager_SD(void)
{
}


void CPlayerDrawManager_SD::LoadImage(LPCTSTR lpPathTimer, 
									  NMBASE::GRAPHICGDI::xSprite *pBetCmdSpr, 
									  NMBASE::GRAPHICGDI::xSprite *pWinLoseBackSpr, 
									  LPCTSTR lpAllinPath, 
									  NMBASE::GRAPHICGDI::xSprite *pTurnSpr, 
									  LPCTSTR szUserBGPath,
									  LPCTSTR szLevelPath)
{

	CPlayerDrawManager::LoadImage(lpPathTimer, pBetCmdSpr, pWinLoseBackSpr, lpAllinPath, pTurnSpr, szUserBGPath, szLevelPath);


	AddLoadImageList(&m_JokboTableSpr,".\\sutda\\data\\skin\\JokboTable.spr", true);
}


void CPlayerDrawManager_SD::Reset(void)
{
	CPlayerDrawManager::Reset();


}

void CPlayerDrawManager_SD::Reset(int nPNum)
{
	CPlayerDrawManager::Reset(nPNum); 
}


void CPlayerDrawManager_SD::CheckResultValue(void)
{
	CPlayerDrawManager::CheckResultValue();
}

void CPlayerDrawManager_SD::ShowPenaltyMoney(int nPnum, bool bShow)
{
	ASSERT(nPnum >= 0 && nPnum < m_nMaxPlayer);
	((CPlayerDraw_SD*)m_listPlayer[nPnum])->ShowPenaltyMoney(bShow);
}



void CPlayerDrawManager_SD::PlayerJokboNameUpdate(int nPnum)
{
	// 카드 날아가는것이 끝나면 족보이름을 한번씩 업데이트 해준다.
	if(m_listPlayer.size() > 0)
	{
		m_listPlayer[nPnum]->UpdateJokboName();
	}
}
