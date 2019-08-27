#include "StdAfx.h"
#include "ChipAttendSubPopup.h"
#include "ChipAttendSubPopupid.h"

CChipAttendSubPopup::CChipAttendSubPopup(CWnd* pParent/* = NULL*/) : LSY::CLSYLibDialogBase(pParent)
{
	m_pImgBack = NULL; // 배경 이미지
	m_pTxtChip = NULL; // 획득 골드 텍스트
	m_pBtnOk = NULL; // OK 버튼

	m_nPopupType = 0;
	m_nRewardChip = 0;
}

CChipAttendSubPopup::~CChipAttendSubPopup(void)
{
}

BEGIN_MESSAGE_MAP(CChipAttendSubPopup, LSY::CLSYLibDialogBase)
	ON_WM_TIMER()
END_MESSAGE_MAP()

INT_PTR CChipAttendSubPopup::DoModal(CWnd *pParent)
{
	m_pParentWnd = pParent;
	return CLSYLibDialogBase::DoModal(GetObjectMan(), ID_CHIPATTENDSUBPOPUP);
}

bool CChipAttendSubPopup::OnCreatedProject(void)
{
	// LSY Control 연결
	InitLSYControl();
	// 데이터 세팅
	InitData();

	// 2초 뒤 팝업 닫기
	// 1 : 400 ~ 1,638,400 당첨 , 2 : 6,553,600 / 10,000,000 당첨
	if (m_nPopupType >= 1 && m_nPopupType <= 2)
		SetTimer(TIMER_CHIPATTEND_CLOSE_DLG, 2000, NULL);
	
	return TRUE;
}

/// <summary>
/// 타이머 함수 오버라이드
/// <summary>
/// <param name=""></param>
/// <returns></returns>
void CChipAttendSubPopup::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	switch(nIDEvent)
	{
	case TIMER_CHIPATTEND_CLOSE_DLG:
		{
			KillTimer(TIMER_CHIPATTEND_CLOSE_DLG);
			
			CloseDlg();
		}
		break;
	}
	
	CLSYLibDialogBase::OnTimer(nIDEvent);
}

void CChipAttendSubPopup::OnDestroy()
{
	CLSYLibDialogBase::OnDestroy();

	// TODO: Add your message handler code here	
	KillTimer(TIMER_CHIPATTEND_CLOSE_DLG);
}

/// <summary>
/// 버튼 클릭 이벤트. 대부분 서버에 요청 보내는 기능.
/// <summary>
/// <param name=""></param>
/// <returns></returns>
LRESULT CChipAttendSubPopup::OnClickButton(LSY::CMessage *pMsg)
{
	LSY::CObjectMessage *msg = (LSY::CObjectMessage *)pMsg;

	switch(msg->GetObject()->GetID())
	{
	case ID_CHIPATTENDSUBPOPUP_BTN_OK: // OK 버튼
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
void CChipAttendSubPopup::InitLSYControl()
{
	// 획득 골드 텍스트
	m_pImgBack = (LSY::CImage *)GetObject(ID_CHIPATTENDSUBPOPUP_IMG_BACK);
	// 획득 골드 텍스트
	m_pTxtChip = (LSY::CText *)GetObject(ID_CHIPATTENDSUBPOPUP_TXT_CHIP);
	m_pTxtChip->SetShow(false);
	// OK 버튼
	m_pBtnOk = (LSY::CButton *)GetObject(ID_CHIPATTENDSUBPOPUP_BTN_OK);
	m_pBtnOk->AddHandler(LSY::EM_O_MOUSELCLICK, LSY::Fnt(this, &CChipAttendSubPopup::OnClickButton));
	m_pBtnOk->SetShow(false);
}

/// <summary>
/// 데이터 세팅
/// <summary>
/// <param name=""></param>
/// <returns></returns>
void CChipAttendSubPopup::InitData()
{
	if (m_pImgBack == NULL || m_pTxtChip == NULL)
		return;
	
	switch (m_nPopupType)
	{
	// 400 ~ 1,638,400 당첨
	case 1:
		m_pImgBack->SetIndex(13);
		m_pTxtChip->SetShow(true);
		m_pBtnOk->SetShow(false);
		break;
	// 6,553,600 / 10,000,000 당첨
	case 2:
		m_pImgBack->SetIndex(14);
		m_pTxtChip->SetShow(true);
		m_pBtnOk->SetShow(false);
		break;
	// 오늘은 더 이상 참여할 수 없습니다.
	case 3:
		m_pImgBack->SetIndex(15);
		m_pTxtChip->SetShow(false);
		m_pBtnOk->SetShow(true);
		break;
	}

	CString csChip;
	CString csChipComma;
	csChip.Format("%I64d", m_nRewardChip);
	g_GetNumberWithComma(csChip, csChipComma);
	csChipComma.AppendFormat("%s을 획득하였습니다.", strChip_Name[(int)ROOM_CHIPKIND_GOLD]);
	m_pTxtChip->SetText((LPCTSTR)csChipComma);
}

void CChipAttendSubPopup::CloseDlg(void)
{
	OnCancel();
}

/// <summary>
/// 서버에서 보내는 패킷(응답) 처리
/// <summary>
/// <param name="nType">// 1 : 성공!, 2 : 실패!, 3 : 최종 성공, 4 : 종료</param>
/// <param name="nChip">획득한 금액</param>
/// <returns></returns>
void CChipAttendSubPopup::SetPopupType(int nType, INT64 nChip, bool bModaled)
{
	m_nPopupType = nType;
	m_nRewardChip = nChip; // 획득 골드 세팅
	
	if (bModaled)
	{
		// 데이터 세팅
		InitData();

		// 2초 뒤 팝업 닫기
		// 1 : 400 ~ 1,638,400 당첨 , 2 : 6,553,600 / 10,000,000 당첨
		if (m_nPopupType >= 1 && m_nPopupType <= 2)
			SetTimer(TIMER_CHIPATTEND_CLOSE_DLG, 2000, NULL);
	}
}
