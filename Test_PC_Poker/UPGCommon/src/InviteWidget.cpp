#include "StdAfx.h"
#include "InviteWidget.h"



//////////////////////////////////////////////////////////////////////////////
// CInviteUnitWidget
CInviteUnitWidget::CInviteUnitWidget()
{
	m_nControlID = NMBASE::SKINGDI::WIDGET_ID_NULL;
	m_pParent = NULL;
	m_rectPosition.SetRect(0,0,0,0);
	m_rectTargetPosition = m_rectPosition;

	m_bOver = FALSE;
	m_dwOverTick = 0;
	m_bDown = FALSE;
	m_dwDownTick = 0;

	m_strOverSnd = "";
	m_strDownSnd = "";	


	m_rectOverLayer.SetRect(0,0,0,0);
	m_rectOverLimit.SetRect(0,0,0,0);

	m_dwSetInTick = 0;

	m_pSprInvitaion=NULL;
	m_pSprInviteUnit=NULL;


	ZeroMemory(&m_UserInfo, sizeof(m_UserInfo));
	m_nState = STATE_NULL;
	m_bChecked = FALSE;
}
CInviteUnitWidget::~CInviteUnitWidget()
{

}


void CInviteUnitWidget::SetSound( char* szOver, char* szDown )
{
	m_strOverSnd = szOver;
	m_strDownSnd = szDown;

}



void CInviteUnitWidget::DrawWidget( NMBASE::GRAPHICGDI::CPage& Page, CDC& DC )
{

	// 배경
	if (m_pSprInviteUnit)
		Page.PutSprAuto( m_rectPosition.left, m_rectPosition.top, m_pSprInviteUnit, 0);

	if (m_nState==STATE_NULL)
	{
		return;
	}

	CRect rectText;	CString strText;

#define MAKE_COLUMN_RECT(head,tail) rectText.SetRect(m_rectPosition.left+head, m_rectPosition.top, m_rectPosition.left+tail, m_rectPosition.bottom);
//#define MAKE_COLUMN_COORD(x,y) coord_x= m_rectPosition.left+x; coord_y=m_rectPosition.top+y

	
	DC.SelectObject( FONT_DEFAULT );

	//닉네임
	MAKE_COLUMN_RECT(25,25+90);
	strText = m_UserInfo.NickName;
	DC.DrawText(strText, rectText, DT_LEFT|DT_SINGLELINE|DT_VCENTER|DT_END_ELLIPSIS);

	//보유머니
	MAKE_COLUMN_RECT(252,252+75);
	strText = NMBASE::UTIL::g_MakeShortMoneyStrEx(m_UserInfo.PMoney);
	DC.DrawText(strText, rectText, DT_RIGHT|DT_SINGLELINE|DT_VCENTER);

	// 	inline char *g_GetGameName(IDX_GAME idxGame)
	// 	{
	// 		static char *s_GameName[]={"없음", "7포커", "로우바둑이", "맞포커", "뉴포커", "",""};
	// 
	// 		if (idxGame <0 || idxGame>=MAX_GAME)
	// 		{
	// 			return s_GameName[0];
	// 		}
	// 
	// 		return s_GameName[(int)idxGame+1];
	// 	}


	//위치(게임)
	MAKE_COLUMN_RECT(138,138+54);
	rectText.OffsetRect(0,1);
	DrawGameText(DC, (IDX_GAME)m_UserInfo.nGameIndex, &rectText, DT_CENTER|DT_SINGLELINE|DT_VCENTER);
	rectText.OffsetRect(0,-1);

	//위치(방번호)
	MAKE_COLUMN_RECT(195,195+37);
	if (m_UserInfo.RoomNum==0)
		strText.Format("대기실");
	else
		strText.Format("%d번", m_UserInfo.RoomNum);
	DC.DrawText(strText, rectText, DT_RIGHT|DT_SINGLELINE|DT_VCENTER);


	if (m_pSprInvitaion)
		Page.PutSprAuto(m_rectPosition.left+7, m_rectPosition.top+5, m_pSprInvitaion, m_bChecked==0?17:18);


	//	Page.DrawEmptyBox(m_rectPosition.left, m_rectPosition.top, m_rectPosition.right, m_rectPosition.bottom, NMBASE::GRAPHICGDI::xSprite::RGB555(244,0,0));


}

