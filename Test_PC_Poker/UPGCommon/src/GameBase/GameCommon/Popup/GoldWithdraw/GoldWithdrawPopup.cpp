#include "StdAfx.h"
#include "GoldWithdrawPopup.h"
#include "goldwithdrawpopupid.h"


CGoldWithdrawPopup::CGoldWithdrawPopup(CWnd* pParent/* = NULL*/) : LSY::CLSYLibDialogBase(pParent)
{
	InitLSYControlToNULL(); // LSY Control NULL 값으로 초기화

	// 기타 변수
	m_i64SaveGold = 0; // 적립 골드
	m_i64WithdrawGold = 0; // 출금 골드
	m_i64MyGold = 0; // 변경 후 골드

	m_nLimit = 0; // 현재 유저 골드 적립 한도, 로그인시 받아옴

	m_csLimitNormal = "2,000,000"; // 골드 적립 한도 일반
	m_csLimitGold = "5,000,000"; // 골드 적립 한도 골드
	m_csLimitDia = "15,000,000"; // 골드 적립 한도 다이아
}

CGoldWithdrawPopup::~CGoldWithdrawPopup(void)
{
}

INT_PTR CGoldWithdrawPopup::DoModal(CWnd *pParent)
{
	m_pParentWnd = pParent;
	return CLSYLibDialogBase::DoModal(GetObjectMan(), ID_GOLDWITHDRAWPOPUP);
}

bool CGoldWithdrawPopup::OnCreatedProject(void)
{
	// LSY Control 연결
	InitLSYControl();
	// 초기 데이터 세팅
	InitData();

	return TRUE;
}

void CGoldWithdrawPopup::OnCancel()
{
	__super::OnCancel();
}

