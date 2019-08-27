// ChatWnd_PlayResultInfo.cpp: implementation of the CChatWnd_PlayResultInfo class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "ChatWnd_PlayResultInfo.h"


#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

#define MAX_PAGE 20

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CChatWnd_PlayResultInfo::CChatWnd_PlayResultInfo()
{
	m_bShow = FALSE;
	m_deque_PlayResultInfo.clear();
}

CChatWnd_PlayResultInfo::~CChatWnd_PlayResultInfo()
{

}

void CChatWnd_PlayResultInfo::Init(int x, int y)
{	
	//실제 그려지는 시작 위치
	m_OffSetpt.x = x;
	m_OffSetpt.y = y;
	Clear();
	InitButton();
}

void CChatWnd_PlayResultInfo::ReSetWindowPos(int x, int y)
{
	m_OffSetpt.x += x;
	m_OffSetpt.y += y;	
	InitButton();
}

void CChatWnd_PlayResultInfo::InitButton()
{
	//스크롤 업 버튼
	m_ScrollUpBtn.Init(    g_pGameView, &g_pGameView->Page, 270, 499, &GLOBAL_SPR[ spr::GSPR_GAME_CHATBOX ], 8, PLAYRESULTINFO_UP_BTN,3);	
	//스크롤 다운 버튼
	m_ScrollDownBtn.Init(  g_pGameView, &g_pGameView->Page, 270, 594, &GLOBAL_SPR[ spr::GSPR_GAME_CHATBOX ], 9, PLAYRESULTINFO_DOWN_BTN,3);
}


void CChatWnd_PlayResultInfo::EnterRoom()
{
	NMBASE::GRAPHICGDI::xSprite *pSpr = NULL;
	pSpr = &GLOBAL_SPR[ spr::GSPR_GAME_CHATBOX ];

	if( pSpr == NULL )return;
	if( pSpr->GetTotalSpr() == 0 )return;
	
	m_ScrollUpBtn.SetSprite(pSpr,8,3);
	//스크롤 다운 버튼
	m_ScrollDownBtn.SetSprite(pSpr,9,3);
}

void CChatWnd_PlayResultInfo::Clear()
{
	nCurTotalPageNum = 0;
	nCurViewPageNum = 0;
	m_deque_PlayResultInfo.clear();
	m_bShow = FALSE;
}

void CChatWnd_PlayResultInfo::InPut(PLAYRESULTINFO_DATA data)
{		
	//맥스가 정해져있을때
	if( m_deque_PlayResultInfo.size() >= MAX_PAGE ){
		m_deque_PlayResultInfo.pop_front();		

		deque<PLAYRESULTINFO_DATA>::iterator iter;
		for(iter = m_deque_PlayResultInfo.begin(); iter!=m_deque_PlayResultInfo.end(); ++iter){
			int index = (iter)->nPageIndex;
			index  -= 1;
			(iter)->nPageIndex = index;
			
		}		
		data.nPageIndex = nCurTotalPageNum;	
		nCurViewPageNum = nCurTotalPageNum;
	}
	else 
	{			
		nCurTotalPageNum++;
		data.nPageIndex = nCurTotalPageNum;
		nCurViewPageNum = nCurTotalPageNum;
	}

	m_deque_PlayResultInfo.push_back(data);

	int DrawIndex[5]={-1,};
	GetDrawIndex(DrawIndex);
}

PLAYRESULTINFO_DATA* CChatWnd_PlayResultInfo::GetHistoryData(int index )
{
	int nsize = m_deque_PlayResultInfo.size();
	if( index > nsize || index < 0 )return NULL;
	
	deque<PLAYRESULTINFO_DATA>::iterator iter;
	for(iter = m_deque_PlayResultInfo.begin(); iter!=m_deque_PlayResultInfo.end(); ++iter){
		
		if( (*iter).nPageIndex == index ) 
		{		
			return &(*iter);
		}
	}
	return NULL;
}

