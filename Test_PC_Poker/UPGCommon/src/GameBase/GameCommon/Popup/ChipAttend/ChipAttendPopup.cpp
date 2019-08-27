#include "StdAfx.h"
#include "ChipAttendPopup.h"
#include "ChipAttendpopupid.h"

CChipAttendPopup::CChipAttendPopup(CWnd* pParent/* = NULL*/) : LSY::CLSYLibDialogBase(pParent)
{
	// 서버에서 받은 값
	// CSV_PM_WHEEL_USER_BET_RESULT
	m_RoundReward = 0; //도전한 획득하게되는 보상금
	m_RewardIndex = 0;
	// CSV_PM_WHEEL_WINNER_NOTI
	m_UserID = ""; //아이디 혹은 닉네임
	m_RewardMoney = 0; //당첨금
	m_RoundWinner = 0; //해당 라운드 당첨자 수
	
	// 기타 변수
	m_bInProgress = false; // 게임 진행 중인지 여부

#ifdef _DEBUG
	m_bTest = false;
#else
	m_bTest = false;
#endif
}

CChipAttendPopup::~CChipAttendPopup()
{
}

BEGIN_MESSAGE_MAP(CChipAttendPopup, LSY::CLSYLibDialogBase)
	ON_WM_TIMER()
	ON_WM_DESTROY()
END_MESSAGE_MAP()

INT_PTR CChipAttendPopup::DoModal(CWnd *pParent)
{
	m_pParentWnd = pParent;
	INT_PTR iRes = CLSYLibDialogBase::DoModal(GetObjectMan(), ID_CHIPATTENDPOPUP);

	return iRes;
}

bool CChipAttendPopup::OnCreatedProject()
{
	// LSY Control 연결
	InitLSYControl();

	if (m_bTest)
		return TRUE;

	//게임시작을 요청한다.
	//CString authentiCode;
	//MM().Call(UPG_CMK_GET_AUTHENTICODE_CAPTCHA, (WPARAM)(&authentiCode));

	CCL_PM_WHEEL_REQUEST_START msg;
	//const char* captchaKey = authentiCode;
	//msg.Set(captchaKey);
	msg.Set("");
	NMBASE::SOCK::GSOCKMAN().SendData(g_strMainServerName.GetBuffer(), msg.pData, msg.GetTotalSize());
	
	return TRUE;
}

BOOL CChipAttendPopup::PreTranslateMessage(MSG* pMsg)
{
	/*
	// alt + f4 막는거같음
	if (pMsg->message == WM_SYSKEYDOWN)
	{
		if (pMsg->wParam == VK_F4)
		{
			return TRUE;
		}
	}
	}*/

	// esc 키 막음
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_ESCAPE)
		{
			return TRUE;
		}
	}

	return CWnd::PreTranslateMessage(pMsg);
}

void CChipAttendPopup::OnCancel()
{
	// 게임이 진행 중 일 경우 닫기 안됨.
	if (m_bInProgress)
	{
		ShowMainMessageDlg("게임 진행 중에는 종료할 수 없습니다.");
		
		return;
	}

	// 현재 골드 출석부 결과 연출중인지 여부.
	GM().SetPlayDailyGoldEffect(false);

	__super::OnCancel();
}

