#include "stdafx.h"
#include "AddUserSelectDlg.h"
#include "AddUserSelectDlgConnectUserList.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CAddUserSelectDlgConnectUserList::CAddUserSelectDlgConnectUserList(CAddUserSelectDlg *pDlg)
:	LSY::CList(),
	m_pDlg(pDlg)
{
}

CAddUserSelectDlgConnectUserList::~CAddUserSelectDlgConnectUserList()
{
}

int CAddUserSelectDlgConnectUserList::AddItem(LPARAM lParam)
{
	int nRet = __super::AddItem(lParam);

	if(m_pDlg->IsCheckConnectUserAllSelectItem())
		m_pDlg->SetCheckConnectUserAllSelectItem(false);

	return nRet;
}

bool CAddUserSelectDlgConnectUserList::DeleteItem(LPARAM lParam)
{
	bool bRet = __super::DeleteItem(lParam);
	CheckAllItemCheck();
	return bRet;
}

void CAddUserSelectDlgConnectUserList::DeleteAllItem(void)
{
	__super::DeleteAllItem();
	m_pDlg->SetCheckBuddyAllSelectItem(false);
}

bool CAddUserSelectDlgConnectUserList::CheckAllItemCheck(void)
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
		if(!m_pDlg->IsCheckConnectUserAllSelectItem())
			m_pDlg->SetCheckConnectUserAllSelectItem(true);
	}
	else
	{
		if(m_pDlg->IsCheckConnectUserAllSelectItem())
			m_pDlg->SetCheckConnectUserAllSelectItem(false);
	}

	return bRet;
}

void CAddUserSelectDlgConnectUserList::ClearAllItemCheck(void)
{
	m_pDlg->ClearAllConnectUserCheck() ;
}