BOOL CInviteUnitWidget::OnMouseMoveWidget( UINT nFlags, CPoint point )
{
	return NMBASE::SKINGDI::CNMUnitWidget::OnMouseMoveWidget(nFlags, point);
}

BOOL CInviteUnitWidget::OnMouseLeaveWidget( UINT nFlags, CPoint point )
{
	return NMBASE::SKINGDI::CNMUnitWidget::OnMouseLeaveWidget(nFlags, point);
}

BOOL CInviteUnitWidget::OnLButtonDownWidget( UINT nFlags, CPoint point )
{
	if (m_rectPosition.PtInRect(point))
	{
		m_bChecked = !m_bChecked;
		return TRUE;
	}
	else
		return FALSE;
	//return NMBASE::SKINGDI::CNMUnitWidget::OnLButtonDownWidget(nFlags, point);
}
BOOL CInviteUnitWidget::OnLButtonDbClkWidget( UINT nFlags, CPoint point )
{
	if (m_rectPosition.PtInRect(point))
	{
		AskProfileToServer(m_UserInfo.llUserKey, m_UserInfo.ID);
		return TRUE;
	}

	return FALSE;
}

BOOL CInviteUnitWidget::OnLButtonUpWidget( UINT nFlags, CPoint point )
{
	return NMBASE::SKINGDI::CNMUnitWidget::OnLButtonUpWidget(nFlags, point);
}

void CInviteUnitWidget::SetUserInfo( SMALLUSERINFO* pUserInfo, BOOL bChecked )
{
	if( pUserInfo == NULL )
	{
		m_nState = STATE_NULL;
		m_bChecked = FALSE;

		ZeroMemory( &m_UserInfo, sizeof(m_UserInfo) );
		return;
	}


	m_nState = STATE_ONLINE;
	m_bChecked = bChecked;

	memcpy( &m_UserInfo, pUserInfo, sizeof(m_UserInfo) );

	m_dwOverTick = GetTickCount();
}

void CInviteUnitWidget::SetSetInTick( DWORD dwdelay )
{
	m_dwSetInTick = GetTickCount() + dwdelay;
}


void CInviteUnitWidget::SetSprite(NMBASE::GRAPHICGDI::xSprite *pSprInvitaion, NMBASE::GRAPHICGDI::xSprite *pSprInviteUnit)
{
	m_pSprInvitaion= pSprInvitaion;
	m_pSprInviteUnit= pSprInviteUnit;
}


//-------------------------------------------------------------------------------------------------







CInviteListWidget::CInviteListWidget(void)
{
	m_pParent=NULL;
	m_pDataUserList =NULL;

	m_FilterKey = CDataUserList::KEY_FILTER_NULL;
}

CInviteListWidget::~CInviteListWidget(void)
{
	m_map_userinvite.clear();
}


