#include "StdAfx.h"
#include "GoldAttendanceBookSubPopup.h"
#include "GoldAttendanceBookSubPopupid.h"

CGoldAttendanceBookSubPopup::CGoldAttendanceBookSubPopup(CWnd* pParent/* = NULL*/) : LSY::CLSYLibDialogBase(pParent)
{
	m_pImgBack = NULL; // 배경 이미지
	m_pBtnOk = NULL; // 계속하기 버튼
	m_pBtnClose = NULL; // 종료하기 버튼
	m_pBtnXClose = NULL; // X 버튼
	m_pTxtGold1 = NULL; // 획득 골드 텍스트
	m_pTxtGold2 = NULL; // 획득 골드 텍스트
	
	m_bCloseParent = false;
	m_nPopupType = 0;


}

CGoldAttendanceBookSubPopup::~CGoldAttendanceBookSubPopup(void)
{
}

BEGIN_MESSAGE_MAP(CGoldAttendanceBookSubPopup, LSY::CLSYLibDialogBase)
	ON_WM_TIMER()
END_MESSAGE_MAP()

INT_PTR CGoldAttendanceBookSubPopup::DoModal(CWnd *pParent)
{
	m_pParentWnd = pParent;
	return CLSYLibDialogBase::DoModal(GetObjectMan(), ID_GOLDATTENDANCEBOOKSUBPOPUP);
}

bool CGoldAttendanceBookSubPopup::OnCreatedProject(void)
{
	// LSY Control 연결
	InitLSYControl();
	// 데이터 세팅
	InitData();
	// 2초 뒤 팝업 닫기
	// 1 : 성공!, 2 : 실패!, 3 : 최종 성공, 4 : 종료</param>
	if (m_nPopupType >= 1 && m_nPopupType <= 3)
		SetTimer(TIMER_CLOSE_DLG, 2000, NULL);
	
	return TRUE;
}

/// <summary>
/// 타이머 함수 오버라이드
/// <summary>
/// <param name=""></param>
/// <returns></returns>
void CGoldAttendanceBookSubPopup::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	switch(nIDEvent)
	{
	case TIMER_CLOSE_DLG:
		{
			KillTimer(TIMER_CLOSE_DLG);
			
			CloseDlg();
		}
		break;
	}
	
	CLSYLibDialogBase::OnTimer(nIDEvent);
}

