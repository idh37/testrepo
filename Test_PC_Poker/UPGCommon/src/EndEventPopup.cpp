// ProfileDlg.cpp : implementation file
//

#include "stdafx.h"
#include "EndEventPopup.h"
#include "GlobalBase.h"
#include "endeventpopupid.h"
#include "endeventpopup2id.h"
#include "endeventbannerid.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

CEndEventPopupDlg::CEndEventPopupDlg(CWnd* pParent /*=NULL*/)
	: LSY::CLSYLibDialogBase(pParent)
{
}

BEGIN_MESSAGE_MAP(CEndEventPopupDlg, LSY::CLSYLibDialogBase)
	ON_WM_MOVE()
END_MESSAGE_MAP()

bool CEndEventPopupDlg::OnCreatedProject(void)
{	
	// 규제안 체크용 종료팝업.
	if(true == GM().IsLimitOver() )
	//if(true)
	{
		LSY::CObjectProject *pProject = m_pObjectMan->GetProjectFromID(ID_ENDEVENTPOPUP2);
		m_pWndBack = (LSY::CWindows *)pProject->GetMainObject();

		LSY::CButton *pButton = (LSY::CButton *)GetObject(ID_ENDEVENTPOPUP2_BTN_GAMEOVER);
		pButton->AddHandler(LSY::EM_O_MOUSELCLICK, LSY::Fnt(this, &CEndEventPopupDlg::OnClickButton2));

		pButton = (LSY::CButton *)GetObject(ID_ENDEVENTPOPUP2_BTN_GOTOGAME);
		pButton->AddHandler(LSY::EM_O_MOUSELCLICK, LSY::Fnt(this, &CEndEventPopupDlg::OnClickButton2));

		pButton = (LSY::CButton *)GetObject(ID_ENDEVENTPOPUP2_BTN_CLOSE);
		pButton->AddHandler(LSY::EM_O_MOUSELCLICK, LSY::Fnt(this, &CEndEventPopupDlg::OnClickButton2));
		// 시간값을 얻어온다.
		COleDateTime time = COleDateTime::GetCurrentTime() - g_CSTimeGap;

		SYSTEMTIME st; 
		time.GetAsSystemTime(st);

		COleDateTime CurTime(st) ;
		COleDateTime EndTime(GM().GetBlockTime()) ;
	
		LSY::CImage *pImage = (LSY::CImage *)GetObject(ID_ENDEVENTPOPUP2_IMG_DATE);
		
		if(CurTime.GetDay() != EndTime.GetDay())
		{
			pImage->SetIndex(28); // 날이 다르면 과거는 나올 수 없으므로 내일.
		}else
		{
			pImage->SetIndex(27); // 오늘
		}

		LSY::CImageNumber *pImgNum = (LSY::CImageNumber *)GetObject(ID_ENDEVENTPOPUP2_IMG_MONTH);
		pImgNum->SetNumber(EndTime.GetMonth());
		pImgNum = (LSY::CImageNumber *)GetObject(ID_ENDEVENTPOPUP2_IMG_DAY);
		pImgNum->SetNumber(EndTime.GetDay());
		pImgNum = (LSY::CImageNumber *)GetObject(ID_ENDEVENTPOPUP2_IMG_HOUR);
		pImgNum->SetNumber(EndTime.GetHour());		
		pImgNum = (LSY::CImageNumber *)GetObject(ID_ENDEVENTPOPUP2_IMG_MINUTE01);
		pImgNum->SetNumber(EndTime.GetMinute());

		pImgNum = (LSY::CImageNumber *)GetObject(ID_ENDEVENTPOPUP2_IMG_MINUTE10);
		if(10 <= EndTime.GetMinute())
		{
			pImgNum->SetShow(false) ;
		}else
		{
			pImgNum->SetShow(true) ;
		}

	}
	else
	{
		LSY::CObjectProject *pProject = m_pObjectMan->GetProjectFromID(ID_ENDEVENTPOPUP);
		m_pWndBack = (LSY::CWindows *)pProject->GetMainObject();
		LSY::CButton *pButton = NULL;
		pButton = (LSY::CButton *)GetObject(ID_ENDEVENTPOPUP_BTN_GOTOEVENTPAGE);
		pButton->AddHandler(LSY::EM_O_MOUSELCLICK, LSY::Fnt(this, &CEndEventPopupDlg::OnClickButton));
		pButton->SetEnable(false);
		pButton->SetShow(false);

		pButton = (LSY::CButton *)GetObject(ID_ENDEVENTPOPUP_BTN_GAMEOVER);
		pButton->AddHandler(LSY::EM_O_MOUSELCLICK, LSY::Fnt(this, &CEndEventPopupDlg::OnClickButton));

		pButton = (LSY::CButton *)GetObject(ID_ENDEVENTPOPUP_BTN_GOTOGAME);
		pButton->AddHandler(LSY::EM_O_MOUSELCLICK, LSY::Fnt(this, &CEndEventPopupDlg::OnClickButton));
	}
	m_EndEventBannerDlg.Create(this);

	return true;
}

