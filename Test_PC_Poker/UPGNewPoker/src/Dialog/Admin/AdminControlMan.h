// AdminControlMan.h: interface for the CAdminControlMan class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_ADMINCONTROLMAN_H__08AA53F0_78B6_4314_95FD_64D2BA16F146__INCLUDED_)
#define AFX_ADMINCONTROLMAN_H__08AA53F0_78B6_4314_95FD_64D2BA16F146__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "../../Player/Player_NP.h"

class CAdmin_UserInfo
{
public:
	CAdmin_UserInfo();
	virtual ~CAdmin_UserInfo();

	BOOL			m_bValid;
	BOOL			m_bShowPlayerInof;
	CPlayer_NP		*m_pPlayInfo;
	CRect			m_rtPlayerArea;
	POINT			m_ptMouse;

	void Init(CPlayer_NP *pPlayer);
	void OnDraw(CDC *pDC);	
	void OnMouseMove(int x, int y);
};

class CAdminControlMan  
{
public:
	CAdminControlMan();
	virtual ~CAdminControlMan();

	BOOL			m_bActive;
	CAdmin_UserInfo *m_pPlayerInfo;

	void	SetAdministrator();

	BOOL	IsAdministartor(){
		return m_bActive >= 1 ? TRUE : FALSE;
	}

	void OnMouseMove(int x, int y);
	void OnDraw(CDC *pDC);
};





#endif // !defined(AFX_ADMINCONTROLMAN_H__08AA53F0_78B6_4314_95FD_64D2BA16F146__INCLUDED_)