/// <summary>
/// 버튼 클릭 이벤트. 대부분 서버에 요청 보내는 기능.
/// <summary>
/// <param name=""></param>
/// <returns></returns>
LRESULT CGoldAttendanceBookSubPopup::OnClickButton(LSY::CMessage *pMsg)
{
	LSY::CObjectMessage *msg = (LSY::CObjectMessage *)pMsg;
	switch(msg->GetObject()->GetID())
	{
	case ID_GOLDATTENDANCEBOOKSUBPOPUP_BTN_OK: // 계속하기 버튼
		{
			m_bCloseParent = false;
			CloseDlg();
		}
		break;
	case ID_GOLDATTENDANCEBOOKSUBPOPUP_BTN_CLOSE: // 종료하기 버튼
		{
			m_bCloseParent = true;
			CloseDlg();
		}
		break;
	case ID_GOLDATTENDANCEBOOKSUBPOPUP_BTN_XCLOSE: // X 버튼
		{
			m_bCloseParent = false;
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
void CGoldAttendanceBookSubPopup::InitLSYControl()
{
	// 배경 이미지
	m_pImgBack = (LSY::CImage *)GetObject(ID_GOLDATTENDANCEBOOKSUBPOPUP_IMG_BACK);
	// 계속하기 버튼
	m_pBtnOk = (LSY::CButton *)GetObject(ID_GOLDATTENDANCEBOOKSUBPOPUP_BTN_OK);
	m_pBtnOk->AddHandler(LSY::EM_O_MOUSELCLICK, LSY::Fnt(this, &CGoldAttendanceBookSubPopup::OnClickButton));
	// 종료하기 버튼
	m_pBtnClose = (LSY::CButton *)GetObject(ID_GOLDATTENDANCEBOOKSUBPOPUP_BTN_CLOSE);
	m_pBtnClose->AddHandler(LSY::EM_O_MOUSELCLICK, LSY::Fnt(this, &CGoldAttendanceBookSubPopup::OnClickButton));
	// X 버튼
	m_pBtnXClose = (LSY::CButton *)GetObject(ID_GOLDATTENDANCEBOOKSUBPOPUP_BTN_XCLOSE);
	m_pBtnXClose->AddHandler(LSY::EM_O_MOUSELCLICK, LSY::Fnt(this, &CGoldAttendanceBookSubPopup::OnClickButton));
	// 획득 골드 텍스트
	m_pTxtGold1 = (LSY::CText *)GetObject(ID_GOLDATTENDANCEBOOKSUBPOPUP_TXT_GOLD1);
	m_pTxtGold2 = (LSY::CText *)GetObject(ID_GOLDATTENDANCEBOOKSUBPOPUP_TXT_GOLD2);
}

/// <summary>
/// 데이터 세팅
/// <summary>
/// <param name=""></param>
/// <returns></returns>
void CGoldAttendanceBookSubPopup::InitData()
{
	if (m_pImgBack == NULL ||
		m_pBtnOk == NULL || 
		m_pBtnClose == NULL || 
		m_pBtnXClose == NULL || 
		m_pTxtGold1 == NULL || 
		m_pTxtGold2 == NULL)
		return;
	
	switch (m_nPopupType)
	{
	// 성공 팝업
	case 1:
		{
			m_pImgBack->SetIndex(101);
			m_pBtnOk->SetShow(false);
			m_pBtnClose->SetShow(false);
			m_pTxtGold1->SetShow(true);
			m_pTxtGold2->SetShow(true);
			
			CString csGold;
			CString csGoldComma;
			csGold.Format("%I64d", m_nGold);
			GetNumberWithComma(csGold, csGoldComma);
			csGoldComma.AppendFormat("%s을 획득하였습니다.", strChip_Name[(int)ROOM_CHIPKIND_GOLD]);
			m_pTxtGold1->SetText("당첨을 축하 드립니다.");
			m_pTxtGold2->SetText((LPCTSTR)csGoldComma);
		}
		break;
	// 실패 팝업
	case 2:
		{
			m_pImgBack->SetIndex(102);
			m_pBtnOk->SetShow(false);
			m_pBtnClose->SetShow(false);
			m_pTxtGold1->SetShow(true);
			m_pTxtGold2->SetShow(true);
			
			CString csGold;
			CString csGoldComma;
			csGold.Format("%I64d", m_nGold);
			GetNumberWithComma(csGold, csGoldComma);
			csGoldComma.AppendFormat("%s을 지급합니다.", strChip_Name[(int)ROOM_CHIPKIND_GOLD]);
			m_pTxtGold1->SetText("안타깝게도 꽝입니다.");
			m_pTxtGold2->SetText((LPCTSTR)csGoldComma);
		}
		break;
	// 최종 성공 팝업
	case 3:
		{
			m_pImgBack->SetIndex(103);
			m_pBtnOk->SetShow(false);
			m_pBtnClose->SetShow(false);
			m_pTxtGold1->SetShow(true);
			m_pTxtGold2->SetShow(true);
			
			m_pTxtGold1->SetText("축하합니다.");
			CString csFinal;
			csFinal.Format("1,000만%s 지급 되었습니다.", strChip_Name[(int)ROOM_CHIPKIND_GOLD]);
			m_pTxtGold2->SetText((LPCTSTR)csFinal);
		}
		break;
	// 종료 팝업
	case 4:
		{
			m_pImgBack->SetIndex(100);
			m_pBtnOk->SetShow(true);
			m_pBtnClose->SetShow(true);
			m_pTxtGold1->SetShow(false);
			m_pTxtGold2->SetShow(false);
		}
		break;
	}
}

void CGoldAttendanceBookSubPopup::CloseDlg(void)
{
	KillTimer(TIMER_CLOSE_DLG);
	
	OnCancel();
}

/// <summary>
/// 서버에서 보내는 패킷(응답) 처리
/// <summary>
/// <param name=""></param>
/// <returns></returns>
void CGoldAttendanceBookSubPopup::HandleResponse(int Signal, int TotSize, char *lpData) // 서버 응답 핸들링
{
}

/// <summary>
/// 서버에서 보내는 패킷(응답) 처리
/// <summary>
/// <param name="nType">// 1 : 성공!, 2 : 실패!, 3 : 최종 성공, 4 : 종료</param>
/// <param name="nGold">획득한 금액</param>
/// <returns></returns>
void CGoldAttendanceBookSubPopup::SetPopupType(int nType, INT64 nGold, bool bModaled)
{
	m_nPopupType = nType;
	m_nGold = nGold; // 획득 골드 세팅
	
	if (bModaled)
	{
		// 데이터 세팅
		InitData();
		// 2초 뒤 팝업 닫기
		// 1 : 성공!, 2 : 실패!, 3 : 최종 성공, 4 : 종료</param>
		if (m_nPopupType >= 1 && m_nPopupType <= 3)
			SetTimer(TIMER_CLOSE_DLG, 2000, NULL);
	}
}

bool CGoldAttendanceBookSubPopup::GetCloseParent()
{
	return m_bCloseParent;
}

/// <summary>
/// 세 자리 수 마다 콤마 찍어주기
/// <summary>
/// <param name="nType">성공시 획득 골드</param>
/// <returns></returns>
void CGoldAttendanceBookSubPopup::GetNumberWithComma(CString parm_data, CString &parm_string)
{
    // 결과값을 정확하게 구성하기 위해서 ',' 처리결과가 저장될 문자열 변수를 초기화한다.
    if(parm_string.GetLength()) parm_string.Format("");
    // 문자열의 길이를 구한다.
    int count = parm_data.GetLength();
	
    for(int i = 0; i < count; i++)
	{
        // 3의 배수일 때 "," 를 문자열에 추가한다.
        if(i && !(i % 3)) parm_string = "," + parm_string;
        // parm_data 에서 parm_string으로 숫자한개씩 순차적으로 복사한다.
        parm_string = CString(parm_data[count - 1 - i]) + parm_string;
    }
}

/// <summary>
/// 세 자리 수 마다 콤마 찍어주기
/// 이 함수 사용하면 안됨! 특정 PC에서 숫자 깨지는 현상 발생.
/// <summary>
/// <param name="nType">성공시 획득 골드</param>
/// <returns></returns>
void CGoldAttendanceBookSubPopup::GetNumberWithComma2(INT64 nNum, char cBuffer[])
{
	CString csNum;
	csNum.Format(_T("%I64d"), nNum);
	NUMBERFMT fmt = {0, 0, 3, ".", ",", 1};
	::GetNumberFormat(NULL, NULL, csNum, &fmt, cBuffer, strlen(cBuffer));
}
