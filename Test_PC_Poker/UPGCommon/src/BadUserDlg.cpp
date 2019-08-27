#include "stdafx.h"
#include "BadUserDlg.h"
#include "BadUserDlgID.h"
#include "baduserscamid.h"
#include "MsgBoxDlg.h"
#include "GlobalBase.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

int	CBadUserDlg::ms_nSubmitCnt = 0;

CBadUserDlg::CBadUserDlg(CWnd* pParent /*=NULL*/)
	: LSY::CLSYLibDialogBase(pParent)
{
	m_strSubmitReason	= _T("");
	m_strTargetID		= _T("");
	m_strTargetNickName	= _T("");
	m_nKind				= REPORT_KIND_NULL;
	m_bEditNoClick		= true;
}

CBadUserDlg::~CBadUserDlg()
{
	ClearIDList();
}

BEGIN_MESSAGE_MAP(CBadUserDlg, LSY::CLSYLibDialogBase)
END_MESSAGE_MAP()

bool CBadUserDlg::OnCreatedProject(void)
{
	if(m_vIDList.size() == 0)
	{
		ShowMainMessageDlg("신고할 대상자가 없습니다.");
		return false;
	}

	if(m_nKind != REPORT_KIND_ONLY_SCAM)
	{
		_LoadCommonTypeDlgData();		
	}
	else
	{
		_LoadOnlyScamTypeDlgData();
	}	

	return true;
}

void CBadUserDlg::SetReasonTextCount(int nCount)
{
	LSY::CEdit *pEdit = (LSY::CEdit *)GetObject(ID_NOTIFY_EDT_REASON);
	CString strTemp;
	strTemp.Format("(%d/%d 바이트)", nCount, pEdit->GetLimitText());

	LSY::CText *pText = (LSY::CText *)GetObject(ID_NOTIFY_TXT_REASON_COUNT);
	pText->SetText(strTemp.GetString());
}

LRESULT CBadUserDlg::OnClickButton(LSY::CMessage *pMsg)
{
	LSY::CObjectMessage *msg = (LSY::CObjectMessage *)pMsg;
	switch(msg->GetObject()->GetID())
	{
	case ID_NOTIFY_BTN_OK:
		OnOK();
		break;
	case ID_NOTIFY_BTN_CANCEL:
		OnCancel();
		break;
	}
	return TRUE;
}

LRESULT CBadUserDlg::OnFocusIn(LSY::CMessage *pMsg)
{
	if(m_bEditNoClick)
	{
		LSY::CMO_FocusIn *msg = (LSY::CMO_FocusIn *)pMsg;
		LSY::CEdit *pEdit = (LSY::CEdit *)msg->GetObject();
		pEdit->SetText("");
		m_bEditNoClick = false;
	}

	return NULL;
}

LRESULT CBadUserDlg::OnSelChange(LSY::CMessage *pMsg)
{
	LSY::CMO_SelChange *msg = (LSY::CMO_SelChange *)pMsg;
	LSY::CTextList *pList = (LSY::CTextList *)msg->GetObject();
	int nCurSel = msg->GetValue();
	m_strTargetNickName = pList->GetItemString(nCurSel).c_str();

	LSY::CText *pText = 0;

	if(m_nKind != REPORT_KIND_ONLY_SCAM)
	{
		pText = (LSY::CText *)GetObject(ID_NOTIFY_TXT_NOTIFY_USER);
	}
	else
	{
		pText = (LSY::CText *)GetObject(ID_NOTIFYSCAM_TXT_NOTIFY_USER);		
	}
	
	pText->SetText(m_strTargetNickName.GetString());

	return TRUE;
}

LRESULT CBadUserDlg::OnKeyProc(LSY::CMessage *pMsg)
{
	LSY::CMO_KeyMessageProc *msg = (LSY::CMO_KeyMessageProc *)pMsg;
	LSY::CEdit *pEdit = (LSY::CEdit *)msg->GetObject();
	SetReasonTextCount(pEdit->GetLength());
	return TRUE;
}

