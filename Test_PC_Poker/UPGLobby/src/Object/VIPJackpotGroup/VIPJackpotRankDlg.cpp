#include "StdAfx.h"
#include "VIPJackpotRankDlg.h"
#include "../../WidgetID.h"

CVIPJackpotRankDlg::CVIPJackpotRankDlg()
: m_pListWin(NULL)
, m_pListTop(NULL)
, m_bLoadList(false)
{

}

CVIPJackpotRankDlg::~CVIPJackpotRankDlg()
{

}

void CVIPJackpotRankDlg::OnCreateChild( LSY::CObjectBase *pObject )
{
	if(pObject == NULL) 
		return;

	switch(pObject->GetID())
	{
	case ID_LOBBY_LIST_VIPJACKPOT_WIN:
		{
			m_pListWin = (LSY::CList *)pObject;
		}
		break;
	case ID_LOBBY_LIST_VIPJACKPOT_TOP:
		{
			m_pListTop = (LSY::CList *)pObject;
		}
		break;
	}

	__super::OnCreateChild(pObject);
}

void CVIPJackpotRankDlg::SetShow(const bool &bShow)
{
	if (bShow)
		AddListItem();

	CObjectBase::SetShow(bShow);
}

void CVIPJackpotRankDlg::AddListItemWin()
{
	if (NULL == m_pListWin)
		return;

	m_pListWin->DeleteAllItem();
}

void CVIPJackpotRankDlg::AddListItemTop()
{
	if (NULL == m_pListTop)
		return;

	m_pListTop->DeleteAllItem();
}

void CVIPJackpotRankDlg::AddListItem()
{
	if (m_bLoadList)
		return;

	AddListItemWin();
	AddListItemTop();

	m_bLoadList = true;
}