//bUpDown == 0 up bUpDown == 1 down
void CChatWnd_PlayResultInfo::OnScrollButtonUpDown(BOOL bUpDown )
{	
	if( bUpDown != TRUE ){
		nCurViewPageNum = nCurViewPageNum-1 <= 1 ? 1 : nCurViewPageNum - 1;			
	}
	else{		
		nCurViewPageNum = nCurViewPageNum + 1 >= nCurTotalPageNum ? nCurTotalPageNum : nCurViewPageNum + 1;
	}
}

void CChatWnd_PlayResultInfo::OnLButtonDown(int x, int y)
{
	if( !IsShow() )return;
	//	스크롤 업 버튼
	if( m_ScrollUpBtn.OnLButtonDown( x, y ) )
	{
		OnScrollButtonUpDown(0);		
	}
	
	//	스크롤 다운 버튼
	if( m_ScrollDownBtn.OnLButtonDown( x, y ) )
	{	
		OnScrollButtonUpDown(1);	
	}	
}

void CChatWnd_PlayResultInfo::OnLButtonUp(int x, int y)
{
	if( !IsShow() )return;
	//	스크롤 업 버튼
	m_ScrollUpBtn.OnLButtonUp( x, y );
	//	스크롤 다운 버튼
	m_ScrollDownBtn.OnLButtonUp( x, y );	
}

void CChatWnd_PlayResultInfo::OnMouseMove(int x, int y)
{
	if( !IsShow() )return;
	//	스크롤 업 버튼
	m_ScrollUpBtn.OnMouseMove( x, y );
	//	스크롤 다운 버튼
	m_ScrollDownBtn.OnMouseMove( x, y );	
}