BOOL CInviteListWidget::CreateWidget( CWnd* pParent)
{
	m_pParent = pParent;

	//닉네임
	NMBASE::SKINGDI::CNMToggleButton *pButton = new NMBASE::SKINGDI::CNMToggleButton();
	pButton->Initialize( INVITE_BUTTON_ID_INVITELIST_SORT_NICKNAME, pParent, 0, 0 );	
	pButton->SetFont(FONT_DEFAULT);	
	pButton->SetTextColor(RGB(120,124,126), RGB(84,88,90), RGB(120,124,126));	
	pButton->AddToggleStateTextBtn("닉네임", 121, 26);
	pButton->AddToggleStateTextBtn("닉네임", 121, 26);
	pButton->SetTextOffset(0,2);
	//pButton->SetEnable(false);

	AddSortButton(pButton);


	//위치 
	NMBASE::SKINGDI::CNMToggleButton *pButton2 = new NMBASE::SKINGDI::CNMToggleButton();
	pButton2->Initialize( INVITE_BUTTON_ID_INVITELIST_SORT_POSITION, pParent, 0, 0 );	
	pButton2->SetFont(FONT_DEFAULT);	
	pButton2->SetTextColor(RGB(120,124,126), RGB(84,88,90), RGB(120,124,126));	
	pButton2->AddToggleStateTextBtn("위치", 105, 26);
	pButton2->AddToggleStateTextBtn("위치", 105, 26);
	pButton2->SetTextOffset(0,2);
	//pButton2->SetEnable(false);

	AddSortButton(pButton2);

	//보유머니
	NMBASE::SKINGDI::CNMToggleButton *pButton3 = new NMBASE::SKINGDI::CNMToggleButton();
	pButton3->Initialize( INVITE_BUTTON_ID_INVITELIST_SORT_MONEY, pParent, 0, 0 );	
	pButton3->SetFont(FONT_DEFAULT);	
	pButton3->SetTextColor(RGB(120,124,126), RGB(84,88,90), RGB(120,124,126));	
	pButton3->AddToggleStateTextBtn("보유머니", 100, 26);
	pButton3->AddToggleStateTextBtn("보유머니", 100, 26);
	pButton3->SetTextOffset(0,2);
	//pButton3->SetEnable(false);

	AddSortButton(pButton3);




	
	//스크롤버튼
	NMBASE::SKINGDI::CNMButton *pButtonUp = new NMBASE::SKINGDI::CNMButton();
	pButtonUp->Initialize( INVITE_WIDGET_ID_INVITELIST_UPBUTTON, pParent, 0, 0, "Invitation", 9, 4 );	
	//pButtonUp->SetSound( "UI-SND_BUTTON_OVER9", "UI-SND_BUTTON_CLICK9" );
	
	NMBASE::SKINGDI::CNMButton *pButtonDown = new NMBASE::SKINGDI::CNMButton();
	pButtonDown->Initialize( INVITE_WIDGET_ID_INVITELIST_DOWNBUTTON, pParent, 0, 0, "Invitation", 13, 4 );
	//pButtonDown->SetSound( "UI-SND_BUTTON_OVER9", "UI-SND_BUTTON_CLICK9" );
	
	NMBASE::SKINGDI::CNMThumButton *pButtonThum = new NMBASE::SKINGDI::CNMThumButton();
	pButtonThum->Initialize( INVITE_WIDGET_ID_INVITELIST_THUMBBUTTON, pParent, 0, 0, "Invitation", 5, 4 );
	//pButtonThum->SetSound( "UI-SND_BUTTON_OVER9", "UI-SND_BUTTON_CLICK9" );

	SetScrollButton(pButtonUp, pButtonDown, pButtonThum);

	return CNMListWidget::CreateWidget(pParent);

}



BOOL CInviteListWidget::Initialize(int nControlID, CWnd *pParent, NMBASE::GRAPHICGDI::xSprite *pSprUnit, int nPosX, int nPosY, CDataUserList *pDataUserList)
{

	m_pDataUserList = pDataUserList;

	return CNMListWidget::Initialize(nControlID, pParent, pSprUnit, nPosX, nPosY);
}


void CInviteListWidget::EnterWidget()
{
	ClearCheckedUser(); //선택유저 삭제
	OnResetDataList();

	CNMListWidget::EnterWidget();
}
void CInviteListWidget::DrawWidget( NMBASE::GRAPHICGDI::CPage& Page, CDC& DC )
{

	CNMListWidget::DrawWidget( Page, DC );
}
BOOL CInviteListWidget::OnMouseMoveWidget( UINT nFlags, CPoint point )
{

	return CNMListWidget::OnMouseMoveWidget( nFlags, point );
}