/// <summary>
/// 타이머 함수 오버라이드
/// <summary>
/// <param name=""></param>
/// <returns></returns>
void CChipAttendPopup::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	switch(nIDEvent)
	{
	case TIMER_CHIPATTEND_WHEEL_ANI:
		{
			KillTimer(TIMER_CHIPATTEND_WHEEL_ANI);

			// 럭키팡팡 게임 애니메이션

			int nIdx = GetWheelAngleIdx();

			m_pSprWheel->SetPlayIndex(0);
			m_pSprWheel->ChangeMultiScene(1, nIdx);
			m_pSprWheel->SetRepeatCount(1);
			m_pSprWheel->StartAnimation();

			NMBASE::SOUND::g_Sound.PlayWav(SND_FX_SPIN_MIX);

			// 성공 팝업 띄움
			DWORD dwPlayTime = m_pSprWheel->GetPlayTime();

			SetTimer(TIMER_CHIPATTEND_RESULT_DLG, dwPlayTime, NULL);

			if (m_bTest)
				break;
			
			m_pBtnStart->SetEnable(false);
		}
		break;
	case TIMER_CHIPATTEND_RESULT_DLG:
		{
			KillTimer(TIMER_CHIPATTEND_RESULT_DLG);

			NMBASE::SOUND::g_Sound.Stop(SND_FX_SPIN_MIX);
			NMBASE::SOUND::g_Sound.PlayWav(SND_FX_BJ_WIN);

			m_bInProgress = false;
			m_pBtnClose->SetEnable(true);
			
			// 성공! 팝업
			bool bModaled = m_popChipAttendSub.GetSafeHwnd();

			// 팝업 종류 설정
			// 1 : 500 ~ 1,500,000 당첨 , 2 : 5,000,000 / 10,000,000 당첨
			// 3 : 오늘은 더 이상 참여할 수 없습니다.
			int nType = m_RewardIndex < 7 ? 1 : 2;
			m_popChipAttendSub.SetPopupType(nType, m_RoundReward, bModaled);
			
			if (bModaled == false)
				m_popChipAttendSub.DoModal(this);
			
			if (m_bTest)
				break;

			OnCancel();
		}
		break;
	}
	
	CLSYLibDialogBase::OnTimer(nIDEvent);
}

void CChipAttendPopup::OnDestroy()
{
	CLSYLibDialogBase::OnDestroy();

	// TODO: Add your message handler code here
	GM().SetPlayDailyGoldEffect(false);
	KillTimer(TIMER_CHIPATTEND_WHEEL_ANI);
	KillTimer(TIMER_CHIPATTEND_RESULT_DLG);
}

