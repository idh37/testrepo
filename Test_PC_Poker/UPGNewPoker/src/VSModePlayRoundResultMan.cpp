// VSModePlayRoundResultMan.cpp: implementation of the CVSModePlayRoundResultMan class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "VSModePlayRoundResultMan.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CVSModePlayRoundResultMan::CVSModePlayRoundResultMan()
{

}

CVSModePlayRoundResultMan::~CVSModePlayRoundResultMan()
{

}

void CVSModePlayRoundResultMan::Init()
{
	m_HideBtn.Init( g_pGameView , &g_pGameView->Page ,185 ,383,&VSModeRoundPlayResultInfoBtnSpr,0,IDM_VSHIDEBTN);	
	Clear();	
}

void CVSModePlayRoundResultMan::Clear()
{
	nStartTime = 0;
	nEndTime = 0;
	nDelay = 0;
	nWinNum = 0;
	nLooseNum = 0;
	nRoundNum = 0;
	nDrawMode = -1;//0 나오는 효과 1 들어가는 효과
	bShowRound = FALSE;
	m_HideBtn.SetSprite(&VSModeRoundPlayResultInfoBtnSpr , 0 );
	m_HideBtn.SetButtonPos(185,383);
	m_HideBtn.Show(FALSE);
}

void CVSModePlayRoundResultMan::SetRoundInfo( int win ,int loose ,int round)
{
	nStartTime = timeGetTime();
	pSpr = &VSModeRoundPlayResultInfoSpr;
	if( pSpr == NULL ){		
		return ;
	}		
	nDelay = 0;
	nWinNum = win;
	if( nWinNum >= 99999 )nWinNum = 99999 ;
	nLooseNum = loose;	
	if( nLooseNum >= 99999 )nLooseNum = 99999 ;

	nRoundNum = round ;
	if( nRoundNum >= 99999 )nRoundNum = 99999 ;

	nEndTime = pSpr->GetMScenePlayTime(0);
	nDrawMode = 0;	
}