BOOL CInviteListWidget::OnLButtonDownWidget( UINT nFlags, CPoint point )
{

	for(int i=0; i< (int)m_vec_Units.size(); i++)
	{
		CInviteUnitWidget *pUserUnit= (CInviteUnitWidget*) m_vec_Units[i];
		if (pUserUnit->OnLButtonDownWidget(nFlags, point))
		{
			//Unit이 클릭되었다.체크유저 추가/삭제
			if (pUserUnit->GetCheck())
			{				
				AddCheckedUser(pUserUnit->GetUserInfo());					
			}
			else
			{
				DelCheckedUser(pUserUnit->GetUserInfo());
			}

			return TRUE;
		}
	}

	return CNMListWidget::OnLButtonDownWidget(nFlags, point);

}

BOOL CInviteListWidget::OnCommandWidget( int nCommand, WPARAM wParam, LPARAM lParam )
{

	return CNMListWidget::OnCommandWidget( nCommand, wParam, lParam);
}





NMBASE::SKINGDI::CNMUnitWidget *CInviteListWidget::CreateNewUnitWidget(int nIndex)
{
	CInviteUnitWidget *pUserUnit= new CInviteUnitWidget();

	pUserUnit->Initialize( m_nControlID+ nIndex, m_pParent, m_pSprListUnit, 0, 0 );
	pUserUnit->SetSprite(NMBASE::SKINGDI::NMRESSM.GetSprite( "Invitation" ), m_pSprListUnit);
	AttachWidget( m_nControlID+nIndex, pUserUnit );	

	return pUserUnit;

}
int CInviteListWidget::GetTotalDataCount()
{
	//데이터의 총갯수 반환

	if (m_pDataUserList==NULL) 
		return 0;




	return m_pDataUserList->GetSize(m_FilterKey);

}


BOOL CInviteListWidget::RenewalList()
{
	if (CNMListWidget::RenewalList()==FALSE)
		return FALSE;

	if (m_pDataUserList==NULL) 
		return FALSE;

	int nStartIndex = m_pButtonThum->GetStartElement();

	CDataUserList::DATAPOSITION pos;
	SMALLUSERINFO *pUserinfo = m_pDataUserList->GetDataAt(m_FilterKey, pos, nStartIndex);

	int i = 0;
	while( i<(int)m_vec_Units.size())
	{
		CInviteUnitWidget* pUserUnit = (CInviteUnitWidget*) m_vec_Units[i++];
		ASSERT(pUserUnit);		

		BOOL bChecked=IsInviteCheckedUser(pUserinfo);//체크된 유저인가?

		if(pUserUnit)
		{
			pUserUnit->SetUserInfo(pUserinfo, bChecked);
		}

		pUserinfo = m_pDataUserList->GetNextData(m_FilterKey, pos);
	}

	return TRUE;

}

BOOL CInviteListWidget::IsInviteCheckedUser(SMALLUSERINFO *pUI)
{
	if (pUI==NULL) 
		return FALSE;

	return m_map_userinvite.find(pUI->ID)!=m_map_userinvite.end();

}
void CInviteListWidget::AddCheckedUser(SMALLUSERINFO *pUI)
{

	if (IsInviteCheckedUser(pUI)==FALSE)
	{
		m_map_userinvite[pUI->ID]= pUI->llUserKey;
	}

}
void CInviteListWidget::DelCheckedUser(SMALLUSERINFO *pUI)
{
	map<string, UINT64>::iterator it=m_map_userinvite.find(pUI->ID);
	if (it==m_map_userinvite.end())
		return;

	m_map_userinvite.erase(it);

}

void CInviteListWidget::ClearCheckedUser()
{
	m_map_userinvite.clear();
}


void CInviteListWidget::SetDataList(CDataUserList *pDataUserList)
{
	m_pDataUserList = pDataUserList;
	OnResetDataList();

}

