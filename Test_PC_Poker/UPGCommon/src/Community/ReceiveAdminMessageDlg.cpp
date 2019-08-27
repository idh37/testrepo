#include "stdafx.h"
#include "ReceiveAdminMessageDlg.h"
#include "ReceiveAdminMessageDlgID.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CReceiveAdminMessageDlg::CReceiveAdminMessageDlg(CWnd* pParent/* = NULL*/)
:	LSY::CLSYLibDialogBase(pParent)
{
	m_cMemo.Clear();
}

BEGIN_MESSAGE_MAP(CReceiveAdminMessageDlg, LSY::CLSYLibDialogBase)
	ON_WM_DESTROY()
END_MESSAGE_MAP()

INT_PTR CReceiveAdminMessageDlg::DoModal(Camel::Dar2::SS::Memo *pMemo, CWnd *pParent)
{
	m_pParentWnd = pParent;
	m_cMemo.Copy(*pMemo);

	return CLSYLibDialogBase::DoModal(GetObjectMan(), ID_RECEIVEADMINMESSAGE);
}

bool CReceiveAdminMessageDlg::OnCreatedProject(void)
{
#ifdef UPGRADE_20120308_TOCTOC_CHANNELING
	LSY::CImage *pImage = (LSY::CImage *)GetObject(ID_RECEIVEADMINMESSAGE_IMAGE_BACK);
	if (NMBASE::UTIL::GetCurSiteNo() == NMBASE::UTIL::SNO_TOCTOC)
		pImage->SetIndex(11);
	else
		pImage->SetIndex(0);
#endif //UPGRADE_20120308_TOCTOC_CHANNELING


	LSY::CButton *pButton = (LSY::CButton *)GetObject(ID_RECEIVEADMINMESSAGE_BTN_MESSAGE_DELETE);
	pButton->AddHandler(LSY::EM_O_MOUSELCLICK, LSY::Fnt(this, &CReceiveAdminMessageDlg::OnClickButton));

	pButton = (LSY::CButton *)GetObject(ID_RECEIVEADMINMESSAGE_BTN_MESSAGE_SAVE);
	pButton->AddHandler(LSY::EM_O_MOUSELCLICK, LSY::Fnt(this, &CReceiveAdminMessageDlg::OnClickButton));

	LSY::CCheckBox *pCheckBox = (LSY::CCheckBox *)GetObject(ID_RECEIVEADMINMESSAGE_CHECK_NO_RECEIVE_MESSAGE);
	pCheckBox->AddHandler(LSY::EM_O_MOUSELCLICK, LSY::Fnt(this, &CReceiveAdminMessageDlg::OnClickButton));
	pCheckBox->SetCheck(false);

	pButton = (LSY::CButton *)GetObject(ID_RECEIVEADMINMESSAGE_BTN_CLOSE);
	pButton->AddHandler(LSY::EM_O_MOUSELCLICK, LSY::Fnt(this, &CReceiveAdminMessageDlg::OnClickButton));

	ChangeMemo(&m_cMemo);

	return TRUE;
}

LRESULT CReceiveAdminMessageDlg::OnClickButton(LSY::CMessage *pMsg)
{
	LSY::CObjectMessage *msg = (LSY::CObjectMessage *)pMsg;
	switch(msg->GetObject()->GetID())
	{
	case ID_RECEIVEADMINMESSAGE_BTN_MESSAGE_DELETE:
		if(!CCommunityManager::Instance()->IsConnect())
		{
			CMsgBoxDlg dlg(this);
			dlg.DoModal(CMsgBoxDlg::EMBT_NOTICE, "커뮤니티 서버와 접속이 되어 있지 않아 쪽지를 삭제할 수 없습니다.");
			break;
		}
		else
		{
			list<std::string> listMemoNode;
			listMemoNode.push_back(m_cMemo.node);
			CCommunityManager::Instance()->SendRemoveMemo(listMemoNode);
		}
		break;
	case ID_RECEIVEADMINMESSAGE_BTN_MESSAGE_SAVE:
		if(!CCommunityManager::Instance()->IsConnect())
		{
			CMsgBoxDlg dlg(this);
			dlg.DoModal(CMsgBoxDlg::EMBT_NOTICE, "커뮤니티 서버와 접속이 되어 있지 않아 쪽지를 쪽지보관함으로 이동할 수 없습니다.");
		}
		else
		{
			list<std::string> listMemoNode;
			listMemoNode.push_back(m_cMemo.node);
			CCommunityManager::Instance()->SendStorageMoveMemo(listMemoNode);
		}
		break;
	case ID_RECEIVEADMINMESSAGE_BTN_CLOSE:
		OnCancel();
		break;
	}
	return TRUE;
}

void CReceiveAdminMessageDlg::OnDestroy()
{
	LSY::CCheckBox *pCheckBox = (LSY::CCheckBox *)GetObject(ID_RECEIVEADMINMESSAGE_CHECK_NO_RECEIVE_MESSAGE);
	if(pCheckBox->IsCheck())
	{
		if(CCommunityManager::Instance()->IsConnect())
		{
			if(CCommunityManager::Instance()->GetOption(COM_ECOT_MEMO) != COM_ECOPT_OFF) CCommunityManager::Instance()->SetOption(COM_ECOT_MEMO, COM_ECOPT_OFF);
		}
		else
		{
			ShowMainMessageDlg("커뮤니티 서버와 접속되어있지 않아 쪽지 수신 여부가 적용되지 않았습니다.");
		}
	}

	__super::OnDestroy();
}

void CReceiveAdminMessageDlg::ChangeMemo(Camel::Dar2::SS::Memo *pMemo)
{
	m_cMemo.Copy(*pMemo);

	LSY::CRichText *pRich = (LSY::CRichText *)GetObject(ID_RECEIVEADMINMESSAGE_RICHTEXT_MESSAGE);
	pRich->AddText(m_cMemo.content);

	LSY::CText *pText = (LSY::CText *)GetObject(ID_RECEIVEADMINMESSAGE_TXT_SEND_TIME);
	pText->SetText(m_cMemo.recvTime);
}