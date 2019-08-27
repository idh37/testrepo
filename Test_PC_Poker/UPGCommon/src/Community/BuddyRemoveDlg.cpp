#include "stdafx.h"
#include "BuddyRemoveDlg.h"
#include "BuddyRemoveDlgID.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CBuddyRemoveDlg::CBuddyRemoveDlg(CWnd* pParent/* = NULL*/)
:	LSY::CLSYLibDialogBase(pParent),
	m_pBtnClose(NULL),
	m_pBtnOK(NULL),
	m_pBtnCancel(NULL),
	m_pTxtResult(NULL)
{
}

BEGIN_MESSAGE_MAP(CBuddyRemoveDlg, LSY::CLSYLibDialogBase)
END_MESSAGE_MAP()

INT_PTR CBuddyRemoveDlg::DoModal(BUDDYREMOVEDLGTYPE nType, std::string strMessage, CWnd *pParent)
{
	m_pParentWnd = pParent;
	m_nStartType = nType;
	m_nCurType = nType;
	m_strMessage = strMessage;
	return CLSYLibDialogBase::DoModal(GetObjectMan(), ID_FRIENDREMOVEDLG);
}

bool CBuddyRemoveDlg::OnCreatedProject(void)
{
	m_pTxtResult = (LSY::CText *)GetObject(ID_FRIENDREMOVEDLG_TXT_RESULT);
	m_pTxtResult->SetText(m_strMessage);

	m_pBtnOK = (LSY::CButton *)GetObject(ID_FRIENDREMOVEDLG_BTN_OK);
	m_pBtnOK->AddHandler(LSY::EM_O_MOUSELCLICK, LSY::Fnt(this, &CBuddyRemoveDlg::OnClickButton));

	m_pBtnCancel = (LSY::CButton *)GetObject(ID_FRIENDREMOVEDLG_BTN_CANCEL);
	m_pBtnCancel->AddHandler(LSY::EM_O_MOUSELCLICK, LSY::Fnt(this, &CBuddyRemoveDlg::OnClickButton));

	m_pBtnClose = (LSY::CButton *)GetObject(ID_FRIENDREMOVEDLG_BTN_CLOSE);
	m_pBtnClose->AddHandler(LSY::EM_O_MOUSELCLICK, LSY::Fnt(this, &CBuddyRemoveDlg::OnClickButton));

	SetCurType(m_nStartType);

	return TRUE;
}

void CBuddyRemoveDlg::SetCurType(BUDDYREMOVEDLGTYPE nType)
{
	switch(nType)
	{
	case BRDT_QUESTION:
		m_pTxtResult->SetShow(true);
		m_pTxtResult->SetText("선택한 친구를 삭제하시겠습니까?\n확인 버튼을 누르시면 친구목록에서 삭제됩니다.");
		
		m_pBtnOK->SetShow(true);
		m_pBtnCancel->SetShow(true);
		m_pBtnClose->SetShow(false);
		break;
	case BRDT_MESSAGE:
		m_pTxtResult->SetShow(true);

		m_pBtnOK->SetShow(false);
		m_pBtnCancel->SetShow(false);
		m_pBtnClose->SetShow(true);
		break;
	}

	m_nCurType = nType;
}

LRESULT CBuddyRemoveDlg::OnClickButton(LSY::CMessage *pMsg)
{
	LSY::CObjectMessage *msg = (LSY::CObjectMessage *)pMsg;
	switch(msg->GetObject()->GetID())
	{
	case ID_FRIENDREMOVEDLG_BTN_OK:
		OnOK();
		break;
	case ID_FRIENDREMOVEDLG_BTN_CANCEL:
		OnCancel();
		break;
	case ID_FRIENDREMOVEDLG_BTN_CLOSE:
		OnCancel();
		break;
	}
	return TRUE;
}