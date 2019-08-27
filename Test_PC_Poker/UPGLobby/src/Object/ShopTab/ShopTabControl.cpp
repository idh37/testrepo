#include "StdAfx.h"
#include "ShopTabControl.h"
#include "../../WidgetID.h"

CShopTabControl::CShopTabControl()
:	m_pWebShop(NULL),
	m_pWebEvent(NULL)
{
}

CShopTabControl::~CShopTabControl()
{
}

void CShopTabControl::OnCreateChild(LSY::CObjectBase *pObject)
{
	if(pObject == NULL) return;

	switch(pObject->GetID())
	{
	case ID_LOBBY_WEB_SHOP:		m_pWebShop		= (LSY::CWeb *)pObject; break;
	case ID_LOBBY_WEB_EVENT:	m_pWebEvent		= (LSY::CWeb *)pObject; break;
	}

	__super::OnCreateChild(pObject);
}

void CShopTabControl::OnCreated(void)
{
	LSY::CObjectProject *pProject = GetObjectMan()->GetProjectFromID(ID_LOBBY);
	OnCreateChild(pProject->GetObject(ID_LOBBY_WEB_SHOP));
	OnCreateChild(pProject->GetObject(ID_LOBBY_WEB_EVENT));

	AddHandler(LSY::EM_O_SELCHANGE, LSY::Fnt(this, &CShopTabControl::OnSelTabChange));

	//SetCurSelFromObject((LSY::CRadio*)pProject->GetObject(ID_LOBBY_RAD_EVENT));
	//SetUseLogic(true);
	
	__super::OnCreated();
}

void CShopTabControl::RefreshWeb()
{
	LSY::CRadio *pRadio = GetCurSel();
	if(pRadio->GetID() == ID_LOBBY_RAD_SHOP)
	{
		m_pWebShop->SetURL(getGameURL(IDX_GAMEURL_LOBBY_SHOP).GetString());
	}
	else
	{
		m_pWebEvent->SetURL(getGameURL(IDX_GAMEURL_LOBBY_EVENT).GetString());
	}
}

LRESULT CShopTabControl::OnSelTabChange(LSY::CMessage *msg)
{
	LSY::CRadio *pRadio = GetCurSel();
	if(pRadio->GetID() == ID_LOBBY_RAD_SHOP)
	{
		SendUILog_Basic(CSSTATS_UINO_POKER_LOBBY_DP_AREA_RECOMMEND_ITEM);
	}
	else
	{
		SendUILog_Basic(CSSTATS_UINO_POKER_LOBBY_DP_AREA_EVENT);

		// JIS
		// 테스트용 강제 에러발생
		//NMBASE::UTIL::BugSplatWrapper::GetInstance().AddMemLog(CString("test1"));
		//*(volatile int *)0 = 0;
	}


	RefreshWeb();
	return TRUE;
}

void CShopTabControl::SetEventURL(std::string strURL)
{
	m_pWebEvent->SetURL(strURL);
}

//void CShopTabControl::Logic(const DWORD &dwTime)
//{
//	if(dwTime >= 5000)
//	{
//		LSY::CObjectProject *pProject = GetObjectMan()->GetProjectFromID(ID_LOBBY);
//		SetCurSelFromObject((LSY::CRadio*)pProject->GetObject(ID_LOBBY_RAD_SHOP));
//		SetUseLogic(false);
//	}
//}