void CChatWnd_PlayResultInfo::Draw(CDC *pdc , NMBASE::GRAPHICGDI::CPage *page)
{		
	if( !IsShow() )return;
	if( page == NULL )return;

	page->PutSprAuto(8,499,&GLOBAL_SPR[ spr::GSPR_GAME_CHATBOX ] , 37);
	
	CFont *oldf = pdc->SelectObject( &NMBASE::UTIL::GetMyFont( NMBASE::UTIL::FONT_12_BOLD));

	CRect wrt,idrt,jrt,mrt,wincasert;

	
	wrt.SetRect(0,0,12,12);
	wrt.OffsetRect(17, 510);

	idrt.SetRect(0,0,132,12);
	idrt.OffsetRect(42, 503);
	
	jrt.SetRect(0,0,132,12);
	jrt.OffsetRect(42, 516);

	mrt.SetRect(0,0,91,12);
	mrt.OffsetRect(178,513);

	wincasert.SetRect(0,0,36,15);
	wincasert.OffsetRect(141,503);

	m_ScrollUpBtn.Draw( pdc );			//	스크롤 업 버튼
	m_ScrollDownBtn.Draw( pdc );			//	스크롤 다운 버튼	
	
	PLAYRESULTINFO_DATA *PlayResultInfo = NULL;	
	int index = nCurViewPageNum;
	PlayResultInfo = GetHistoryData(index);

	if( PlayResultInfo == NULL )return;	

	COLORREF color = pdc->GetTextColor();

	BOOL bGoryRatioKind = FALSE;
	INT64 nGoryMoney = 0;


	for( int i = 0 ; i < 5 ; i ++)
	{	

		int nindex = PlayResultInfo->nDrawIndex[i];
		if( nindex == -1 )continue;

		PLAYRESULTINFO *pData =	&PlayResultInfo->Data[nindex];
		
		if ( pData->strValueName.length() == 0 ) continue;
		if ( pData->strID.length() == 0 )continue;
		
		if( strcmp( pData->strID.c_str(), GM().GetMyInfo()->UI.ID ) == 0 ){
			page->PutSprAuto(8,496 + ((i) * 35) + 4,&GLOBAL_SPR[ spr::GSPR_GAME_CHATBOX ] , 39);
		}
		
		CString str;
		// 승 			
		if (pData->nWinType == 1){

			float fgoryratio_1 = pData->fGoryRatio * 100;

			//if( pData->PremMenuNo == CODE_NEW_FAMILY_VIP || pData->PremMenuNo == CODE_NEW_FAMILY_VVIP )
			if ( pData->fGoryRatio < 0.05f ) //< CYS 100802 >
			{
				bGoryRatioKind = TRUE;
			}			
			nGoryMoney = pData->nGoryMoney;
			pdc->SetTextColor(RGB(139,165,166));
			str.Format("승");
		}
		else{
			pdc->SetTextColor(RGB(96,50,54));
			str.Format("패");		
		}
#ifdef _DEBUG
		//g_pGameView->Page.DrawFillBoxGB(wrt.left,wrt.top,wrt.right,wrt.bottom,RGBmix( 0, 2, 5 ),3);
#endif
		
		pdc->DrawText(str, &wrt, DT_CENTER | DT_WORDBREAK);
		
		//하이,로우,스윙
		if( pData->nWinCase > 0 ){
			switch(pData->nWinCase) {
			case 1:
				str="하이";
				break;
			case 2:
				str="로우";
				break;
			case 3:
				str="스윙";
				break;
			}
#ifdef _DEBUG
			//g_pGameView->Page.DrawFillBoxGB(wincasert.left,wincasert.top,wincasert.right,wincasert.bottom,RGBmix( 0, 2, 5 ),3);
#endif
			pdc->DrawText(str, &wincasert, DT_CENTER | DT_WORDBREAK);			
		}				
		
#ifdef _DEBUG
		//g_pGameView->Page.DrawFillBoxGB(idrt.left,idrt.top,idrt.right,idrt.bottom,RGBmix( 0, 2, 5 ),3);
#endif
		pdc->DrawText(pData->strID.c_str(), &idrt, DT_LEFT | DT_WORDBREAK);		
		
#ifdef _DEBUG
		//g_pGameView->Page.DrawFillBoxGB(jrt.left,jrt.top,jrt.right,jrt.bottom,RGBmix( 0, 2, 5 ),3);
#endif
		pdc->DrawText( pData->strValueName.c_str(), &jrt, DT_RIGHT | DT_WORDBREAK);
				
		
		// 금액
		if( pData->llResultMoney > 0 ){
			pdc->SetTextColor(RGB(139,165,166));
			str = "+";
			str += (CString)NMBASE::UTIL::g_HistoryMoneyMarkEx(pData->llResultMoney);
			CRect rt(mrt);
			rt.OffsetRect(0,-10);
			
#ifdef _DEBUG
			//g_pGameView->Page.DrawFillBoxGB(rt.left,rt.top,rt.right,rt.bottom,RGBmix( 0, 2, 5 ),3);
#endif
			pdc->DrawText(str, &rt, DT_RIGHT | DT_WORDBREAK);		
			
			CRect rt1(mrt);
			rt1.OffsetRect(0,6);
			
			CFont *oldf1 = pdc->SelectObject( &NMBASE::UTIL::GetMyFont( NMBASE::UTIL::FONT_11_BOLD));
		//	if( g_GetGroupGrade(g_NowGroupNo) != GROUP_GRADE_FREE )
			{
				float fgoryratio = pData->fGoryRatio * 100;			
				pdc->SetTextColor(RGB(81,80,85));
				//str.Format("%s딜러비%1.1f%%",g_GetPremName( pData->PremMenuNo,FALSE ),fgoryratio);
				str.Format("-%s",NMBASE::UTIL::g_MakeShortMoneyStrEx( pData->nGoryMoney));
#ifdef _DEBUG
				//g_pGameView->Page.DrawFillBoxGB(rt1.left,rt1.top,rt1.right,rt1.bottom,RGBmix( 0, 2, 5 ),3);
#endif						
				pdc->DrawText(str, &rt1, DT_RIGHT | DT_WORDBREAK);
			}
// 			else
// 			{
// 				float fgoryratio = pData->fGoryRatio * 100;			
// 				pdc->SetTextColor(RGB(81,80,85));
// 				//str.Format("딜러비%1.1f%%",fgoryratio);				
// 				str.Format("딜러비:%s",NMBASE::UTIL::g_MakeShortMoneyStrEx( pData->nGoryMoney));				
// 				pdc->DrawText(str, &rt1, DT_RIGHT | DT_WORDBREAK);
// 			}
// 			
			
			pdc->SelectObject(oldf1);
		}
		else{
			str = (CString)NMBASE::UTIL::g_HistoryMoneyMarkEx(pData->llResultMoney);
			pdc->DrawText(str, &mrt, DT_RIGHT | DT_WORDBREAK);	
		}

		wrt.OffsetRect(0,35);
		idrt.OffsetRect(0,35);
		jrt.OffsetRect(0,35);
		mrt.OffsetRect(0,34);
		wincasert.OffsetRect(0,35);
	}


	CString str3;
	CRect rect2;
#ifdef _DEBUG
	str3.Format("%d",nCurViewPageNum);
	rect2.SetRect(0,0,112,12);
	rect2.OffsetRect(154,504);
	pdc->DrawText(str3, &rect2, DT_LEFT | DT_WORDBREAK);
#endif
	
	pdc->SetTextColor(RGB(81,80,85));

	//	if( g_NowGroupClass != IDX_GROUPCLASS_BASIC ){
	
	//if( g_GetGroupGrade(g_NowGroupNo) != GROUP_GRADE_FREE ) //< CYS 100802 >
	{
		if( bGoryRatioKind == FALSE ){
		//	str3.Format("딜러비 : %s",g_MakeShortMoneyStr(nGoryMoney) );
			str3="딜러비가 적립되었습니다.";
		}
		else{
		//	str3.Format("딜러비 : %s (할인 적용 중)",g_MakeShortMoneyStr(nGoryMoney) );
			str3="딜러비가 할인되어 적립되었습니다.";
		}
		rect2.SetRect(0,0,205,15);
		rect2.OffsetRect(60,678);
		pdc->DrawText(str3, &rect2, DT_RIGHT | DT_WORDBREAK);
	}

	pdc->SelectObject(oldf);
	pdc->SetTextColor(color);
}


