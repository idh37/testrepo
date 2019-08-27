#include "StdAfx.h"
#include "GoldSupportFundPopup.h"
#include "goldsupportfundPopupid.h"

CGoldSupportFundPopup::CGoldSupportFundPopup(CWnd* pParent/* = NULL*/) : LSY::CLSYLibDialogBase(pParent)
{
	m_Grade = -1;
}

CGoldSupportFundPopup::~CGoldSupportFundPopup(void)
{
}

INT_PTR CGoldSupportFundPopup::DoModal(CWnd *pParent)
{
	m_pParentWnd = pParent;
	return CLSYLibDialogBase::DoModal(GetObjectMan(), ID_GOLDSUPPORTFUNDPOPUP);
}

bool CGoldSupportFundPopup::OnCreatedProject(void)
{
	// LSY Control 연결
	InitLSYControl();
	// 데이터 세팅
	InitData(m_Grade);
	//InitData(2); // test
	
	return TRUE;
}

/// <summary>
/// 버튼 클릭 이벤트. 대부분 서버에 요청 보내는 기능.
/// <summary>
/// <param name=""></param>
/// <returns></returns>
LRESULT CGoldSupportFundPopup::OnClickButton(LSY::CMessage *pMsg)
{
	LSY::CObjectMessage *msg = (LSY::CObjectMessage *)pMsg;
	switch(msg->GetObject()->GetID())
	{
	case ID_GOLDSUPPORTFUNDPOPUP_BTN_OK: // 확인 버튼
		{
			CloseDlg();
		}
		break;
	}
	
	return TRUE;
}

/// <summary>
/// LSY Control 연결
/// <summary>
/// <param name=""></param>
/// <returns></returns>
void CGoldSupportFundPopup::InitLSYControl()
{
	// 팝업
	//m_pWndPopup = (LSY::CWindows *)GetObject(ID_GOLDSUPPORTFUNDPOPUP);
	LSY::CObjectProject *pProject = m_pObjectMan->GetProjectFromID(m_nID);
	m_pWndPopup = (LSY::CWindows *)pProject->GetMainObject();
	
	// 확인 버튼
	m_pBtnOk = (LSY::CButton *)GetObject(ID_GOLDSUPPORTFUNDPOPUP_BTN_OK);
	m_pBtnOk->AddHandler(LSY::EM_O_MOUSELCLICK, LSY::Fnt(this, &CGoldSupportFundPopup::OnClickButton));
	
	// 지원금 이미지
	m_pImgFund = (LSY::CImage *)GetObject(ID_GOLDSUPPORTFUNDPOPUP_IMG_FUND);
}

/// <summary>
/// 데이터 세팅
/// <summary>
/// <param name=""></param>
/// <returns></returns>
void CGoldSupportFundPopup::InitData(int nGrade)
{
	switch (nGrade)
	{
	// 미당첨
	case -1:
		{
			m_pWndPopup->SetIndex(1);
			m_pImgFund->SetShow(false);
		}
		break;
	// 1등
	case 1:
		{
			m_pWndPopup->SetIndex(0);
			m_pImgFund->SetIndex(8);
			m_pImgFund->SetShow(true);
		}
		break;
	// 2등
	case 2:
	case 3:
	case 4:
	case 5:
		{
			m_pWndPopup->SetIndex(0);
			m_pImgFund->SetIndex(7);
			m_pImgFund->SetShow(true);
		}
		break;
	// 3등
	case 6:
	case 7:
	case 8:
	case 9:
	case 10:
		{
			m_pWndPopup->SetIndex(0);
			m_pImgFund->SetIndex(6);
			m_pImgFund->SetShow(true);
		}
		break;
	// 값이 이상하게 왔을 시 미당첨 팝업 띄움(원래는 에러..)
	default:
		{
			m_pWndPopup->SetIndex(1);
			m_pImgFund->SetShow(false);
		}
		break;
	}
}

void CGoldSupportFundPopup::CloseDlg(void)
{
	OnCancel();
}

/// <summary>
/// 서버에서 보내는 패킷(응답) 처리
/// <summary>
/// <param name=""></param>
/// <returns></returns>
void CGoldSupportFundPopup::HandleResponse(int Signal, int TotSize, char *lpData) // 서버 응답 핸들링
{
	switch(Signal)
	{
	case SV_GOLD_HELP_REWARD:
		{
			CSV_GOLD_HELP_REWARD msg;
			msg.Get(lpData, TotSize);
			
			m_Grade = *msg.m_Grade; //순위.. 이 값이 -1이면 꽝이다...
			INT64 m_RewardGold = *msg.m_RewardGold; //보상금이다.
		}
		break;
	}
}