void CVSModePlayRoundResultMan::VSModePlayRoundResult_Draw(CDC *pDC)
{
	if( pSpr == NULL )return;


	

	if(nStartTime > 0)
	{			
		//하이드 된상태면 그리지 마라
		m_HideBtn.Draw(pDC);
		if( bShowRound == TRUE )return;

		DWORD drawt = timeGetTime() - nStartTime;	
		
		if( drawt >= nDelay && nDelay > 0 ){
			nDelay = 0 ;
			nStartTime = timeGetTime();	
		}
		
		if(drawt >= nEndTime && nDelay == 0 && nDrawMode == 0 ) {
			nDrawMode = 1;
			nStartTime = timeGetTime();
			nEndTime = pSpr->GetScenePlayTime(0);		
		}
		NMBASE::GRAPHICGDI::CPage* pPage = &g_pGameView->Page;
		
		//	DWORD drawt = timeGetTime() - nStartTime;
		
		if( nDrawMode == 0 ){			
			pPage->DrawMultiScene(0,0,pSpr,0,drawt,FALSE);
			
			if( drawt >= 120 ){
				CRect rt(110,428,165,443);
				
				CString str;
				//라운드 정보
				str.Format("%d",nRoundNum);
				int len = str.GetLength();			
				int xp = (int)((rt.Width()/2) - (len * 5.5 ));
				int i=0;
				for(i=0; i<len; i++)
				{			
					char num[4] = {0,};
					num[0] = str.GetAt(i);
					int index = atoi(num);
					index += 2;
					pPage->PutSprAuto(rt.left + xp ,rt.top,pSpr,index);
					xp += 11;				
				}
				
				//승 정보
				rt.SetRect(15,472,101,497);
				str = "";
				str.Format("%d",nWinNum);
				len = str.GetLength();
				xp = (int)((rt.Width()/2) - (len * 8.5 ));
				for(i=0; i<len; i++)
				{			
					char num[4] = {0,};
					num[0] = str.GetAt(i);
					int index = atoi(num);
					index += 12;				
					pPage->PutSprAuto(rt.left + xp ,rt.top,pSpr,index);
					xp += 17;				
				}
				
				//패 정보
				rt.SetRect(118,472,203,497);
				str = "";
				str.Format("%d",nLooseNum);
				len = str.GetLength();
				xp = (int)((rt.Width()/2) - (len * 8.5 ));
				for(i=0; i<len; i++)
				{			
					char num[4] = {0,};
					num[0] = str.GetAt(i);
					int index = atoi(num);
					index += 12;
					pPage->PutSprAuto(rt.left + xp ,rt.top,pSpr,index);
					xp += 17;				
				}

			}		
		}
		else if( nDrawMode == 1 ){	
			
			pPage->DrawMultiScene(0,0,pSpr,1,drawt,TRUE);

			CRect rt(110,428,165,443);

			CString str;
			//라운드 정보
			str.Format("%d",nRoundNum);
			int len = str.GetLength();			
			int xp = (int)((rt.Width()/2) - (len * 5.5 ));
			int i=0;
			for(i=0; i<len; i++)
			{			
				char num[4] = {0,};
				num[0] = str.GetAt(i);
				int index = atoi(num);
				index += 2;
				pPage->PutSprAuto(rt.left + xp ,rt.top,pSpr,index);
				xp += 11;				
			}

			//승 정보
			rt.SetRect(15,472,101,497);
			str = "";
			str.Format("%d",nWinNum);
			len = str.GetLength();
			xp = (int)((rt.Width()/2) - (len * 8.5 ));
			for(i=0; i<len; i++)
			{			
				char num[4] = {0,};
				num[0] = str.GetAt(i);
				int index = atoi(num);
				index += 12;				
				pPage->PutSprAuto(rt.left + xp ,rt.top,pSpr,index);
				xp += 17;				
			}

			//패 정보
			rt.SetRect(118,472,203,497);
			str = "";
			str.Format("%d",nLooseNum);
			len = str.GetLength();
			xp = (int)((rt.Width()/2) - (len * 8.5 ));
			for(i=0; i<len; i++)
			{			
				char num[4] = {0,};
				num[0] = str.GetAt(i);
				int index = atoi(num);
				index += 12;
				pPage->PutSprAuto(rt.left + xp ,rt.top,pSpr,index);
				xp += 17;				
			}
		}

		
		CFont *oldf = pDC->SelectObject(&g_pGameView->Font1);
		CRect irt(6,390,106,409);
		pDC->DrawText(g_pMyInfo_NP->UI.NickName,&irt,DT_LEFT | DT_WORDBREAK);
		irt.SetRect(111,407,211,422);
		if( strlen(GAME()->GetPlayerNP(0)->VS_ResultInfo.strVS_ID) > 0 ){
			pDC->DrawText(GAME()->GetPlayerNP(0)->VS_ResultInfo.strVS_ID,&irt,DT_LEFT | DT_WORDBREAK);
		}
		pDC->SelectObject(oldf);
	}
}

void CVSModePlayRoundResultMan::VSRoundResltWindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{

	short mxp, myp;

	switch(message)
	{
	case WM_MOUSEMOVE :
		{
			mxp = LOWORD(lParam);
			myp = HIWORD(lParam);
			m_HideBtn.OnMouseMove(mxp,myp);
		}
		return;

	case WM_LBUTTONDOWN :
		{	
			mxp = LOWORD(lParam);
			myp = HIWORD(lParam);
			m_HideBtn.OnLButtonDown(mxp,myp);	
			
			if( bShowRound == FALSE ){
				CRect rt;
				rt.SetRect(0,0,212,115);
				rt.OffsetRect(6,390);
				POINT pt;
				pt.x = mxp;
				pt.y = myp;
				if( rt.PtInRect(pt) ){
					ReSetHideButtonState();
				}			
			}
		}
		return;

	case WM_LBUTTONUP :
		{
			mxp = LOWORD(lParam);
			myp = HIWORD(lParam);
			m_HideBtn.OnLButtonUp(mxp,myp);			
		}
		return;	
	}
}

void CVSModePlayRoundResultMan::ReSetHideButtonState()
{
	bShowRound = !bShowRound;

	//하이드 된 상태
	if( bShowRound ){
		m_HideBtn.SetSprite(&VSModeRoundPlayResultInfoBtnSpr , 1 );
		m_HideBtn.SetButtonPos(2,483);
		m_HideBtn.Show(TRUE);
	}
	//활성화 된 상태
	else{
		m_HideBtn.SetSprite(&VSModeRoundPlayResultInfoBtnSpr , 0 );
		m_HideBtn.SetButtonPos(185,383);
		m_HideBtn.Show(FALSE);
		nStartTime = timeGetTime();
		nDrawMode = 0;
	}
}
