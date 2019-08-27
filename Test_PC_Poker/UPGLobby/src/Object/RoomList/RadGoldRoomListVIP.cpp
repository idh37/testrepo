#include "StdAfx.h"
#include "RadGoldRoomListVIP.h"

CRadGoldRoomListVIP::CRadGoldRoomListVIP(void)
{
	m_pLobbyView = NULL;
}

CRadGoldRoomListVIP::~CRadGoldRoomListVIP(void)
{
}

void CRadGoldRoomListVIP::OnCreateChild(LSY::CObjectBase *pObject)
{
	if(pObject == NULL) return;
	
	__super::OnCreateChild(pObject);
}

void CRadGoldRoomListVIP::OnCreated(void)
{
	LSY::CObjectProject *pProject = GetObjectMan()->GetProjectFromID(ID_LOBBY);
	
	__super::OnCreated();
}

void CRadGoldRoomListVIP::OnMouseOver(void)
{
	if (m_pLobbyView == NULL)
		m_pLobbyView = (CLobbyView *)GetLobbyWindow();

	if (GM().GetPlaying() == true)
		m_pLobbyView->m_pImgFundTooltipVIP->SetShow(true);
	
	__super::OnMouseOver();
}

void CRadGoldRoomListVIP::OnMouseLeave(void)
{
	if (m_pLobbyView == NULL)
		m_pLobbyView = (CLobbyView *)GetLobbyWindow();

	if (GM().GetPlaying() == true)
		m_pLobbyView->m_pImgFundTooltipVIP->SetShow(false);
	
	__super::OnMouseLeave();
}

bool CRadGoldRoomListVIP::OnMouseMove(const CPoint &ptPos)
{
	return TRUE;
}