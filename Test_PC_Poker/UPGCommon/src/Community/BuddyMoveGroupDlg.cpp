#include "stdafx.h"
#include "BuddyMoveGroupDlg.h"
#include "BuddyMoveGroupDlgID.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CBuddyMoveGroupDlg::CBuddyMoveGroupDlg(CWnd* pParent/* = NULL*/)
:	LSY::CLSYLibDialogBase(pParent),
	m_pWndBack(NULL),
	m_pTxtResult(NULL),
	m_pComboGroup(NULL),
	m_pBtnClose(NULL),
	m_pBtnOK(NULL),
	m_pBtnCancel(NULL)
{
}

BEGIN_MESSAGE_MAP(CBuddyMoveGroupDlg, LSY::CLSYLibDialogBase)
END_MESSAGE_MAP()

INT_PTR CBuddyMoveGroupDlg::DoModal(std::string strMessage, CWnd *pParent)
{
	m_pParentWnd = pParent;
	m_nType = BMGDT_MESSAGE;
	m_strMessage = strMessage;
	return CLSYLibDialogBase::DoModal(GetObjectMan(), ID_FRIENDMOVEGROUP);
}

INT_PTR CBuddyMoveGroupDlg::DoModal(std::string strID, std::string strGroup, CWnd *pParent)
{
	m_pParentWnd = pParent;
	m_nType = BMGDT_EDIT;
	m_strMessage = strGroup;
	m_strID = strID;
	return CLSYLibDialogBase::DoModal(GetObjectMan(), ID_FRIENDMOVEGROUP);
}

bool CBuddyMoveGroupDlg::OnCreatedProject(void)
{
	LSY::CObjectProject *pProject = m_pObjectMan->GetProjectFromID(m_nID);
	m_pWndBack = (LSY::CWindows *)pProject->GetMainObject();

	m_pTxtResult = (LSY::CText *)GetObject(ID_FRIENDMOVEGROUP_TXT_MESSAGE);
	m_pTxtResult->SetText(m_strMessage);

	m_pComboGroup = (LSY::CCombo *)GetObject(ID_FRIENDMOVEGROUP_COMBO_GROUP);

	m_pBtnOK = (LSY::CButton *)GetObject(ID_FRIENDMOVEGROUP_BTN_OK);
	m_pBtnOK->AddHandler(LSY::EM_O_MOUSELCLICK, LSY::Fnt(this, &CBuddyMoveGroupDlg::OnClickButton));

	m_pBtnCancel = (LSY::CButton *)GetObject(ID_FRIENDMOVEGROUP_BTN_CANCEL);
	m_pBtnCancel->AddHandler(LSY::EM_O_MOUSELCLICK, LSY::Fnt(this, &CBuddyMoveGroupDlg::OnClickButton));

	m_pBtnClose = (LSY::CButton *)GetObject(ID_FRIENDMOVEGROUP_BTN_CLOSE);
	m_pBtnClose->AddHandler(LSY::EM_O_MOUSELCLICK, LSY::Fnt(this, &CBuddyMoveGroupDlg::OnClickButton));

	switch(m_nType)
	{
	case BMGDT_EDIT:
		m_pWndBack->SetIndex(0);
		m_pTxtResult->SetShow(false);
		m_pComboGroup->SetShow(true);
		CCommunityManager::Instance()->SetComboGroupList(m_pComboGroup, m_strMessage);
		m_pBtnOK->SetShow(true);
		m_pBtnCancel->SetShow(true);
		m_pBtnClose->SetShow(false);		
		break;
	case BMGDT_MESSAGE:
		m_pWndBack->SetIndex(1);
		m_pTxtResult->SetShow(true);
		m_pTxtResult->SetText(m_strMessage);
		m_pComboGroup->SetShow(false);
		m_pBtnOK->SetShow(false);
		m_pBtnCancel->SetShow(false);
		m_pBtnClose->SetShow(true);
		break;
	default:
		ASSERT(FALSE);
		break;
	}

	return TRUE;
}

LRESULT CBuddyMoveGroupDlg::OnClickButton(LSY::CMessage *pMsg)
{
	LSY::CObjectMessage *msg = (LSY::CObjectMessage *)pMsg;
	switch(msg->GetObject()->GetID())
	{
	case ID_FRIENDMOVEGROUP_BTN_OK:
		{
			if(m_pComboGroup->GetText() != m_strMessage)
			{
				std::map<std::string, std::string> mapIDAndGroup;
				mapIDAndGroup[m_strID] = m_strMessage;

				CCommunityManager::Instance()->SendMoveBuddyToGroup(mapIDAndGroup, m_pComboGroup->GetText());
			}
			OnOK();
		}
		break;
	case ID_FRIENDMOVEGROUP_BTN_CANCEL:
		OnCancel();
		break;
	case ID_FRIENDMOVEGROUP_BTN_CLOSE:
		OnCancel();
		break;
	}
	return TRUE;
}