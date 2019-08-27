#include "StdAfx.h"
#include "ShopView.h"

CShopView::CShopView(void)
{
}

CShopView::~CShopView(void)
{
}


void CShopView::EnterWidget()
{

	SetShow(TRUE);
	NavigateSSOWeb(getGameURL(IDX_GAMEURL_LOBBY_SHOP));
	//NavigateSSOWeb("http://game1.netmarble.net/agame/gameshop/shop/?gamecode=spoker2&channel=3&tab=1");


	CSSOWebWidget::EnterWidget();
}

void CShopView::ExitWidget()
{
	SetShow(FALSE);	

	CSSOWebWidget::ExitWidget();
}