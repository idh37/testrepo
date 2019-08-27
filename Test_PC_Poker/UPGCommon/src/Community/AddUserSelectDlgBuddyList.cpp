#include "stdafx.h"
#include "AddUserSelectDlg.h"
#include "AddUserSelectDlgBuddyList.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CAddUserSelectDlgBuddyList::CAddUserSelectDlgBuddyList(CAddUserSelectDlg *pDlg)
:	LSY::CList(),
	m_pDlg(pDlg)
{
	m_mapBuddyCheck.clear();
}

CAddUserSelectDlgBuddyList::~CAddUserSelectDlgBuddyList()
{
}

int CAddUserSelectDlgBuddyList::AddItem(LPARAM lParam)
{
	int nRet = __super::AddItem(lParam);

	if(m_pDlg->IsCheckBuddyAllSelectItem())
		m_pDlg->SetCheckBuddyAllSelectItem(false);

	return nRet;
}

bool CAddUserSelectDlgBuddyList::DeleteItem(LPARAM lParam)
{
	bool bRet = __super::DeleteItem(lParam);
	CheckAllItemCheck();
	return bRet;
}

void CAddUserSelectDlgBuddyList::DeleteAllItem(void)
{
	__super::DeleteAllItem();
	m_pDlg->SetCheckBuddyAllSelectItem(false);
}

bool CAddUserSelectDlgBuddyList::CheckAllItemCheck(void)
{
	bool bRet = true;

	LSY::CListItemData *pItemData = GetFirstItem();
	while(pItemData)
	{
		if(!pItemData->GetUserData(_DEF_USER_DATA_BUDDYITEM_TYPE_CHECK))
		{
			bRet = false;
			break;
		}
		pItemData = GetNextItem();
	}

	if(GetItemCount() == 0) bRet = false;

	if(bRet)
	{
		if(!m_pDlg->IsCheckBuddyAllSelectItem())
			m_pDlg->SetCheckBuddyAllSelectItem(true);
	}
	else
	{
		if(m_pDlg->IsCheckBuddyAllSelectItem())
			m_pDlg->SetCheckBuddyAllSelectItem(false);
	}

	return bRet;
}

void CAddUserSelectDlgBuddyList::ClearAllItemCheck(void)
{
	m_pDlg->ClearAllBuddyCheck() ;
}