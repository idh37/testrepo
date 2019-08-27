#include "stdafx.h"
#include "GameOverlay.h"
#include "GameOverlayid.h"
#include "VIPJackpotAlert/VIPJackpotAlertGroup.h"
#include "VIPJackpotAlert/VIPJackpotAlertGuide.h"

CGameOverlay::CGameOverlay()
: m_pVIPJackpotAlertGroup(NULL)
{

}

CGameOverlay::~CGameOverlay()
{

}

LRESULT CGameOverlay::OnCallCreateObject( LSY::CMessage *msg )
{
	LRESULT hRet = NULL;

	typedef LRESULT (*CallCreateObjFunc)(LSY::CMessage * , CGameOverlay* );
	std::vector<CallCreateObjFunc> vecCreateObj;

	/////////////객체생성 함수등록 구간
	vecCreateObj.push_back(&CGameOverlay::_OnCallCreateObject_VIPJackpotAlert);
	//////////////////////////////////////////////////////////

	std::vector<CallCreateObjFunc>::iterator vit = vecCreateObj.begin();
	while(vit != vecCreateObj.end())
	{
		hRet = (*vit)(msg , this);

		if (hRet)
			return hRet;

		++vit;
	}

	return NULL;	
}

LRESULT CGameOverlay::_OnCallCreateObject_VIPJackpotAlert( LSY::CMessage *msg_ , CGameOverlay* pObj_ )
{
	if (NULL == pObj_)
		return NULL;

	LSY::CMO_CreateObject *pMsg = (LSY::CMO_CreateObject *)msg_;

	LSY::CObjectBase* pObj = NULL;

	switch(*pMsg->GetID())
	{
	case ID_GAMEOVERLAY_GRP_VIPJACKPOT:
		pObj_->m_pVIPJackpotAlertGroup = new CVIPJackpotAlertGroup();
		return (LRESULT)pObj_->m_pVIPJackpotAlertGroup;
	case ID_GAMEOVERLAY_DLG_VIPJACKPOT_INFO:
		pObj = new CVIPJackpotAlertGuide();
		break;
	}

	return (LRESULT)pObj;
}

BOOL CGameOverlay::Init( CWnd* pWnd_ ) 
{
	LSY::CObjectMan *pObjectMan = GetObjectMan();
	if (NULL == pObjectMan)
	{
		pObjectMan = CreateObjectMan(pWnd_, GM().GetMainWnd());
		pObjectMan->LoadXml("poker.lsyproj");
		pObjectMan->SetUseCustomCursor(true);
	}

	LSY::CObjectProject *pProject = pObjectMan->GetProjectFromID(ID_GAMEOVERLAY);
	if (pProject)
	{
		pProject->SetCallCreateObject(LSY::Fnt(this, &CGameOverlay::OnCallCreateObject));
		pProject->LoadXml();
	}

	return TRUE;

}
