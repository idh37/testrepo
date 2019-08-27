#include "stdafx.h"
#include "BadUserComplete.h"
#include "BadUserCompleteID.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CBadUserComplete::CBadUserComplete(CWnd* pParent /*=NULL*/)
	: LSY::CLSYLibDialogBase(pParent)
{
}

BEGIN_MESSAGE_MAP(CBadUserComplete, LSY::CLSYLibDialogBase)
END_MESSAGE_MAP()

INT_PTR CBadUserComplete::DoModal() 
{
	// TODO: Add your specialized code here and/or call the base class
	return __super::DoModal(GetObjectMan(), ID_NOTIFYCOMPLETE);
}

bool CBadUserComplete::OnCreatedProject(void)
{
#ifdef UPGRADE_20120308_TOCTOC_CHANNELING
	LSY::CImage *pImage = (LSY::CImage *)GetObject(ID_NOTIFYCOMPLETE_IMAGE_BACK);
	if (NMBASE::UTIL::GetCurSiteNo() == NMBASE::UTIL::SNO_TOCTOC)
		pImage->SetIndex(2);
	else
		pImage->SetIndex(1);
#endif //UPGRADE_20120308_TOCTOC_CHANNELING

	LSY::CButton *pButton = (LSY::CButton *)GetObject(ID_NOTIFYCOMPLETE_BTN_OK);
	pButton->AddHandler(LSY::EM_O_MOUSELCLICK, LSY::Fnt(this, &CBadUserComplete::OnClickButton));

	pButton = (LSY::CButton *)GetObject(ID_NOTIFYCOMPLETE_BTN_CANCEL);
	pButton->AddHandler(LSY::EM_O_MOUSELCLICK, LSY::Fnt(this, &CBadUserComplete::OnClickButton));

	return true;
}

LRESULT CBadUserComplete::OnClickButton(LSY::CMessage *pMsg)
{
	LSY::CObjectMessage *msg = (LSY::CObjectMessage *)pMsg;
	switch(msg->GetObject()->GetID())
	{
	case ID_NOTIFYCOMPLETE_BTN_OK:
		OnOK();
		break;
	case ID_NOTIFYCOMPLETE_BTN_CANCEL:
		OnCancel();
		break;
	}
	return TRUE;
}