//잃은 금액이 큰 사람을 상위로 보내기 위해
void CChatWnd_PlayResultInfo::GetDrawIndex(int drawindex[] )
{
	PLAYRESULTINFO_DATA *PlayResultInfo = NULL;	
	int index = nCurViewPageNum;
	PlayResultInfo = GetHistoryData(index);
	if( PlayResultInfo == NULL )return;

	//윈 효과 
	const int FOLD = 2;
	const int WIN  = 1;
	const int LOSE = 0;
	
	const int FOLD_CASE	= 0;
	const int HI	= 1;
	const int LOW	= 2;
	const int SWING = 3;
	
	int i=0;
	int j=0;
	int drawcnt = 0;	
	
	int dindex[ 5 ] = { 0, 1, 2, 3, 4 };	
		
	
	for( i = 0; i < 5; i++) //오름차순.
	{
		if ( PlayResultInfo->Data[i].nWinType == 1 )continue;
		if ( PlayResultInfo->Data[i].strID.length() == 0 )continue;
		
		for(int j = i; j < 5; j++)
		{
			if ( PlayResultInfo->Data[j].nWinType == 1 )continue;
			if ( PlayResultInfo->Data[j].strID.length() == 0 )continue;
			// A
			if(PlayResultInfo->Data[i].llResultMoney > PlayResultInfo->Data[j].llResultMoney)
			{
				PLAYRESULTINFO temp;
				ZeroMemory(&temp,sizeof(PLAYRESULTINFO));
				
				memcpy( &temp , &PlayResultInfo->Data[i],sizeof(PLAYRESULTINFO));
				memcpy( &PlayResultInfo->Data[i] , &PlayResultInfo->Data[j],sizeof(PLAYRESULTINFO));
				memcpy( &PlayResultInfo->Data[j] , &temp,sizeof(PLAYRESULTINFO));
			}				
		}
	}


	for( i = 0 ; i < 5 ; i++){
		drawindex[i] = dindex[i];
		PlayResultInfo->nDrawIndex[i] = dindex[i];
	}		
}