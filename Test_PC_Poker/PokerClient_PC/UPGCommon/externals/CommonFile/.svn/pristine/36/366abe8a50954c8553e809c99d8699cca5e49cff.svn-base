// BadugiRuleMan.h: interface for the CBadugiRuleMan class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_BADUGIRULEMAN_H__05144EEA_6B8B_471E_BD76_0AA96C86D013__INCLUDED_)
#define AFX_BADUGIRULEMAN_H__05144EEA_6B8B_471E_BD76_0AA96C86D013__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "BadugiRule.h"

enum BDR_GAMEMODE {BDR_NORMAL_MODE = 0, BDR_BOEB_MODE = 1,};

class CBadugiRuleMan  
{	
public:
	CBadugiRuleMan();
	virtual ~CBadugiRuleMan();

	int m_PlayTotalNum;
	int m_nCurTotalUser;		//현재 게임중이 유저
	int m_nGameMode;			// BDR_GAMEMODE

	CBadugiRule m_MycardRule[BD_TOTAL_USER];
	
	void BDR_Clear(int mode =0);	
	void BDR_SetUserCard(int pnum , int card[] , int cardnum ,BOOL bfirst = TRUE);
	void BDR_FoldUser(int pnum);

	void BDR_GetWinner(int playnum ,int nwinner[] , int &nwinnernum , int rank[]);
	int  BDR_GetWinner();
	int  BDR_GetWinnerCnt(int rank);
	
};

#endif // !defined(AFX_BADUGIRULEMAN_H__05144EEA_6B8B_471E_BD76_0AA96C86D013__INCLUDED_)
