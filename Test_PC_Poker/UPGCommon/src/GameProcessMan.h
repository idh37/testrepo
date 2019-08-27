// GameProcessMan.h: interface for the CGameProcessMan class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_GAMEPROCESSMAN_H__35F9D504_03D5_429E_83D6_DD30E359EC1E__INCLUDED_)
#define AFX_GAMEPROCESSMAN_H__35F9D504_03D5_429E_83D6_DD30E359EC1E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "GameProcess.h"



class UPG_COMMON_EXPORT CGameProcessMan  
{

public:
	CGameProcessMan();
	virtual ~CGameProcessMan();


	
	int					m_CurGameMode;

	CGameProcess	*m_pCurrentProces;	

	CGameProcess	*GetCurGameProcess();
	void	InitGameProcess(CGameProcess *pGameProces);

	CPlayer* GetPlayer(int index );


};

#endif // !defined(AFX_GAMEPROCESSMAN_H__35F9D504_03D5_429E_83D6_DD30E359EC1E__INCLUDED_)