void CInviteListWidget::ClearDataList()
{
	m_pDataUserList=NULL;

	for(int i=0; i< (int)m_vec_Units.size(); i++)
	{
		CInviteUnitWidget *pUserUnit= (CInviteUnitWidget*) m_vec_Units[i];
		pUserUnit->SetUserInfo(NULL);
	}

	ClearCheckedUser();
}
void CInviteListWidget::SetFilter(CDataUserList::KEY_FILTER FilterKey)
{
	m_FilterKey = FilterKey;
}



//------------------------------------------------------------------------------------------------
CInviteTab::CInviteTab(void)
{

}

CInviteTab::~CInviteTab(void)
{
}


BOOL CInviteTab::CreateWidget( CWnd* pParent )
{

	m_pParent = pParent;

	CDataUserList *pDataUserList = GM().GetDataUserList();
	ASSERT(pDataUserList);
	if(!pDataUserList)
		return FALSE;


	//Create Tab Buttons
	//탭 버튼
	NMBASE::SKINGDI::CNMTextButton *pOneGameButton = new NMBASE::SKINGDI::CNMTextButton();
	pOneGameButton->Initialize(INVITE_BUTTON_ID_INVITETAB_ONEGAME, pParent, 0, 0, "Invitation", 1,4);		
	pOneGameButton->SetTextColor(RGB(140,140,140),RGB(107,107,107), RGB(107,107,107), RGB(107,107,107) );
	pOneGameButton->SetFont(&NMBASE::UTIL::GetMyFont(NMBASE::UTIL::FONT_11D_BOLD));


	/*
	//전체 게임 탭버튼 생성 : 2012-09-20 제거(어뷰징으로 인하여 타게임 유저 초대 금지), YS.Jeong
	NMBASE::SKINGDI::CNMTextButton *pAllGameButton = new NMBASE::SKINGDI::CNMTextButton();
	pAllGameButton->Initialize(INVITE_BUTTON_ID_INVITETAB_ALLGAME, pParent, 178, 0, "Invitation", 1,4);
	pAllGameButton->SetText("전체게임");
	pAllGameButton->SetTextColor(RGB(140,140,140),RGB(107,107,107), RGB(107,107,107), RGB(107,107,107) );
	pAllGameButton->SetFont(&NMBASE::UTIL::GetMyFont(NMBASE::UTIL::FONT_11D_BOLD));
	*/

	//방 리스트뷰
	CInviteListWidget *pInviteListWidget = new CInviteListWidget();
	pInviteListWidget->Initialize(INVITE_WIDGET_ID_INVITELIST_ONEGAME, pParent, NMBASE::SKINGDI::NMRESSM.GetSprite("ListInviteUnit"), 1,32, pDataUserList);	
	pInviteListWidget->SetUnitSize(10,1);
	pInviteListWidget->SetSpaceBetweenUnits(0);
	

	AddTab(INVITE_WIDGET_ID_INVITELIST_ONEGAME, pInviteListWidget, pOneGameButton);


	CInviteListWidget *pInviteListWidget2 = new CInviteListWidget();
	pInviteListWidget2->Initialize(INVITE_WIDGET_ID_INVITELIST_ALLGAME, pParent, NMBASE::SKINGDI::NMRESSM.GetSprite("ListInviteUnit"), 1,32, pDataUserList);
	pInviteListWidget2->SetUnitSize(10,1);
	pInviteListWidget2->SetSpaceBetweenUnits(0);	

	//AddTab(INVITE_WIDGET_ID_INVITELIST_ALLGAME, pInviteListWidget2, pAllGameButton);


	//default tab
	ChangeTab(INVITE_WIDGET_ID_INVITELIST_ONEGAME);

	return CNMWidgetComposite::CreateWidget( pParent );

}

BOOL CInviteTab::RefreshInviteList()
{

	RefreshButton();

	CInviteListWidget *pWidget = dynamic_cast<CInviteListWidget*>(GetWidget(GetCurTab()));
	if (pWidget)
	{
	
		pWidget->OnResetDataList();
	}
	


	return TRUE;
}