void CBadUserDlg::OnOK() 
{
	// TODO: Add extra validation here
	UpdateData(TRUE);	

	CMsgBoxDlg msgdlg(this);

	m_strTargetID  = FindID( m_strTargetNickName );
	if ( m_strTargetID.GetLength() == 0 )
	{
		msgdlg.DoModal(CMsgBoxDlg::EMBT_NOTICE, "선택된 닉네임의 Id가 검색되지 않습니다. 다시 시도해 주세요");
		return;
	}

	if(m_nKind != REPORT_KIND_ONLY_SCAM)
	{		
	// 	if(ms_nSubmitCnt >= 3)
	// 	{
	// 		msgdlg.DoModal(CMsgBoxDlg::EMBT_NOTICE, "접속 후 총 세 번 이상 신고할 수 없습니다.");				
	// 		return;
	// 	}
		LSY::CGroup *pGroup = (LSY::CGroup *)GetObject(ID_NOTIFY_GRP_NOTIFY_TYPE);
		m_nKind = (REPORT_KIND)(pGroup->GetCurSelIndex() + 1);

		if (m_nKind==REPORT_KIND_SWEAR)
		{
			LSY::CTextList *pTxtList = (LSY::CTextList *)GetObject(ID_NOTIFY_TXTLIST_USER);
			int nSel = pTxtList->GetCurSelectItem();
			if ( nSel < 0 )
			{
				msgdlg.DoModal(CMsgBoxDlg::EMBT_NOTICE, "신고 대상 닉네임을 선택하세요.");
				return;
			}
		}		

		LSY::CEdit *pEdit = (LSY::CEdit *)GetObject(ID_NOTIFY_EDT_REASON);
		m_strSubmitReason = pEdit->GetText().c_str();

		if(m_bEditNoClick || (m_strSubmitReason.GetLength() < 1))
		{
			msgdlg.DoModal(CMsgBoxDlg::EMBT_NOTICE, "신고 사유를 기입하세요.");
			return;
		}

	// 	if (msgdlg.DoModal(CMsgBoxDlg::EMBT_QUESTION, "허위 신고 또는 장난 신고인 경우 불이익을 받게 됩니다. 정말로 신고하시겠습니까?") != IDOK)
	// 	{
	// 		m_nKind = REPORT_KIND_NULL;
	// 		LSY::CLSYLibDialogBase::OnCancel(); 
	// 		return;
	// 	}		
	}

	g_TargetID = m_strTargetNickName;

	LSY::CLSYLibDialogBase::OnOK();
}

void CBadUserDlg::OnCancel()
{
	m_nKind = REPORT_KIND_NULL;

	LSY::CLSYLibDialogBase::OnCancel();
}

void CBadUserDlg::AddUser( char *szID, char *szNick )
{
	if(g_LoginID.Compare( szID ) == 0) return;
	if(szID == NULL || szNick == NULL) return;
	if(strlen(szID) == 0 || strlen(szNick) == NULL) return;

	bool bErase = false;
	
	IDLIST_ITEM	stItem;
	list< IDLIST_ITEM >::iterator start = m_vIDList.begin();
	list< IDLIST_ITEM >::iterator end = m_vIDList.end();

	for ( ; start != end ; start++ )
	{
		stItem = (*start);
		
		if ( strncmp( stItem.szID, szID, sizeof(stItem.szID) ) == 0 )
		{
			if ( strncmp( stItem.szNick, szNick, sizeof(stItem.szNick) ) != 0 )
			{
				bErase = true;
				break;
			}
			else return;
		}
	}
	
	if ( bErase )
		m_vIDList.erase( start );
	
	strncpy( stItem.szID, szID, sizeof(stItem.szID) );
	strncpy( stItem.szNick, szNick, sizeof(stItem.szNick) );
	
	m_vIDList.push_back( stItem );
}

CString	CBadUserDlg::FindID( CString szNick )
{
	list< IDLIST_ITEM >::iterator start = m_vIDList.begin();
	list< IDLIST_ITEM >::iterator end = m_vIDList.end();

	for ( ; start != end ; start++ )
	{
		IDLIST_ITEM stItem = (*start);
		
		if ( strncmp( stItem.szNick, szNick, sizeof(stItem.szNick) ) == 0 )
		{
			return (LPCTSTR)stItem.szID;
		}
	}
	return _T("");
}

void CBadUserDlg::ClearIDList()
{
	m_vIDList.clear();
}

