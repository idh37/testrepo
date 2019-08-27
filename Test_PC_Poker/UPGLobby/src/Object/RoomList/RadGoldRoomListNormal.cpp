#include "StdAfx.h"
#include "RadGoldRoomListNormal.h"

CRadGoldRoomListNormal::CRadGoldRoomListNormal(void)
{
	m_pLobbyView = NULL;
}

CRadGoldRoomListNormal::~CRadGoldRoomListNormal(void)
{
}

void CRadGoldRoomListNormal::OnCreateChild(LSY::CObjectBase *pObject)
{
	if(pObject == NULL) return;
	
	__super::OnCreateChild(pObject);
}

void CRadGoldRoomListNormal::OnCreated(void)
{
	LSY::CObjectProject *pProject = GetObjectMan()->GetProjectFromID(ID_LOBBY);
	
	__super::OnCreated();
}

void CRadGoldRoomListNormal::OnMouseOver(void)
{
	if (m_pLobbyView == NULL)
		m_pLobbyView = (CLobbyView *)GetLobbyWindow();

	if (GM().GetPlaying() == true)
		m_pLobbyView->m_pImgFundTooltipNormal->SetShow(true);
	
	__super::OnMouseOver();
}

void CRadGoldRoomListNormal::OnMouseLeave(void)
{
	if (m_pLobbyView == NULL)
		m_pLobbyView = (CLobbyView *)GetLobbyWindow();

	if (GM().GetPlaying() == true)
		m_pLobbyView->m_pImgFundTooltipNormal->SetShow(false);
	
	__super::OnMouseLeave();
}

bool CRadGoldRoomListNormal::OnMouseMove(const CPoint &ptPos)
{
	return TRUE;
}