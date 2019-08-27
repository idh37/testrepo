#include "StdAfx.h"
#include "GoldMileagePopup.h"
#include "goldmileagepopupid.h"

CGoldMileagePopup::CGoldMileagePopup(CWnd* pParent/* = NULL*/) : LSY::CLSYLibDialogBase(pParent)
{
}

CGoldMileagePopup::~CGoldMileagePopup(void)
{
}

INT_PTR CGoldMileagePopup::DoModal(CWnd *pParent)
{
	m_pParentWnd = pParent;
	return CLSYLibDialogBase::DoModal(GetObjectMan(), ID_GOLDMILEAGEPOPUP);
}

bool CGoldMileagePopup::OnCreatedProject(void)
{
	// LSY Control 연결
	InitLSYControl();
	// 초기 데이터 세팅
	InitData(m_TempGold);
	
	return TRUE;
}

/// <summary>
/// 버튼 클릭 이벤트. 대부분 서버에 요청 보내는 기능.
/// <summary>
/// <param name=""></param>
/// <returns></returns>
LRESULT CGoldMileagePopup::OnClickButton(LSY::CMessage *pMsg)
{
	LSY::CObjectMessage *msg = (LSY::CObjectMessage *)pMsg;
	switch(msg->GetObject()->GetID())
	{
	case ID_GOLDMILEAGEPOPUP_BTN_CLOSE: // 닫기 버튼
		CloseDlg();
		break;
	case ID_GOLDMILEAGEPOPUP_BTN_OK: // 확인 버튼
		CloseDlg();
		break;
	}
	
	return TRUE;
}

/// <summary>
/// LSY Control 연결
/// <summary>
/// <param name=""></param>
/// <returns></returns>
void CGoldMileagePopup::InitLSYControl()
{
	// 닫기 버튼
	m_pBtnClose = (LSY::CButton *)GetObject(ID_GOLDMILEAGEPOPUP_BTN_CLOSE);
	m_pBtnClose->AddHandler(LSY::EM_O_MOUSELCLICK, LSY::Fnt(this, &CGoldMileagePopup::OnClickButton));
	// 확인 버튼
	m_pBtnOk = (LSY::CButton *)GetObject(ID_GOLDMILEAGEPOPUP_BTN_OK);
	m_pBtnOk->AddHandler(LSY::EM_O_MOUSELCLICK, LSY::Fnt(this, &CGoldMileagePopup::OnClickButton));
	
	// 골드 텍스트
	m_pTxtGoldAmount = (LSY::CText *)GetObject(ID_GOLDMILEAGEPOPUP_TXT_GOLDAMOUNT);
}

/// <summary>
/// 데이터 세팅
/// <summary>
/// <param name=""></param>
/// <returns></returns>
void CGoldMileagePopup::InitData(INT64 nTempGold)
{
	// 골드를 0000만0000골드 형식으로 표기
	CString strOutput;
	if (nTempGold >= 10000)
	{
		INT64 nTempGold1 = nTempGold / 10000;
		int nTempGold2 = nTempGold % 10000;
		strOutput.Format("%I64d만%04d%s", nTempGold1, nTempGold2, strChip_Name[(int)ROOM_CHIPKIND_GOLD]);
	}
	else
	{
		strOutput.Format("%I64d%s", nTempGold, strChip_Name[(int)ROOM_CHIPKIND_GOLD]);
	}

	m_pTxtGoldAmount->SetText((LPCTSTR)strOutput);
}

void CGoldMileagePopup::CloseDlg(void)
{
	OnCancel();
}

/// <summary>
/// 서버에서 보내는 패킷(응답) 처리
/// <summary>
/// <param name=""></param>
/// <returns></returns>
void CGoldMileagePopup::HandleResponse(int Signal, int TotSize, char *lpData)
{
	switch(Signal)
	{
	case SV_GOLD_TEMPMONEY_REAL_UPDATE:
		{
			CSV_GOLD_TEMPMONEY_REAL_UPDATE msg;
			msg.Get(lpData, TotSize);
			
			m_TempGold = *msg.m_TempGold; //업데이트되는 임시(적립된) 골드 금액이다.
			m_RealGold = *msg.m_RealGold; //합산되어 실제 보유하게된 골드 금액이다.(이전 보유머니 포함 총액)
		}
		break;
	}
}