INT_PTR CBadUserDlg::DoModal()
{
	int nID;

	if(m_nKind == REPORT_KIND_ONLY_SCAM)
	{
		nID = ID_NOTIFYSCAM;
	}
	else
	{
		nID = ID_NOTIFY;
	}
	
	return __super::DoModal(GetObjectMan(), nID);
}

void CBadUserDlg::_LoadCommonTypeDlgData()
{
	LSY::CButton *pButton = (LSY::CButton *)GetObject(ID_NOTIFY_BTN_OK);
	pButton->AddHandler(LSY::EM_O_MOUSELCLICK, LSY::Fnt(this, &CBadUserDlg::OnClickButton));

	pButton = (LSY::CButton *)GetObject(ID_NOTIFY_BTN_CANCEL);
	pButton->AddHandler(LSY::EM_O_MOUSELCLICK, LSY::Fnt(this, &CBadUserDlg::OnClickButton));

	LSY::CGroup *pGroup = (LSY::CGroup *)GetObject(ID_NOTIFY_GRP_NOTIFY_TYPE);
	pGroup->SetCurSelFromIndex(0);

	LSY::CText *pText = (LSY::CText *)GetObject(ID_NOTIFY_TXT_NOTIFY_USER);
	pText->SetText("");

	LSY::CTextList *pTxtList = (LSY::CTextList *)GetObject(ID_NOTIFY_TXTLIST_USER);
	pTxtList->AddHandler(LSY::EM_O_SELCHANGE, LSY::Fnt(this, &CBadUserDlg::OnSelChange));

	list< IDLIST_ITEM >::iterator start = m_vIDList.begin();
	list< IDLIST_ITEM >::iterator end = m_vIDList.end();
	IDLIST_ITEM *pItem = NULL;

	for(;start != end;++start)
	{
		pItem = &(*start);
		pTxtList->AddText(pItem->szNick);
	}

	if(m_vIDList.size() == 1)
	{
		pTxtList->SetCurSelectItem(0);
		pText->SetText(m_vIDList.front().szNick);
		m_strTargetNickName = pText->GetText().c_str();
	}

	LSY::CEdit *pEdit = (LSY::CEdit *)GetObject(ID_NOTIFY_EDT_REASON);
	pEdit->AddHandler(LSY::EM_O_FOCUSIN, LSY::Fnt(this, &CBadUserDlg::OnFocusIn));
	pEdit->AddHandler(LSY::EM_O_KEYMESSAGEPROC, LSY::Fnt(this, &CBadUserDlg::OnKeyProc));

	pEdit->SetText("신고사유를 입력해 주세요!");

	SetReasonTextCount(0);
}

void CBadUserDlg::_LoadOnlyScamTypeDlgData()
{
	LSY::CButton *pButton = (LSY::CButton *)GetObject(ID_NOTIFYSCAM_BTN_OK);
	pButton->AddHandler(LSY::EM_O_MOUSELCLICK, LSY::Fnt(this, &CBadUserDlg::OnClickButton));

	pButton = (LSY::CButton *)GetObject(ID_NOTIFYSCAM_BTN_CANCEL);
	pButton->AddHandler(LSY::EM_O_MOUSELCLICK, LSY::Fnt(this, &CBadUserDlg::OnClickButton));

	LSY::CText *pText = (LSY::CText *)GetObject(ID_NOTIFYSCAM_TXT_NOTIFY_USER);
	pText->SetText("");

	LSY::CTextList *pTxtList = (LSY::CTextList *)GetObject(ID_NOTIFYSCAM_TXTLIST_USER);
	pTxtList->AddHandler(LSY::EM_O_SELCHANGE, LSY::Fnt(this, &CBadUserDlg::OnSelChange));

	list< IDLIST_ITEM >::iterator start = m_vIDList.begin();
	list< IDLIST_ITEM >::iterator end = m_vIDList.end();
	IDLIST_ITEM *pItem = NULL;

	for(;start != end;++start)
	{
		pItem = &(*start);
		pTxtList->AddText(pItem->szNick);
	}

	if(m_vIDList.size() == 1)
	{
		pTxtList->SetCurSelectItem(0);
		pText->SetText(m_vIDList.front().szNick);
		m_strTargetNickName = pText->GetText().c_str();
	}
}