/// <summary>
/// 버튼 클릭 이벤트. 대부분 서버에 요청 보내는 기능.
/// <summary>
/// <param name=""></param>
/// <returns></returns>
LRESULT CGoldWithdrawPopup::OnClickButton(LSY::CMessage *pMsg)
{
	LSY::CObjectMessage *msg = (LSY::CObjectMessage *)pMsg;

	switch(msg->GetObject()->GetID())
	{
	case ID_GOLDWITHDRAWPOPUP_BTN_CLOSE: // 닫기 버튼
		CloseDlg();
		break;
	case ID_GOLDWITHDRAWPOPUP_BTN_001: // 10만 버튼
		{
			RaiseWithdrawGold(10 * 10000);
		}
		break;
	case ID_GOLDWITHDRAWPOPUP_BTN_002: // 30만 버튼
		{
			RaiseWithdrawGold(30 * 10000);
		}
		break;
	case ID_GOLDWITHDRAWPOPUP_BTN_003: // 50만 버튼
		{
			RaiseWithdrawGold(50 * 10000);
		}
		break;
	case ID_GOLDWITHDRAWPOPUP_BTN_004: // 초기화 버튼
		{
			m_i64WithdrawGold = 0;
			
			SetData();
		}
		break;
	case ID_GOLDWITHDRAWPOPUP_BTN_005: // 100만
		{
			RaiseWithdrawGold(100 * 10000);
		}
		break;
	case ID_GOLDWITHDRAWPOPUP_BTN_006: // 200만
		{
			RaiseWithdrawGold(200 * 10000);
		}
		break;
	case ID_GOLDWITHDRAWPOPUP_BTN_007: // 500만
		{
			RaiseWithdrawGold(500 * 10000);
		}
		break;
	case ID_GOLDWITHDRAWPOPUP_BTN_008: // 최대
		{
			// i64MaxGold : 최대로 출금할 수 있는 골드
			INT64 i64MaxGold = m_i64SaveGold;
			
			// 최대로 출금할 수 있는 골드는 보유한도를 넘어서는 안된다
			if (m_i64MyGold + i64MaxGold > m_nLimit)
				i64MaxGold = m_nLimit - m_i64MyGold;
			
			// i64AGold : 추가 출금 골드
			INT64 i64AGold = i64MaxGold - m_i64WithdrawGold;
			
			RaiseWithdrawGold(i64AGold);
		}
		break;
	case ID_GOLDWITHDRAWPOPUP_BTN_WITHDRAW: // 출금하기
		{
			CCL_GOLDBANK_WITHDRAW sndMsg;
			sndMsg.Set(m_i64WithdrawGold);
			NMBASE::SOCK::GSOCKMAN().SendData(g_strMainServerName.GetBuffer(), sndMsg.pData, sndMsg.GetTotalSize());
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
void CGoldWithdrawPopup::InitLSYControl()
{
	// 닫기 버튼
	m_pBtnClose = (LSY::CButton *)GetObject(ID_GOLDWITHDRAWPOPUP_BTN_CLOSE);
	m_pBtnClose->AddHandler(LSY::EM_O_MOUSELCLICK, LSY::Fnt(this, &CGoldWithdrawPopup::OnClickButton));

	// 적립 골드
	m_pTxtSaveGold = (LSY::CText *)GetObject(ID_GOLDWITHDRAWPOPUP_TXT_SAVEGOLD);
	// 출금 골드
	m_pTxtWithdrawGold = (LSY::CText *)GetObject(ID_GOLDWITHDRAWPOPUP_TXT_WITHDRAWGOLD);
	// 변경 후 골드
	m_pTxtMyGold = (LSY::CText *)GetObject(ID_GOLDWITHDRAWPOPUP_TXT_MYGOLD);

	// 10만
	m_pBtn001 = (LSY::CButton *)GetObject(ID_GOLDWITHDRAWPOPUP_BTN_001);
	m_pBtn001->AddHandler(LSY::EM_O_MOUSELCLICK, LSY::Fnt(this, &CGoldWithdrawPopup::OnClickButton));
	// 30만
	m_pBtn002 = (LSY::CButton *)GetObject(ID_GOLDWITHDRAWPOPUP_BTN_002);
	m_pBtn002->AddHandler(LSY::EM_O_MOUSELCLICK, LSY::Fnt(this, &CGoldWithdrawPopup::OnClickButton));
	// 50만
	m_pBtn003 = (LSY::CButton *)GetObject(ID_GOLDWITHDRAWPOPUP_BTN_003);
	m_pBtn003->AddHandler(LSY::EM_O_MOUSELCLICK, LSY::Fnt(this, &CGoldWithdrawPopup::OnClickButton));
	// 초기화
	m_pBtn004 = (LSY::CButton *)GetObject(ID_GOLDWITHDRAWPOPUP_BTN_004);
	m_pBtn004->AddHandler(LSY::EM_O_MOUSELCLICK, LSY::Fnt(this, &CGoldWithdrawPopup::OnClickButton));
	// 100만
	m_pBtn005 = (LSY::CButton *)GetObject(ID_GOLDWITHDRAWPOPUP_BTN_005);
	m_pBtn005->AddHandler(LSY::EM_O_MOUSELCLICK, LSY::Fnt(this, &CGoldWithdrawPopup::OnClickButton));
	// 200만
	m_pBtn006 = (LSY::CButton *)GetObject(ID_GOLDWITHDRAWPOPUP_BTN_006);
	m_pBtn006->AddHandler(LSY::EM_O_MOUSELCLICK, LSY::Fnt(this, &CGoldWithdrawPopup::OnClickButton));
	// 500만
	m_pBtn007 = (LSY::CButton *)GetObject(ID_GOLDWITHDRAWPOPUP_BTN_007);
	m_pBtn007->AddHandler(LSY::EM_O_MOUSELCLICK, LSY::Fnt(this, &CGoldWithdrawPopup::OnClickButton));
	// 최대
	m_pBtn008 = (LSY::CButton *)GetObject(ID_GOLDWITHDRAWPOPUP_BTN_008);
	m_pBtn008->AddHandler(LSY::EM_O_MOUSELCLICK, LSY::Fnt(this, &CGoldWithdrawPopup::OnClickButton));

	// 골드 적립 한도 일반
	m_pTxtLimitNormal = (LSY::CText *)GetObject(ID_GOLDWITHDRAWPOPUP_TXT_LIMITNORMAL);
	// 골드 적립 한도 골드
	m_pTxtLimitGold = (LSY::CText *)GetObject(ID_GOLDWITHDRAWPOPUP_TXT_LIMITGOLD);
	// 골드 적립 한도 다이아
	m_pTxtLimitDia = (LSY::CText *)GetObject(ID_GOLDWITHDRAWPOPUP_TXT_LIMITDIA);

	// 출금하기 버튼
	m_pBtnWithdraw = (LSY::CButton *)GetObject(ID_GOLDWITHDRAWPOPUP_BTN_WITHDRAW);
	m_pBtnWithdraw->AddHandler(LSY::EM_O_MOUSELCLICK, LSY::Fnt(this, &CGoldWithdrawPopup::OnClickButton));
	m_pBtnWithdraw->SetEnable(false);
}

/// <summary>
/// LSY Control NULL 값으로 초기화.
/// 생성자, OnClose 함수에서 호출.
/// <summary>
/// <param name=""></param>
/// <returns></returns>
void CGoldWithdrawPopup::InitLSYControlToNULL()
{
	// LSY 컨트롤 변수
	m_pBtnClose = NULL; // 닫기 버튼

	m_pTxtSaveGold = NULL; // 적립 골드 텍스트
	m_pTxtWithdrawGold = NULL; // 출금 골드 텍스트
	m_pTxtMyGold = NULL; // 변경 후 골드 텍스트

	m_pBtn001 = NULL; // 10만 버튼
	m_pBtn002 = NULL; // 30만 버튼
	m_pBtn003 = NULL; // 50만 버튼
	m_pBtn004 = NULL; // 초기화 버튼
	m_pBtn005 = NULL; // 100만 버튼
	m_pBtn006 = NULL; // 200만 버튼
	m_pBtn007 = NULL; // 500만 버튼
	m_pBtn008 = NULL; // 최대 버튼

	m_pTxtLimitNormal = NULL; // 골드 적립 한도 일반 텍스트
	m_pTxtLimitGold = NULL; // 골드 적립 한도 골드 텍스트
	m_pTxtLimitDia = NULL; // 골드 적립 한도 다이아 텍스트

	m_pBtnWithdraw = NULL; // 출금하기 버튼
}

/// <summary>
/// 데이터 초기값 세팅
/// <summary>
/// <param name=""></param>
/// <returns></returns>
void CGoldWithdrawPopup::InitData()
{
	if (m_pTxtSaveGold == NULL || m_pTxtWithdrawGold == NULL || m_pTxtMyGold == NULL ||
		m_pTxtLimitNormal == NULL || m_pTxtLimitGold == NULL || m_pTxtLimitDia == NULL)
		return;

	m_i64WithdrawGold = 0;

	// 금액에 세 자리수 마다 콤마 추가
	CString csSaveGold = g_GetNumberWithComma(m_i64SaveGold); // 적립 골드
	CString csWithdrawGold = g_GetNumberWithComma(m_i64WithdrawGold); // 출금 골드
	CString csMyGold = g_GetNumberWithComma(m_i64MyGold); // 변경 후 골드

	m_pTxtSaveGold->SetText(csSaveGold.GetBuffer(0)); // 적립 골드
	m_pTxtWithdrawGold->SetText(csWithdrawGold.GetBuffer(0)); // 출금 골드
	m_pTxtMyGold->SetText(csMyGold.GetBuffer(0)); // 변경 후 골드

	m_pTxtLimitNormal->SetText(m_csLimitNormal.GetBuffer(0)); // 골드 적립 한도 일반
	m_pTxtLimitGold->SetText(m_csLimitGold.GetBuffer(0)); // 골드 적립 한도 골드
	m_pTxtLimitDia->SetText(m_csLimitDia.GetBuffer(0)); // 골드 적립 한도 다이아

	SetSumBtn(); // 금액 버튼 Enable 설정

	m_nLimit = g_MaxServerGold; // 현재 유저 골드 적립 한도, 로그인시 받아옴
}

/// <summary>
/// 데이터 세팅
/// <summary>
/// <param name=""></param>
/// <returns></returns>
void CGoldWithdrawPopup::SetData()
{
	if (m_pTxtSaveGold == NULL || m_pTxtWithdrawGold == NULL || m_pTxtMyGold == NULL || m_pBtnWithdraw == NULL)
		return;

	// 금액에 세 자리수 마다 콤마 추가
	CString csSaveGold = g_GetNumberWithComma(m_i64SaveGold); // 적립 골드
	CString csWithdrawGold = g_GetNumberWithComma(m_i64WithdrawGold); // 출금 골드
	CString csMyGold_sum = g_GetNumberWithComma(m_i64MyGold + m_i64WithdrawGold); // 변경 후 골드
	
	m_pTxtSaveGold->SetText(csSaveGold.GetBuffer(0)); // 적립 골드
	m_pTxtWithdrawGold->SetText(csWithdrawGold.GetBuffer(0)); // 출금 골드
	m_pTxtMyGold->SetText(csMyGold_sum.GetBuffer(0)); // 변경 후 골드
	
	if (m_i64WithdrawGold <= 0)
		m_pBtnWithdraw->SetEnable(false);
	else
		m_pBtnWithdraw->SetEnable(true);
	
	SetSumBtn(); // 금액 버튼 Enable 설정
}

void CGoldWithdrawPopup::CloseDlg(void)
{
	InitLSYControlToNULL();
	
	OnCancel();
}

/// <summary>
/// 서버에서 보내는 패킷(응답) 처리
/// <summary>
/// <param name=""></param>
/// <returns></returns>
void CGoldWithdrawPopup::HandleResponse(int Signal, int TotSize, char *lpData)
{
	switch(Signal)
	{
	case SV_ETCCHIP_INFO:
		{
			CSV_ETCCHIP_INFO msg;
			msg.Get(lpData, TotSize);
			
			if (strcmp(msg.m_ChipType, "GD") == 0)
			{
				m_i64MyGold = *msg.m_RealChip; // 변경 후 골드
			}
			else if (strcmp(msg.m_ChipType, "GB") == 0)
			{
				m_i64SaveGold = *msg.m_RealChip; // 적립 골드
			}
			
			SetData();
		}
		break;
	case SV_GOLDBANK_WITHDRAW_RESULT:
		{
			CSV_GOLDBANK_WITHDRAW_RESULT msg;
			msg.Get(lpData, TotSize);
			
			int nResult = *msg.m_Result; //결과코드 0.성공, 1.계좌잔액부족, 2.골드보유한도에걸림, 99.기타오류
			
			switch (nResult)
			{
			case 0:
				{
					CString strMsg;
					strMsg.Format("정상적으로 %s이 이동 되었습니다.", strChip_Name[(int)ROOM_CHIPKIND_GOLD]);
					m_dlgMsgBox.DoModal(CMsgBoxDlg::EMBT_NOTICE, strMsg);
					GM().SetOverMoneyGB(false);
				}
				break;
			case 1:
				// 출금버튼 누르기 전에 UI 단계에서 유효성 검사가 되기 때문에 이 메시지가 필요 없음
				{
					CString strMsg;
					strMsg.Format("적립 %s이 부족합니다. %s을 확인해 주세요.", strChip_Name[(int)ROOM_CHIPKIND_GOLD], strChip_Name[(int)ROOM_CHIPKIND_GOLD]);
					m_dlgMsgBox.DoModal(CMsgBoxDlg::EMBT_NOTICE, strMsg);
				}
				return;
			case 2:
				// 출금버튼 누르기 전에 UI 단계에서 유효성 검사가 되기 때문에 이 메시지가 필요 없음
				m_dlgMsgBox.DoModal(CMsgBoxDlg::EMBT_NOTICE, "보유 한도를 초과시 출금이 불가능합니다.");
				return;
			case 99:
				m_dlgMsgBox.DoModal(CMsgBoxDlg::EMBT_NOTICE, "알 수 없는 오류.\nError code : 99");
				return;
			default:
				return;
			}

			//INT64 m_WithDrawAmount = *msg.m_WithDrawAmount; //인출된 금액(최초 요청한 금액)
			m_i64SaveGold = *msg.m_RemainGoldBank; //인출후 골드계좌에 남아 있는 금액
			m_i64MyGold = *msg.m_CurGold; //인출금액 합산후 보유하게된 골드
			m_i64WithdrawGold = 0; // 출금 골드
			
			SetData();
		}
		break;
	}
}

/// <summary>
/// 기존 출금 골드에 추가 출금 골드 추가
/// <summary>
/// <param name="nAmount">추가 출금 골드</param>
/// <returns></returns>
void CGoldWithdrawPopup::RaiseWithdrawGold(INT64 nAmount)
{
	// 기존 출금 골드 + 추가 출금 골드 + 보유 골드 > 보유 한도 -> 보유 한도 초과
	if (m_i64WithdrawGold + nAmount + m_i64MyGold > m_nLimit)
	{
		m_dlgMsgBox.DoModal(CMsgBoxDlg::EMBT_NOTICE, "보유 한도를 초과시 출금이 불가능합니다.");
		
		return;
	}
	// 기존 출금 골드가 이미 적립금 전체일 경우
	else if (m_i64WithdrawGold >= m_i64SaveGold)
	{
		CString strMsg;
		strMsg.Format("적립 %s이 부족합니다. %s을 확인해 주세요.", strChip_Name[(int)ROOM_CHIPKIND_GOLD], strChip_Name[(int)ROOM_CHIPKIND_GOLD]);
		m_dlgMsgBox.DoModal(CMsgBoxDlg::EMBT_NOTICE, strMsg);
		
		return;
	}
	// 추가 출금 골드가 0원이고 변경후 골드가 한도에 도달했을 경우
	else if (nAmount == 0 && m_i64WithdrawGold + nAmount + m_i64MyGold >= m_nLimit)
	{
		m_dlgMsgBox.DoModal(CMsgBoxDlg::EMBT_NOTICE, "보유 한도를 초과시 출금이 불가능합니다.");
	}
	// 기존 출금 골드 + 추가 츌굼 골드가 적립 골드보다 많을 경우
	else if (m_i64WithdrawGold + nAmount > m_i64SaveGold)
	{
		m_i64WithdrawGold = m_i64SaveGold;
	}
	else
	{
		m_i64WithdrawGold += nAmount;
	}
	
	SetData();
}

// 금액 버튼 Enable 설정
void CGoldWithdrawPopup::SetSumBtn()
{
	if (m_pBtn001 == NULL || m_pBtn002 == NULL || m_pBtn003 == NULL ||
		m_pBtn005 == NULL || m_pBtn006 == NULL || m_pBtn007 == NULL)
	{
		return;
	}
	
	bool b10 = false;
	bool b30 = false;
	bool b50 = false;
	bool b100 = false;
	bool b200 = false;
	bool b500 = false;
	
	// 적립 골드가 출금 금액 버튼 보다 적을 경우 dim 처리된 게 맞습니다.
	// 라고 하길래 추가.
	if (m_i64SaveGold >= 100000)
		b10 = true; // 10만 버튼
	if (m_i64SaveGold >= 300000)
		b30 = true; // 30만 버튼
	if (m_i64SaveGold >= 500000)
		b50 = true; // 50만 버튼
	if (m_i64SaveGold >= 1000000)
		b100 = true; // 100만 버튼
	if (m_i64SaveGold >= 2000000)
		b200 = true; // 200만 버튼
	if (m_i64SaveGold >= 5000000)
		b500 = true; // 500만 버튼
	
	m_pBtn001->SetEnable(b10); // 10만 버튼
	m_pBtn002->SetEnable(b30); // 30만 버튼
	m_pBtn003->SetEnable(b50); // 50만 버튼
	m_pBtn005->SetEnable(b100); // 100만 버튼
	m_pBtn006->SetEnable(b200); // 200만 버튼
	m_pBtn007->SetEnable(b500); // 500만 버튼
}