BOOL CEndEventPopupDlg::OnInitDialog() 
{
	CLSYLibDialogBase::OnInitDialog();
	// TODO: Add extra initialization here

	CRect rect, rect1;
	GetWindowRect(rect);
	m_EndEventBannerDlg.GetWindowRect(rect1);
	m_EndEventBannerDlg.MoveWindow(rect.right, rect.top, rect1.Width(), rect1.Height());

	if(true == GM().IsLimitOver() )
	{

		CString str;

		//LSY::CTextNumber* pTxtExplain = (LSY::CTextNumber*)GetObject(ID_ENDEVENTPOPUP2_TEXT);
		//str.Format("%s", NumberToOrientalString(_abs64(GM().GetLossMaxMoneyBase())));
		//pTxtExplain->SetText(str.operator LPCTSTR());


		//pTxtExplain = (LSY::CTextNumber*)GetObject(ID_ENDEVENTPOPUP2_TEXT_EXPLAIN1);
		//str.Format("손실머니가 %s을 초과", NumberToOrientalString(_abs64(GM().GetLossMaxMoneyBase())));
		//pTxtExplain->SetText(str.operator LPCTSTR());

	}
	else
	{
		// 2017.12 골드경기장 오픈 팝업으로 변경
		/*
		LSY::CImage *pImgPan = (LSY::CImage*)GetObject(ID_ENDEVENTPOPUP_IMAGE_MISSIONREMAIN);
		LSY::CImage *pImgCount = (LSY::CImage*)GetObject(ID_ENDEVENTPOPUP_IMAGE_MISSIONLCLEAR);
		LSY::CImage *pImgEnd = (LSY::CImage*)GetObject(ID_ENDEVENTPOPUP_IMAGE_MISSIONEND);
		if(0<=GM().GetPromotionData()->nCnts[1]){
			if((5<=GM().GetPromotionData()->nCnts[1])){
				GM().GetPromotionData()->nCnts[1]=0;
			}
			pImgPan->SetIndex(49+(5-GM().GetPromotionData()->nCnts[1]-1));
		}
		if(0<=GM().GetPromotionData()->nCnts[2]){
			if(20<GM().GetPromotionData()->nCnts[2])
			{
				pImgCount->SetIndex(59+GM().GetPromotionData()->nCnts[2]);
			}
			else
			{
				pImgCount->SetIndex(54+GM().GetPromotionData()->nCnts[2]);
			}
		}

		if(true==GM().GetPromotionData()->IsEnd()){
			pImgEnd->SetShow(true);
		}
		else{
			pImgEnd->SetShow(false);
		}
		*/
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

void CEndEventPopupDlg::SetMissionStep(LSY::CImage *pImg, int step)
{
	int missionIdx = pImg->GetIndex();
	int missionStep = GM().GetPromotionData()->nStep;
	pImg->SetShow(missionStep>=step);
	pImg->SetIndex(missionStep==step?missionIdx:GM().GetPromotionData()->nCnts[step]==1?missionIdx+8:missionIdx+4);
}

LRESULT CEndEventPopupDlg::OnClickButton(LSY::CMessage *pMsg)
{
	LSY::CMO_MouseLClick *msg = (LSY::CMO_MouseLClick *)pMsg;
	switch(msg->GetObject()->GetID())
	{
	case ID_ENDEVENTPOPUP_BTN_GOTOEVENTPAGE:
	//case ID_ENDEVENTPOPUP_BTN_GOTOEVENTPAGEEND:
		{
			/*NMBASE::UTIL::CShellMan sm;
			sm.OpenNewBrowser(getGameURL(IDX_GAMEURL_EVENTPAGE));*/

			// 새로운 방식
			//NMBASE::UTIL::OpenNetmarbleWebpage(g_MyUniqNo, (char*)(LPCTSTR)getGameURL(IDX_GAMEURL_EVENTPAGE));

			//::ShellExecute(NULL,  "open", (char*)(LPCTSTR)getGameURL(IDX_GAMEURL_EVENTPAGE), NULL, NULL, SW_SHOWNORMAL);
			//::ShellExecute(NULL,  "open", "http://game1.netmarble.net/event/2014/poker/01/newMember.asp", NULL, NULL, SW_SHOWNORMAL);

			NMBASE::UTIL::CShellMan ShellMan;
			//NMBASE::UTIL::BROWSEROPTION boption;
			//ZeroMemory(&boption, sizeof(boption));
			//boption.bAddressBar=FALSE;
			//boption.bMenuBar=FALSE;
			//boption.bResizable=FALSE;
			//boption.bStatusBar=FALSE;
			//boption.bToolBar=FALSE;
			//boption.nWidth=980+42;
			//boption.nHeight=690+56;
			//ShellMan.OpenNewBrowser("http://game1.netmarble.net/poker/round/guide/index.asp", &boption);

			//ShellMan.OpenNewBrowser("http://game1.netmarble.net/event/2015/poker/0903/");
			//ShellMan.OpenNewBrowser("http://game1.netmarble.net/event/2015/poker/1013/_html/");
			
			//ShellMan.OpenNewBrowser("http://game1.netmarble.net/event/2015/poker/12/index.asp");
			//ShellMan.OpenNewBrowser("http://promotion.netmarble.net/m/event/mpoker/26");
			//ShellMan.OpenNewBrowser("http://game1.netmarble.net/poker/round/guide/index.asp");
			//ShellMan.OpenNewBrowser("http://game1.netmarble.net/event/2016/poker/0719/_html/index.asp");
			ShellMan.OpenNewBrowser("http://game1.netmarble.net/event/2016/poker/1215/_html/index.asp");
			//ShellMan.OpenNewBrowser("http://game1.netmarble.net/event/2017/poker/0926/_html/");			

			OnCancel();
			m_EndEventBannerDlg.Close();

			ExitGameClient(false);
		}
		break;

	case ID_ENDEVENTPOPUP_BTN_GAMEOVER:
		{
			OnCancel();
			m_EndEventBannerDlg.Close();

			// 너무 많이 발생하여 강제 크래시 중단. 20190401
			/*
			TCHAR filePath1[MAX_PATH];
			wchar_t tempPath[MAX_PATH];
			GetTempPathW(MAX_PATH, tempPath);

			// 네트워크 불안정으로 파일 전송 실패시 tmp 파일 전송. 20181112
			wsprintf(filePath1, "%s_sprbd.tmp", NMBASE::UTIL::BugSplatWrapper::GetInstance().ConvUnicodeToMultiByte(tempPath));
			WIN32_FIND_DATA file;
			HANDLE search_handle=FindFirstFile(filePath1, &file);
			if(search_handle != (HANDLE)INVALID_HANDLE_VALUE)
			{
				*(volatile int *)0 = 0;
			}
			*/

			ExitGameClient(true);
		}
	case ID_ENDEVENTPOPUP_BTN_GOTOGAME:
		{
			OnCancel() ;
			m_EndEventBannerDlg.Close();
		}		
		break;		
	}

	return TRUE;
}

LRESULT CEndEventPopupDlg::OnClickButton2(LSY::CMessage *pMsg)
{
	LSY::CMO_MouseLClick *msg = (LSY::CMO_MouseLClick *)pMsg;
	switch(msg->GetObject()->GetID())
	{
	case ID_ENDEVENTPOPUP2_BTN_GAMEOVER:
		{
			OnCancel();
			m_EndEventBannerDlg.Close();

			ExitGameClient(true);
		}
		break;
	case ID_ENDEVENTPOPUP2_BTN_GOTOGAME:
		{
			OnCancel();
			m_EndEventBannerDlg.Close();
		}		
		break;

	case ID_ENDEVENTPOPUP2_BTN_CLOSE:
		{
			OnCancel();
			m_EndEventBannerDlg.Close();
		}		
		break;
	}

	return TRUE;
}

INT_PTR CEndEventPopupDlg::DoModal(CWnd *pParent)
{
	m_pParentWnd = pParent;

	//응답하라1995 종료팝업.
	/*if(true == GM().Get1995EventInfo().bIsEventTarget)
	{
		if (NMBASE::UTIL::GetCurSiteNo() == NMBASE::UTIL::SNO_NETMARBLE)
		{	
			return CLSYLibDialogBase::DoModal(GetObjectMan(), ID_ENDEVENTPOPUP2);
		}		
	}*/

	if(true == GM().IsLimitOver())
	{
		return CLSYLibDialogBase::DoModal(GetObjectMan(), ID_ENDEVENTPOPUP2);
	}
	return CLSYLibDialogBase::DoModal(GetObjectMan(), ID_ENDEVENTPOPUP);
}

void CEndEventPopupDlg::OnMove(int x, int y)
{
	CLSYLibDialogBase::OnMove(x, y);

	if( m_EndEventBannerDlg.m_hWnd != NULL )
	{
		CRect rect, rect1;
		GetWindowRect(rect);

		m_EndEventBannerDlg.GetWindowRect(rect1);
		m_EndEventBannerDlg.MoveWindow(rect.right, rect.top, rect1.Width(), rect1.Height());
	}

	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}



CEndEventBannerDlg::CEndEventBannerDlg(CWnd* pParent /*=NULL*/)
: LSY::CLSYLibDialogBase(pParent)
{
}

BEGIN_MESSAGE_MAP(CEndEventBannerDlg, LSY::CLSYLibDialogBase)
END_MESSAGE_MAP()

bool CEndEventBannerDlg::OnCreatedProject(void)
{
	LSY::CObjectProject *pProject = m_pObjectMan->GetProjectFromID(ID_ENDEVENTBANNER);
	m_pWndBack = (LSY::CWindows *)pProject->GetMainObject();

	LSY::CButton *pButton = (LSY::CButton *)GetObject(ID_ENDEVENTBANNER_BANNER0);
	if( pButton )
		pButton->AddHandler(LSY::EM_O_MOUSELCLICK, LSY::Fnt(this, &CEndEventBannerDlg::OnClickButton));

	return true;
}

BOOL CEndEventBannerDlg::OnInitDialog() 
{
	CLSYLibDialogBase::OnInitDialog();
	return TRUE;  
}

BOOL CEndEventBannerDlg::PreTranslateMessage(MSG* pMsg)
{
	return CWnd::PreTranslateMessage(pMsg);
}

INT_PTR CEndEventBannerDlg::Create(CWnd *pParent)
{
	m_pParentWnd = pParent;

	return CLSYLibDialogBase::Create(GetObjectMan(), ID_ENDEVENTBANNER, pParent);
}

void CEndEventBannerDlg::Close(void)
{
	OnCancel();
}

LRESULT CEndEventBannerDlg::OnClickButton(LSY::CMessage *pMsg)
{
	LSY::CMO_MouseLClick *msg = (LSY::CMO_MouseLClick*)pMsg;
	switch( msg->GetObject()->GetID() )
	{
	case ID_ENDEVENTBANNER_BANNER0:
		{
			NMBASE::UTIL::CShellMan ShellMan;
			ShellMan.OpenNewBrowser( "http://game1.netmarble.net/event/2017/poker/0523/_html/" );
		}
		break;
	}
	return TRUE;
}
