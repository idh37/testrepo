// ProfileDlg.cpp : implementation file
//
#include "stdafx.h"
#include "RestrictionOverPopup.h"
#include "GlobalBase.h"
#include "restrictionoverpopupid.h"


CRestrictionOverPopup::CRestrictionOverPopup(CWnd* pParent /*=NULL*/)
	: LSY::CLSYLibDialogBase(pParent)
{
}

BEGIN_MESSAGE_MAP(CRestrictionOverPopup, LSY::CLSYLibDialogBase)
	ON_COMMAND(IDCANCEL, OnCancel)
END_MESSAGE_MAP()

bool CRestrictionOverPopup::OnCreatedProject(void)
{
	LSY::CObjectProject *pProject = m_pObjectMan->GetProjectFromID(ID_RESTRICTIONOVERPOPUP);
	m_pWndBack = (LSY::CWindows *)pProject->GetMainObject();
#if defined(RESTRICTION_LEVEL)
	m_pWndBack->SetIndex(0);
#else
	//m_pWndBack->SetIndex(62);
	m_pWndBack->SetIndex(0);
#endif

	LSY::CButton *pButton = (LSY::CButton *)GetObject(ID_RESTRICTIONOVERPOPUP_BTN_OK);
	pButton->AddHandler(LSY::EM_O_MOUSELCLICK, LSY::Fnt(this, &CRestrictionOverPopup::OnClickButton));

	pButton = (LSY::CButton *)GetObject(ID_RESTRICTIONOVERPOPUP_BTN_CLOSE);
	pButton->AddHandler(LSY::EM_O_MOUSELCLICK, LSY::Fnt(this, &CRestrictionOverPopup::OnClickButton));

	m_pImgLimitMoneyover = (LSY::CImage *)GetObject(ID_RESTRICTIONOVERPOPUP_IMG_LIMITMONEYOVER);
	//m_pImgLimitTimeover = (LSY::CImage *)GetObject(ID_RESTRICTIONOVERPOPUP_IMG_LIMITTIMEOVER);
	m_pImgToday_Tomorrow = (LSY::CImage *)GetObject(ID_RESTRICTIONOVERPOPUP_IMG_TODAY_TOMORROW);
	if(0!=GM().GetBlockTime())
	{
		m_pImgNumMonth = (LSY::CImageNumber *)GetObject(ID_RESTRICTIONOVERPOPUP_IMGNUM_MONTH);
		m_pImgNumDay = (LSY::CImageNumber *)GetObject(ID_RESTRICTIONOVERPOPUP_IMGNUM_DAY);
		m_pImgNumHour = (LSY::CImageNumber *)GetObject(ID_RESTRICTIONOVERPOPUP_IMGNUM_HOUR);
		m_pImgNumMin = (LSY::CImageNumber *)GetObject(ID_RESTRICTIONOVERPOPUP_IMGNUM_MIN);
		m_pImgNumMin0 = (LSY::CImageNumber *)GetObject(ID_RESTRICTIONOVERPOPUP_IMGNUM_MIN0);
	}
	else
	{
		m_pImgNumMonth = (LSY::CImageNumber *)GetObject(ID_RESTRICTIONOVERPOPUP_IMGNUM_MONTHT);
		m_pImgNumDay = (LSY::CImageNumber *)GetObject(ID_RESTRICTIONOVERPOPUP_IMGNUM_DAYT);
		m_pImgNumHour = (LSY::CImageNumber *)GetObject(ID_RESTRICTIONOVERPOPUP_IMGNUM_HOURT);
		m_pImgNumMin = (LSY::CImageNumber *)GetObject(ID_RESTRICTIONOVERPOPUP_IMGNUM_MINT);
		m_pImgNumMin0 = (LSY::CImageNumber *)GetObject(ID_RESTRICTIONOVERPOPUP_IMGNUM_MIN0T);
	}
	
	return true;
}


BOOL CRestrictionOverPopup::OnInitDialog() 
{
	CLSYLibDialogBase::OnInitDialog();
	DAILYLOSS_STATE eState = GM().GetLossMoneyState() ;
	time_t	blockTime = GM().GetBlockTime() ;

	if(0!=blockTime){
		m_pImgLimitMoneyover->SetShow(true) ;
		m_pImgLimitMoneyover->SetIndex(27) ;


		// 시간값을 얻어온다.
		COleDateTime time = COleDateTime::GetCurrentTime() - g_CSTimeGap;

		SYSTEMTIME st; 
		time.GetAsSystemTime(st);

		COleDateTime CurTime(st) ;
		COleDateTime EndTime(blockTime) ;

		if(CurTime.GetDay() != EndTime.GetDay())
		{
			m_pImgToday_Tomorrow->SetIndex(28) ; // 날이 다르면 과거는 나올 수 없으므로 내일.
		}else
		{
			m_pImgToday_Tomorrow->SetIndex(27) ; // 오늘
		}

		m_pImgNumMonth->SetNumber(EndTime.GetMonth());
		m_pImgNumDay->SetNumber(EndTime.GetDay());

		m_pImgNumHour->SetNumber(EndTime.GetHour());		
		m_pImgNumMin->SetNumber(EndTime.GetMinute());

		if(10 <= EndTime.GetMinute()){
			m_pImgNumMin0->SetShow(false) ;
		}else
		{
			m_pImgNumMin0->SetShow(true) ;
		}
	}
	else{
		COleDateTime EndTime(GM().GetRewardOverTime()) ;
		//m_pImgLimitTimeover->SetShow(true) ;

		m_pImgNumMonth->SetNumber(EndTime.GetMonth());
		m_pImgNumDay->SetNumber(EndTime.GetDay());

		m_pImgNumHour->SetNumber(EndTime.GetHour());		
		m_pImgNumMin->SetNumber(EndTime.GetMinute());

		if(10 <= EndTime.GetMinute()){
			m_pImgNumMin0->SetShow(false) ;
		}else
		{
			m_pImgNumMin0->SetShow(true) ;
		}
	}

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}

LRESULT CRestrictionOverPopup::OnClickButton(LSY::CMessage *pMsg)
{
	LSY::CMO_MouseLClick *msg = (LSY::CMO_MouseLClick *)pMsg;
	switch(msg->GetObject()->GetID())
	{
	case ID_RESTRICTIONOVERPOPUP_BTN_OK:
	case ID_RESTRICTIONOVERPOPUP_BTN_CLOSE:
		{
			OnCancel() ;
		}
		break;
	}

	return TRUE;
}


INT_PTR CRestrictionOverPopup::DoModal(CWnd *pParent)
{
	m_pParentWnd = pParent;

	return CLSYLibDialogBase::DoModal(GetObjectMan(), ID_RESTRICTIONOVERPOPUP);
}

void CRestrictionOverPopup::OnCancel()
{
	EndDialog(IDCANCEL);
}