/// <summary>
/// 버튼 클릭 이벤트. 대부분 서버에 요청 보내는 기능.
/// <summary>
/// <param name=""></param>
/// <returns></returns>
LRESULT CChipAttendPopup::OnClickButton(LSY::CMessage *pMsg)
{
	LSY::CObjectMessage *msg = (LSY::CObjectMessage *)pMsg;

	switch(msg->GetObject()->GetID())
	{
	// Start 버튼
	case ID_CHIPATTENDPOPUP_BTN_START:
		{
			NMBASE::SOUND::g_Sound.PlayWav(SND_FX_BJ_BEFORE_WIN);

			if (m_bTest)
			{
				// 휠 애니메이션 실행
				m_bInProgress = true;

				m_pBtnClose->SetEnable(false);

				SetTimer(TIMER_CHIPATTEND_WHEEL_ANI, 0, NULL);

				break;
			}

			//유저 배팅.
			CCL_PM_WHEEL_USER_BET msg;
			msg.Set(0, 0);
			NMBASE::SOCK::GSOCKMAN().SendData(g_strMainServerName.GetBuffer(), msg.pData, msg.GetTotalSize());
			
			m_bInProgress = true;
		}
		break;
	// 닫기 버튼
	case ID_CHIPATTENDPOPUP_BTN_CLOSE:
		{
			OnCancel();
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
void CChipAttendPopup::InitLSYControl()
{
	// 시작 버튼
	m_pBtnStart = (LSY::CButton *)GetObject(ID_CHIPATTENDPOPUP_BTN_START);
	m_pBtnStart->AddHandler(LSY::EM_O_MOUSELCLICK, LSY::Fnt(this, &CChipAttendPopup::OnClickButton));

	if (m_bTest)
		m_pBtnStart->SetEnable(true);
	else
		m_pBtnStart->SetEnable(false);

	// 닫기 버튼
	m_pBtnClose = (LSY::CButton *)GetObject(ID_CHIPATTENDPOPUP_BTN_CLOSE);
	m_pBtnClose->AddHandler(LSY::EM_O_MOUSELCLICK, LSY::Fnt(this, &CChipAttendPopup::OnClickButton));
	// 휠
	m_pSprWheel = (LSY::CSprite *)GetObject(ID_CHIPATTENDPOPUP_SPR_WHEEL);
	m_pSprWheel->SetPlayIndex(1);
	m_pSprWheel->SetRepeatCount(0);
	m_pSprWheel->StartAnimation();
	// 이벤트 기간 텍스트
	m_pTxtDate = (LSY::CText *)GetObject(ID_CHIPATTENDPOPUP_TXT_DATE);
}

/// <summary>
/// 데이터 세팅
/// <summary>
/// <param name=""></param>
/// <returns></returns>
void CChipAttendPopup::SetData()
{

}

/// <summary>
/// 서버에서 보내는 패킷(응답) 처리
/// <summary>
/// <param name=""></param>
/// <returns></returns>
void CChipAttendPopup::HandleResponse(int Signal, int TotSize, char *lpData) // 서버 응답 핸들링
{
	switch(Signal)
	{
	// 게임시작 요청에 대한 결과이다.
	case SV_PM_WHEEL_RESPONSE_START:
		{
			CSV_PM_WHEEL_RESPONSE_START msg;
			msg.Get(lpData, TotSize);

			//이 패킷을 true로 받으면 게임진행을 할 수 있다고 판단하여, 서버에 게임 시작 요청 패킷을 보내면 된다.
			int nErrCode = *msg.m_ErrCode;
			CString startDate = msg.m_StartDate;
			CString endDate = msg.m_EndDate;

			CString evtDate;

			startDate = startDate.Left(10);
			endDate = endDate.Left(10);

			startDate.Replace("-", "/");
			endDate.Replace("-", "/");

			evtDate.Format("기간: %s ~ %s", startDate, endDate);
			m_pTxtDate->SetText((LPCTSTR)evtDate);

			switch (nErrCode)
			{
			// 정상 시작
			case 0:
				m_pBtnStart->SetEnable(true);
				m_pSprWheel->ChangeMultiScene(5, 5);
				m_pSprWheel->StartAnimation();
				break;
			// 오늘 이미 진행함
			case 1:
				{
					//ShowMainMessageDlg("오늘은 더 이상 참여할 수 없습니다.");
					
					bool bModaled = m_popChipAttendSub.GetSafeHwnd();

					// 팝업 종류 설정
					// 1 : 500 ~ 1,500,000 당첨 , 2 : 5,000,000 / 10,000,000 당첨
					// 3 : 오늘은 더 이상 참여할 수 없습니다.
					m_popChipAttendSub.SetPopupType(3, m_RoundReward, bModaled);

					if (bModaled == false)
						m_popChipAttendSub.DoModal(this);

					MM().Call(UPG_CMK_SET_LOBBY_BUTTON_ENABLE, (WPARAM)false);
					OnCancel();
				}
				break;
			// 기타 오류
			case 2:
				{
					ShowMainMessageDlg("기타 오류. 관리자에게 문의 주세요.");
					MM().Call(UPG_CMK_SET_LOBBY_BUTTON_ENABLE, (WPARAM)false);
					OnCancel();
				}
				break;
			default:
				{
					MM().Call(UPG_CMK_SET_LOBBY_BUTTON_ENABLE, (WPARAM)false);
					OnCancel();
				}
				break;
			}
		}
		break;
	//유저배팅에 대한 결과이다.
	//배팅에 실패했을경우 종료여부가 true로 가며, 성공하면 종료여부가 false 면서 CSV_PM_ODDEVEN_START_ROUND 패킷이 다시 전송됨
	case SV_PM_WHEEL_USER_BET_RESULT:
		{
			GM().SetPlayDailyGoldEffect(true);

			CSV_PM_WHEEL_USER_BET_RESULT msg;
			msg.Get(lpData, TotSize);

			m_RoundReward = *msg.m_RoundReward;
			m_RewardIndex = *msg.m_RewardIndex;

			// 휠 애니메이션 실행
			m_bInProgress = true;

			MM().Call(UPG_CMK_SET_LOBBY_BUTTON_ENABLE, (WPARAM)false);
			m_pBtnClose->SetEnable(false);

			SetTimer(TIMER_CHIPATTEND_WHEEL_ANI, 0, NULL);
		}
		break;
	}
}


int CChipAttendPopup::GetWheelAngleIdx()
{
	int nIdx = 6;

	srand((unsigned int)time(NULL));

	int indexArray[9][2] = { {1, 4}, {17, 4}, {13, 4}, {5, 3}, {8, 3}, {21, 2}, {23, 2}, {11, 1}, {12, 1} };
	nIdx += (indexArray[m_RewardIndex][0] + rand() % indexArray[m_RewardIndex][1]) % 24;

	return nIdx;
}