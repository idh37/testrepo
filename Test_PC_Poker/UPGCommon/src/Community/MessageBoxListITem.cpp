#include "stdafx.h"
#include "MessageBoxListItem.h"
#include "messageboxid.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CMessageBoxListItem::CMessageBoxListItem()
:	m_pCheck(NULL),
	m_pTxtSender(NULL),
	m_pTxtTitle(NULL),
	m_pTxtDate(NULL)
{
}

CMessageBoxListItem::~CMessageBoxListItem()
{
}

void CMessageBoxListItem::OnCreateChild(LSY::CObjectBase *pObject)
{
	switch(pObject->GetID())
	{
	case ID_MESSAGEBOX_CHECK_LISTITEM_SELECT:
		m_pCheck = (LSY::CCheckBox *)pObject;
		break;
	case ID_MESSAGEBOX_TXT_LISTITEM_SENDER:
		m_pTxtSender = (LSY::CText *)pObject;
		break;
	case ID_MESSAGEBOX_TXT_LISTITEM_MESSAGE_TITLE:
		m_pTxtTitle = (LSY::CText *)pObject;
		break;
	case ID_MESSAGEBOX_TXT_LISTITEM_MESSAGE_DATE:
		m_pTxtDate = (LSY::CText *)pObject;
		break;
	}
}

LSY::CObjectBase *CMessageBoxListItem::NewObject(void)
{
	return new CMessageBoxListItem;
}

void CMessageBoxListItem::UpdateData(const LPARAM &lParam)
{
	LSY::CListItemData *pData = GetData();
	MEMO *pMemo = (MEMO *)pData->GetItemData();
	if(pMemo == NULL) return;

	if(pData->GetUserData(0) == 0)
	{
		pData->SetUserData(0, 1);
		m_pCheck->SetCheck(false);
	}

	int nIndex = 0;
	
	std::string strSender;
	std::string strTitle;

	if(CCommunityManager::IsAdminMemo(pMemo))
	{
		if(pMemo->read)	nIndex = 14;
		else nIndex = 13;

		strSender = "";
		strTitle = "운영자 메시지입니다.";
	}
	else
	{
		if(pMemo->read)	nIndex = 12;
		else nIndex = 11;

		strSender = pMemo->senderID;
		strTitle = pMemo->content;
	}

	COLORREF clrTextColor = RGB(70, 109, 176);
	bool bBold = true;
	if(pMemo->read)
	{
		clrTextColor = RGB(107, 107, 107);
		bBold = false;
	}

	std::string strFontName = m_pTxtSender->GetFontName();
	int nFontSize = m_pTxtSender->GetFontSize();

	m_pTxtSender->SetTextColor(clrTextColor);
	m_pTxtTitle->SetTextColor(clrTextColor);
	m_pTxtDate->SetTextColor(clrTextColor);

	m_pTxtSender->SetFont(strFontName, nFontSize, bBold);
	m_pTxtTitle->SetFont(strFontName, nFontSize, bBold);
	m_pTxtDate->SetFont(strFontName, nFontSize, bBold);

	m_pTxtSender->SetText(strSender);
	m_pTxtTitle->SetText(strTitle);
	m_pTxtDate->SetText(pMemo->recvTime);

	int listStatus[LSY::EBS_MAX] = {0, };
	for(int i=(int)LSY::EBS_NORMAL;i<(int)LSY::EBS_MAX;++i)
		listStatus[i] = nIndex;

	SetFreeStatusButton(listStatus);
}