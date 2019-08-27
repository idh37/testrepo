#include "StdAfx.h"
#include "PlayerDrawManager_HL.h"
#include "PlayerHighLow.h"
#include "GlobalGame.h"
#include "PlayerDraw_HL.h"

CPlayerDrawManager_HL::CPlayerDrawManager_HL( int nMaxPlayer )
:	CPlayerDrawManager(nMaxPlayer)
{
	m_nLowWinnerPNum = -1;
	m_nSwingWinnerPNum = -1;
}

CPlayerDrawManager_HL::~CPlayerDrawManager_HL(void)
{
}


void CPlayerDrawManager_HL::Reset(void)
{
	CPlayerDrawManager::Reset();

	m_nLowWinnerPNum = -1;
	m_nSwingWinnerPNum = -1;

}

void CPlayerDrawManager_HL::Reset(int nPNum)
{
	CPlayerDrawManager::Reset(nPNum); 
}


void CPlayerDrawManager_HL::CheckResultValue(void)
{
	CString strJokbo="";
	INT64 i64ResultMoney = 0;

	for(int i=0;i<m_nMaxPlayer;i++)
	{
		int nPNum = GM().GetCurrentProcess()->GetPNum_ByServPN(i);
		CPlayerHighLow *pHLPlayer = (CPlayerHighLow*)GM().GetCurrentProcess()->GetPlayer(nPNum);

		if(pHLPlayer && pHLPlayer->JoinState == 1 && pHLPlayer->UI.ID[0] != 0 )
		{
			if((!pHLPlayer->bFold && m_bOpen) || pHLPlayer->bFoldOpen)
			{
				strJokbo.Empty();

				EHIGHLOWSWINGCASE eCaseSelect = pHLPlayer->GetCaseSelect();
				
				switch(eCaseSelect)
				{				
				case CASE_HIGH:
					{
						MM().Call(UPG_CMK_GET_JOKBO_NAME, (WPARAM)nPNum, (LPARAM)&strJokbo);

					} break;
				case CASE_LOW:
					{
						MM().Call(UPG_CMK_GET_LOW_JOKBO_NAME, (WPARAM)nPNum, (LPARAM)&strJokbo);

					} break;

				case CASE_NULL:
				case CASE_SWING:
					{
						CString strHighJokbo, strLowJokbo;
						MM().Call(UPG_CMK_GET_JOKBO_NAME, (WPARAM)nPNum, (LPARAM)&strHighJokbo);
						MM().Call(UPG_CMK_GET_LOW_JOKBO_NAME, (WPARAM)nPNum, (LPARAM)&strLowJokbo);
						strJokbo.Format("%s / %s", strHighJokbo, strLowJokbo);

					} break;
				
				}				
				
				m_listResultJokbo[i] = strJokbo;
			}

			i64ResultMoney = 0;
			MM().Call(UPG_CMK_GET_RESULT_MONEY, (WPARAM)nPNum, (LPARAM)&i64ResultMoney);
			m_listResultMoney[i] = i64ResultMoney;
		}
	}
}

void CPlayerDrawManager_HL::SetSelectedHLS(int nPNum, bool bSelectHLS)
{
	ASSERT(nPNum >= 0 && nPNum < m_nMaxPlayer);
	if (nPNum<0 || nPNum >= m_nMaxPlayer)
		return;

	CPlayerDraw_HL *pPlayerDraw_HL = (CPlayerDraw_HL *) m_listPlayer[nPNum];
	pPlayerDraw_HL->SetSelHLS(bSelectHLS);


}