void CInviteTab::DrawWidget( NMBASE::GRAPHICGDI::CPage& Page, CDC& DC )
{

	NMBASE::GRAPHICGDI::xSprite *pBackInviteSpr = NMBASE::SKINGDI::NMRESSM.GetSprite( "Invitation" );
	if (pBackInviteSpr)
	{
		//배경
		Page.PutSprAuto(0,0,pBackInviteSpr, 0);  
	}

	//초대창 하단 그리기
	Page.DrawFillBox(0,Page.height-64, Page.width-1, Page.height-1, NMBASE::GRAPHICGDI::xSprite::RGB555(216,222,226));
	Page.DrawEmptyBox(0,Page.height-64, Page.width-1, Page.height-1, NMBASE::GRAPHICGDI::xSprite::RGB555(171,171,171));

	//
	
	
	



	DC.SetTextColor(RGB(107,107,107));
	DC.SelectObject(FONT_DEFAULT);
	CString strText;
	

	int x = 26;
	int y = 315;//307->315로 변경, 3번째줄 삭제처리


	//Line 1		
	strText.Format("* 보유머니가");	
	DC.TextOut(x, y, strText.GetString());

	strText = g_RI.MoneyRange.GetStrMoneyRange().GetString();

	x=96;
	DC.SetTextColor(RGB(156, 16, 16));
	DC.TextOut(x, y, strText.GetString());

	SIZE textsize;
	GetTextExtentPoint32(DC.m_hDC, strText.GetString(), strText.GetLength(), &textsize);
	x+= textsize.cx+4;

	DC.SetTextColor(RGB(107,107,107));
	strText.Format("사용자만 표시됩니다.");
	DC.TextOut(x, y, strText.GetString());
	
	x = 26;	

	//Line2
	y+=20; //17 to 20 for the redesign line position
	strText.Format("* 게임초대 수신을 설정하지 않은 사용자는 초대하지 않습니다.");
	DC.TextOut(x,y,strText);
	


	/*
	//Line3
	y+=17;	
	strText = "* 전체게임: ";

	for(int idxGame=0; idxGame<MAX_GAME; idxGame++)
	{
		if (idxGame>0)
		{
			strText += ", ";
		}

		CGame *pGame = GM().GetGame((IDX_GAME)idxGame);
		if (pGame)
		{
			strText += pGame->GetGameName();
		}
	}

	DC.TextOut(x,y,strText);
	*/





	CNMTab::DrawWidget(Page, DC);
	
}

void CInviteTab::RefreshButton()
{

	NMBASE::SKINGDI::CNMTextButton *pButton = dynamic_cast<NMBASE::SKINGDI::CNMTextButton*>(GetWidget(INVITE_BUTTON_ID_INVITETAB_ONEGAME));
	ASSERT(pButton);
	if (pButton)
		pButton->SetText(GM().GetCurrentGameName());


}

int CInviteTab::GetCurSelIndex()
{
	if (GetCurTab() == INVITE_WIDGET_ID_INVITELIST_ONEGAME)
		return 0;
	else if (GetCurTab() == INVITE_WIDGET_ID_INVITELIST_ALLGAME)
		return 1;

	return -1;
}


map<string, UINT64> *CInviteTab::GetCheckedUser()
{
	CInviteListWidget *pWidget = dynamic_cast<CInviteListWidget*>(GetWidget(GetCurTab()));
	if (pWidget)
	{
		return pWidget->GetCheckedUser();
	}

	return NULL;
}


void CInviteTab::ClearCheckedUser()
{
	for (int i =0; i < (int)m_vec_TabInfo.size(); i++)
	{
		CInviteListWidget *pWidget = dynamic_cast<CInviteListWidget*> (m_vec_TabInfo[i].pViewWidget);
		if (pWidget)
		{
			pWidget->ClearCheckedUser();
		}
	}
}