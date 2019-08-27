#include "StdAfx.h"
#include "GoldAttendanceBookPopup.h"
#include "goldattendancebookpopupid.h"

CGoldAttendanceBookPopup::CGoldAttendanceBookPopup(CWnd* pParent/* = NULL*/) : LSY::CLSYLibDialogBase(pParent)
{
	InitLSYControlToNULL(); // LSY Control NULL 값으로 초기화

	// 기타 변수
	m_Round_Cur = 1; //단계 1 ~ 8까지 있음
	m_CurrentReward = 0; //지금까지 확보된 보상금액(배팅실패, 혹은 게임포기를 하면 받게 되는 보상금)
	m_WinReward = 0; //이번 라운드에서 배팅성공시 받게되는 보상금액이다.
	m_BetTime = 0; //배팅제한시간이다(초), 만일 -1로 가면 제한이 없다는 의미
	
	m_BetSuccess = false; //성공여부
	m_Round = 1; //도전한 라운드
	m_CardNum = 0; //이번판 결과로 부여된 카드번호이다.(세븐포커 카드 인덱스와 동일함)
	m_RoundReward = 0; //도전한 라운드에 배팅성공시 획득하게되는 보상금
	
	m_bContinue = false; // 게임 진행 가능 여부
	m_bInProgress = true; // 게임 진행 중인지 여부
}

CGoldAttendanceBookPopup::~CGoldAttendanceBookPopup()
{
}

BEGIN_MESSAGE_MAP(CGoldAttendanceBookPopup, LSY::CLSYLibDialogBase)
	ON_WM_TIMER()
END_MESSAGE_MAP()

INT_PTR CGoldAttendanceBookPopup::DoModal(CWnd *pParent)
{
	m_pParentWnd = pParent;
	return CLSYLibDialogBase::DoModal(GetObjectMan(), ID_GOLDATTENDANCEBOOKPOPUP);
}

bool CGoldAttendanceBookPopup::OnCreatedProject()
{
	// LSY Control 연결
	InitLSYControl();
	// 데이터 세팅
	SetData(m_Round, true);
	
	//게임시작을 요청한다.
	CString authentiCode;
	MM().Call(UPG_CMK_GET_AUTHENTICODE_CAPTCHA, (WPARAM)(&authentiCode));

	CCL_PM_ODDEVEN_REQUEST_START msg;
	const char* captchaKey = authentiCode;
	msg.Set(captchaKey);
	NMBASE::SOCK::GSOCKMAN().SendData(g_strMainServerName.GetBuffer(), msg.pData, msg.GetTotalSize());
	
	CardOut();
	
	return TRUE;
}

void CGoldAttendanceBookPopup::OnCancel()
{
	// 게임이 진행 중 일 경우 닫기 안됨.
	if (m_bInProgress)
	{
		ShowMainMessageDlg("게임 진행 중에는 종료할 수 없습니다.");
		
		return;
	}
	
	bool bClose = true;
	
	// 홀짝 게임 진행 가능한 상황일 경우, 확인 팝업을 띄운다
	if (m_bContinue)
	{
		// 종료 팝업
		bool bModaled = m_popGoldAttendanceBookSub.GetSafeHwnd();
		
		// 팝업 종류 설정
		// 1 : 성공!, 2 : 실패!, 3 : 최종 성공, 4 : 종료
		m_popGoldAttendanceBookSub.SetPopupType(4, m_RoundReward, bModaled);
		
		if (bModaled == false)
			m_popGoldAttendanceBookSub.DoModal(this);
		
		bClose = m_popGoldAttendanceBookSub.GetCloseParent();
		
		if (bClose)
		{
			m_bContinue = false;
			
			//홀이냐, 짝이냐 유저의 배팅이다.
			CCL_PM_ODDEVEN_GAME_GIVEUP msg;
			msg.Set();
			NMBASE::SOCK::GSOCKMAN().SendData(g_strMainServerName.GetBuffer(), msg.pData, msg.GetTotalSize());
		}
	}
	
	if (bClose)
	{
		MM().Call(UPG_CMK_SET_LOBBY_BUTTON_ENABLE, (WPARAM)false);
		// MM().Call(UPG_CMK_SET_LOBBY_BUTTON_ENABLE, (WPARAM)true);
		
		//CString csMsg;
		//csMsg.Format("%I64d 골드 획득!", m_CurrentReward);
		//ShowMainMessageDlg(csMsg);
		
		// 현재 골드 출석부 결과 연출중인지 여부.
		GM().SetPlayDailyGoldEffect(false);

		// 한도 초과 팝업을 딜레이 시켜야 할 경우
		if (GM().GetWaitOwnLimitPopup() == true)
		{
			MM().Call(UPG_CMK_SHOW_GOLD_OWNLIMIT_POPUP);
		}
		
		MM().Call(UPG_CMK_SET_MONEY_NUM);
		
		InitLSYControlToNULL(); // LSY Control NULL 값으로 초기화

		__super::OnCancel();
	}
}

/// <summary>
/// 타이머 함수 오버라이드
/// <summary>
/// <param name=""></param>
/// <returns></returns>
void CGoldAttendanceBookPopup::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	switch(nIDEvent)
	{
	case TIMER_CARD_OUT:
		{
			KillTimer(TIMER_CARD_OUT);
			
			// 히든 카드 노출 사운드 - 착!
			NMBASE::SOUND::g_Sound.PlayWav(".\\Lobby\\sound\\GoldDaily\\fx_Hidden_card.wav");

			m_pImgCard->SetShow(true);
			
			m_pBtnOdd->SetEnable(true);
			m_pBtnEven->SetEnable(true);
			if (m_Round_Cur >= 7)
			{
				m_pBtnOdd->SetShow(true);
				m_pBtnEven->SetShow(true);
			}
			
			m_bInProgress = false;
			m_pBtnClose->SetEnable(true);
			
			m_pSprOdd->SetShow(true);
			m_pSprEven->SetShow(true);
		}
		break;
	case TIMER_SUCCESS_ANI1:
		{
			KillTimer(TIMER_SUCCESS_ANI1);
		}
		break;
	case TIMER_SUCCESS_ANI2:
		{
			KillTimer(TIMER_SUCCESS_ANI2);
			
			// 카드 오픈 사운드 - 탁
			NMBASE::SOUND::g_Sound.PlayWav(".\\Lobby\\sound\\GoldDaily\\fx_Card open.wav");
			
			SetTimer(TIMER_GAME_RESULT, 2000, NULL);
			
			int nPlayIdx = -1;
			int nCardTrackIdx = -1;
			int nCardIdx = GetPokerSprIndex(m_CardNum); // 결과 포커 인덱스
			int nOddEvenTrackIdx = -1;
			int nOddEvenIdx = ((nCardIdx % 2) == 0 ? 79 : 77); // 77 : 홀 성공, 79 : 짝 성공
			
			if (m_Round_Cur <= 3)
			{
				if (nOddEvenIdx == 77)
					nPlayIdx = 19;
				else
					nPlayIdx = 20;
				
				nCardTrackIdx = 3;
			}
			else if (m_Round_Cur <= 6)
			{
				if (nOddEvenIdx == 77)
					nPlayIdx = 21;
				else
					nPlayIdx = 22;
				
				nCardTrackIdx = 4;
			}
			else
			{
				nPlayIdx = 10;
				nCardTrackIdx = 6;
				nOddEvenTrackIdx = 17;
			}
			
			m_pSprCard->SetPlayIndex(nPlayIdx);
			m_pSprCard->ChangeMultiScene(nCardTrackIdx, nCardIdx); // 카드
			if (nOddEvenTrackIdx >= 0)
				m_pSprCard->ChangeMultiScene(nOddEvenTrackIdx, nOddEvenIdx); // 홀 성공 or 짝 성공
			m_pSprCard->SetRepeatCount(1);
			m_pSprCard->StartAnimation();
			
			// 당첨! + 폭죽
			if (m_Round_Cur == 7)
			{
				m_pSprSuccess->SetPlayIndex(12);
				m_pSprSuccess->SetRepeatCount(1);
				m_pSprSuccess->StartAnimation();
			}
			// 당첨! 1천만골드 획득 + 폭죽
			else if (m_Round_Cur == 8)
			{
				m_pSprSuccess->SetPlayIndex(13);
				m_pSprSuccess->SetRepeatCount(1);
				m_pSprSuccess->StartAnimation();
			}
		}
		break;
	case TIMER_SUCCESS_DLG:
		{
			KillTimer(TIMER_SUCCESS_DLG);
			
			m_pSprSuccess->SetShow(false);
			
			// 성공! 팝업
			bool bModaled = m_popGoldAttendanceBookSub.GetSafeHwnd();
			
			if (m_Round_Cur == 8)
			{
				// 팝업 종류 설정
				// 1 : 성공!, 2 : 실패!, 3 : 최종 성공, 4 : 종료
				m_popGoldAttendanceBookSub.SetPopupType(3, m_RoundReward, bModaled);
				
				if (bModaled == false)
					m_popGoldAttendanceBookSub.DoModal(this);
				
				m_bInProgress = false;
				CloseDlg(false);
				
				break;
			}
			
			// 팝업 종류 설정
			// 1 : 성공!, 2 : 실패!, 3 : 최종 성공, 4 : 종료
			m_popGoldAttendanceBookSub.SetPopupType(1, m_RoundReward, bModaled);
			
			if (bModaled == false)
				m_popGoldAttendanceBookSub.DoModal(this);
			
			m_pSprCard->SetShow(false);
			//m_pImgCard->SetShow(true);
			CardOut();
			
			SetData(m_Round, true);
		}
		break;
	case TIMER_FAIL_ANI1:
		{
			KillTimer(TIMER_FAIL_ANI1);
		}
		break;
	case TIMER_FAIL_ANI2:
		{
			KillTimer(TIMER_FAIL_ANI2);
			
			// 카드 오픈 사운드 - 탁
			NMBASE::SOUND::g_Sound.PlayWav(".\\Lobby\\sound\\GoldDaily\\fx_Card open.wav");
			SetTimer(TIMER_GAME_RESULT, 2000, NULL);
			
			int nPlayIdx = -1;
			int nCardTrackIdx = -1;
			int nCardIdx = GetPokerSprIndex(m_CardNum); // 결과 포커 인덱스
			int nOddEvenTrackIdx = -1;
			int nOddEvenIdx = ((nCardIdx % 2) == 0 ? 80 : 78); // 78 : 홀 실패, 80 : 짝 실패
			
			if (m_Round_Cur <= 6)
			{
				if (nOddEvenIdx == 78)
					nPlayIdx = 23;
				else
					nPlayIdx = 24;
				
				nCardTrackIdx = 3;
			}
			else
			{
				nPlayIdx = 11;
				nCardTrackIdx = 5;
				nOddEvenTrackIdx = 15;
			}
			
			m_pSprCard->SetPlayIndex(nPlayIdx);
			m_pSprCard->ChangeMultiScene(nCardTrackIdx, nCardIdx); // 카드
			if (nOddEvenTrackIdx >= 0)
				m_pSprCard->ChangeMultiScene(nOddEvenTrackIdx, nOddEvenIdx); // 홀 성공 or 짝 성공
			m_pSprCard->SetRepeatCount(1);
			m_pSprCard->StartAnimation();
		}
		break;
	case TIMER_FAIL_DLG:
		{
			KillTimer(TIMER_FAIL_DLG);
			
			// 실패! 팝업
			bool bModaled = m_popGoldAttendanceBookSub.GetSafeHwnd();
			
			// 팝업 종류 설정
			// 1 : 성공!, 2 : 실패!, 3 : 최종 성공, 4 : 종료
			m_popGoldAttendanceBookSub.SetPopupType(2, m_CurrentReward, bModaled);
			
			if (bModaled == false)
				m_popGoldAttendanceBookSub.DoModal(this);
			
			m_bInProgress = false;
			CloseDlg(false);
		}
		break;
	// 당첨 or 꽝
	case TIMER_GAME_RESULT:
		{
			KillTimer(TIMER_GAME_RESULT);
			
			if (m_BetSuccess)
			{
				if (m_Round_Cur <= 3)
				{
					// 당첨 시 사운드 - 찌잉
					NMBASE::SOUND::g_Sound.PlayWav(".\\Lobby\\sound\\GoldDaily\\fx_Card win_1.wav");
				}
				else if (m_Round_Cur <= 6)
				{
					// 당첨 시 사운드 - 찌잉~ 찡!
					NMBASE::SOUND::g_Sound.PlayWav(".\\Lobby\\sound\\GoldDaily\\fx_Card win_2.wav");
				}
				else if (m_Round_Cur == 7)
				{
					// 당첨 시 사운드
					NMBASE::SOUND::g_Sound.PlayWav(".\\Lobby\\sound\\GoldDaily\\fx_Card win_3.wav");
				}
				else if (m_Round_Cur == 8)
				{
					// 당첨 시 사운드
					NMBASE::SOUND::g_Sound.PlayWav(".\\Lobby\\sound\\GoldDaily\\fx_Card win_4.wav");
				}
			}
			else
			{
				if (m_Round_Cur <= 6)
				{
					// 꽝 사운드 - 휘잉~
					NMBASE::SOUND::g_Sound.PlayWav(".\\Lobby\\sound\\GoldDaily\\fx_wind_1.wav");
				}
				else
				{
					// 꽝 사운드 - 휘이잉~
					NMBASE::SOUND::g_Sound.PlayWav(".\\Lobby\\sound\\GoldDaily\\fx_wind_2.wav");
				}
			}
		}
		break;
	case TIMER_WINNER_NOTI:
		{
			KillTimer(TIMER_WINNER_NOTI);
			
			if (m_pTxtWinNum == NULL)
				break;

			m_pTxtWinNum->SetShow(true);
		}
		break;
	case TIMER_WINNER_NOTI_END:
		{
			KillTimer(TIMER_WINNER_NOTI_END);
			
			if (m_pTxtWinNum == NULL)
				break;
			
			m_pTxtWinNum->SetShow(false);
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
LRESULT CGoldAttendanceBookPopup::OnClickButton(LSY::CMessage *pMsg)
{
	LSY::CObjectMessage *msg = (LSY::CObjectMessage *)pMsg;
	switch(msg->GetObject()->GetID())
	{
	// 홀 버튼
	case ID_GOLDATTENDANCEBOOKPOPUP_BTN_ODD:
		{
			// 짝수, 홀수 버튼 사운드 - 딸깍
			NMBASE::SOUND::g_Sound.PlayWav(".\\Lobby\\sound\\GoldDaily\\fx_button.wav");
			
			if (m_bContinue == false)
			{
				ShowMainMessageDlg("오늘은 더 이상 참여할 수 없습니다.");
				break;
			}
			
			if (m_Round < 1 || m_Round > 8)
			{
				ShowMainMessageDlg("알 수 없는 오류. 관리자에게 문의하시기 바랍니다.");
				break;
			}
			
			// 2018-07-06 어뷰징 테스트(for문)
			//for (int i = 0; i < 20; i++)
			//{
				//홀이냐, 짝이냐 유저의 배팅이다.
				CCL_PM_ODDEVEN_USER_BET msg;
				msg.Set(m_Round, PROMOTION_ODDEVEN_BET_SIDE_ODD);
				NMBASE::SOCK::GSOCKMAN().SendData(g_strMainServerName.GetBuffer(), msg.pData, msg.GetTotalSize());
			//}
			
			// 2018-07-06 버튼을 빠르게 여러번 클릭할 시 요청이 여러번 가는것 확인.
			// 요청을 보냈으면 더이상 클릭 못하도록 막음.
			m_bInProgress = true;

			m_pImgCard->SetShow(false);
			m_pSprCard->SetShow(true);
			
			m_pSprOdd->SetShow(false);
			m_pSprEven->SetShow(false);
			
			m_pBtnOdd->SetEnable(false);
			m_pBtnEven->SetEnable(false);
		}
		break;
	// 짝 버튼
	case ID_GOLDATTENDANCEBOOKPOPUP_BTN_EVEN:
		{
			// 짝수, 홀수 버튼 사운드 - 딸깍
			NMBASE::SOUND::g_Sound.PlayWav(".\\Lobby\\sound\\GoldDaily\\fx_button.wav");
			
			if (m_bContinue == false)
			{
				ShowMainMessageDlg("오늘은 더 이상 참여할 수 없습니다.");
				break;
			}
			
			if (m_Round_Cur < 1 || m_Round_Cur > 8)
			{
				ShowMainMessageDlg("알 수 없는 오류. 관리자에게 문의하시기 바랍니다.");
				break;
			}
			
			// 2018-07-06 어뷰징 테스트
			//for (int i = 0; i < 20; i++)
			//{
				//홀이냐, 짝이냐 유저의 배팅이다.
				CCL_PM_ODDEVEN_USER_BET msg;
				msg.Set(m_Round, PROMOTION_ODDEVEN_BET_SIDE_EVEN);
				NMBASE::SOCK::GSOCKMAN().SendData(g_strMainServerName.GetBuffer(), msg.pData, msg.GetTotalSize());
			//}
			
			// 2018-07-06 버튼을 빠르게 여러번 클릭할 시 요청이 여러번 가는것 확인.
			// 요청을 보냈으면 더이상 클릭 못하도록 막음.
			m_bInProgress = true;

			m_pImgCard->SetShow(false);
			m_pSprCard->SetShow(true);
			
			m_pSprOdd->SetShow(false);
			m_pSprEven->SetShow(false);
			
			m_pBtnOdd->SetEnable(false);
			m_pBtnEven->SetEnable(false);
		}
		break;
	// 닫기 버튼
	case ID_GOLDATTENDANCEBOOKPOPUP_BTN_CLOSE:
		{
			CloseDlg(m_bContinue);
		}
		break;
	}
	
	return TRUE;
}

/// <summary>
/// 마우스 오버
/// <summary>
/// <param name=""></param>
/// <returns></returns>
LRESULT CGoldAttendanceBookPopup::OnMouseOver(LSY::CMessage *pMsg)
{
	if (m_bInProgress)
	{
		m_pSprOdd->SetShow(false);
		m_pSprEven->SetShow(false);

		return TRUE;
	}
	
	LSY::CObjectMessage *msg = (LSY::CObjectMessage *)pMsg;
	switch(msg->GetObject()->GetID())
	{
	// 홀 스프라이트
	case ID_GOLDATTENDANCEBOOKPOPUP_SPR_ODD:
		{
			m_pSprOdd->SetShow(false);
		}
		break;
	// 짝 스프라이트
	case ID_GOLDATTENDANCEBOOKPOPUP_SPR_EVEN:
		{
			m_pSprEven->SetShow(false);
		}
		break;
	}
	
	return TRUE;
}

/// <summary>
/// 마우스 리브
/// <summary>
/// <param name=""></param>
/// <returns></returns>
LRESULT CGoldAttendanceBookPopup::OnMouseLeave(LSY::CMessage *pMsg)
{
	if (m_bInProgress)
	{
		m_pSprOdd->SetShow(false);
		m_pSprEven->SetShow(false);
		return TRUE;
	}
	
	LSY::CObjectMessage *msg = (LSY::CObjectMessage *)pMsg;
	switch(msg->GetObject()->GetID())
	{
	// 홀 버튼
	case ID_GOLDATTENDANCEBOOKPOPUP_BTN_ODD:
		{
			m_pSprOdd->SetShow(true);
		}
		break;
	// 짝 버튼
	case ID_GOLDATTENDANCEBOOKPOPUP_BTN_EVEN:
		{
			m_pSprEven->SetShow(true);
		}
		break;
	}
	
	return TRUE;
}

/// <summary>
/// 마우스 무브
/// <summary>
/// <param name=""></param>
/// <returns></returns>
LRESULT CGoldAttendanceBookPopup::OnMouseMove(LSY::CMessage *pMsg)
{
	LSY::CObjectMessage *msg = (LSY::CObjectMessage *)pMsg;
	switch(msg->GetObject()->GetID())
	{
	// 홀 버튼
	case ID_GOLDATTENDANCEBOOKPOPUP_BTN_ODD:
		{

		}
		break;
	// 짝 버튼
	case ID_GOLDATTENDANCEBOOKPOPUP_BTN_EVEN:
		{

		}
		break;
	// 홀 spr
	case ID_GOLDATTENDANCEBOOKPOPUP_SPR_ODD:
		{

		}
		break;
	// 짝 spr
	case ID_GOLDATTENDANCEBOOKPOPUP_SPR_EVEN:
		{

		}
		break;
	}
	
	return FALSE;
}

/// <summary>
/// 마우스 왼쪽 버튼 다운
/// <summary>
/// <param name=""></param>
/// <returns></returns>
LRESULT CGoldAttendanceBookPopup::OnLButtonDown(LSY::CMessage *pMsg)
{
	LSY::CObjectMessage *msg = (LSY::CObjectMessage *)pMsg;
	switch(msg->GetObject()->GetID())
	{
	// 홀 버튼
	case ID_GOLDATTENDANCEBOOKPOPUP_BTN_ODD:
		{
		}
		break;
	// 짝 버튼
	case ID_GOLDATTENDANCEBOOKPOPUP_BTN_EVEN:
		{
		}
		break;
	}
	
	return TRUE;
}

/// <summary>
/// 마우스 왼쪽 버튼 업
/// <summary>
/// <param name=""></param>
/// <returns></returns>
LRESULT CGoldAttendanceBookPopup::OnLButtonUp(LSY::CMessage *pMsg)
{
	LSY::CObjectMessage *msg = (LSY::CObjectMessage *)pMsg;
	switch(msg->GetObject()->GetID())
	{
	// 홀 스프라이트
	case ID_GOLDATTENDANCEBOOKPOPUP_SPR_ODD:
		{
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
void CGoldAttendanceBookPopup::InitLSYControl()
{
	// 배경
	m_pSprBack = (LSY::CSprite *)GetObject(ID_GOLDATTENDANCEBOOKPOPUP_SPR_BACK);

	// 버튼 비활성화 spr 이미지
	m_pSprOdd = (LSY::CSprite *)GetObject(ID_GOLDATTENDANCEBOOKPOPUP_SPR_ODD);
	m_pSprOdd->AddHandler(LSY::EM_O_MOUSEOVER, LSY::Fnt(this, &CGoldAttendanceBookPopup::OnMouseOver));
	m_pSprOdd->AddHandler(LSY::EM_O_MOUSELEAVE, LSY::Fnt(this, &CGoldAttendanceBookPopup::OnMouseLeave));
	m_pSprOdd->AddHandler(LSY::EM_O_MOUSEMOVE, LSY::Fnt(this, &CGoldAttendanceBookPopup::OnMouseMove));
	//m_pSprOdd->AddHandler(LSY::EM_O_LBUTTONDOWN, LSY::Fnt(this, &CGoldAttendanceBookPopup::OnLButtonDown));
	//m_pSprOdd->AddHandler(LSY::EM_O_LBUTTONUP, LSY::Fnt(this, &CGoldAttendanceBookPopup::OnLButtonUp));
	m_pSprOdd->SetShow(false);
	m_pSprEven = (LSY::CSprite *)GetObject(ID_GOLDATTENDANCEBOOKPOPUP_SPR_EVEN);
	m_pSprEven->AddHandler(LSY::EM_O_MOUSEOVER, LSY::Fnt(this, &CGoldAttendanceBookPopup::OnMouseOver));
	m_pSprEven->AddHandler(LSY::EM_O_MOUSELEAVE, LSY::Fnt(this, &CGoldAttendanceBookPopup::OnMouseLeave));
	m_pSprEven->AddHandler(LSY::EM_O_MOUSEMOVE, LSY::Fnt(this, &CGoldAttendanceBookPopup::OnMouseMove));
	m_pSprEven->SetShow(false);
	
	m_pSprCard = (LSY::CSprite *)GetObject(ID_GOLDATTENDANCEBOOKPOPUP_SPR_CARD);
	m_pSprCard->SetShow(false);
	m_pSprSuccess = (LSY::CSprite *)GetObject(ID_GOLDATTENDANCEBOOKPOPUP_SPR_SUCCESS);
	m_pSprSuccess->SetShow(false);
	m_pSprNotice = (LSY::CSprite *)GetObject(ID_GOLDATTENDANCEBOOKPOPUP_SPR_NOTICE);
	
	m_pTxtWinNum = (LSY::CText *)GetObject(ID_GOLDATTENDANCEBOOKPOPUP_TXT_WINNUM);
	m_pTxtWinNum->SetShow(false);
	m_pTxtGold = (LSY::CText *)GetObject(ID_GOLDATTENDANCEBOOKPOPUP_TXT_GOLD);
	m_pTxtWinGold = (LSY::CText *)GetObject(ID_GOLDATTENDANCEBOOKPOPUP_TXT_WINGOLD);
	m_pImgCard = (LSY::CImage *)GetObject(ID_GOLDATTENDANCEBOOKPOPUP_IMG_CARD);
	m_pImgCard->SetShow(false);
	
	// 홀 버튼
	m_pBtnOdd = (LSY::CButton *)GetObject(ID_GOLDATTENDANCEBOOKPOPUP_BTN_ODD);
	m_pBtnOdd->AddHandler(LSY::EM_O_LBUTTONDOWN, LSY::Fnt(this, &CGoldAttendanceBookPopup::OnLButtonDown));
	m_pBtnOdd->AddHandler(LSY::EM_O_MOUSELCLICK, LSY::Fnt(this, &CGoldAttendanceBookPopup::OnClickButton));
	m_pBtnOdd->AddHandler(LSY::EM_O_MOUSELEAVE, LSY::Fnt(this, &CGoldAttendanceBookPopup::OnMouseLeave));
	m_pBtnOdd->AddHandler(LSY::EM_O_MOUSEMOVE, LSY::Fnt(this, &CGoldAttendanceBookPopup::OnMouseMove));
	m_pBtnOdd->SetEnable(false);
	
	// 짝 버튼
	m_pBtnEven = (LSY::CButton *)GetObject(ID_GOLDATTENDANCEBOOKPOPUP_BTN_EVEN);
	m_pBtnEven->AddHandler(LSY::EM_O_LBUTTONDOWN, LSY::Fnt(this, &CGoldAttendanceBookPopup::OnLButtonDown));
	m_pBtnEven->AddHandler(LSY::EM_O_MOUSELCLICK, LSY::Fnt(this, &CGoldAttendanceBookPopup::OnClickButton));
	m_pBtnEven->AddHandler(LSY::EM_O_MOUSELEAVE, LSY::Fnt(this, &CGoldAttendanceBookPopup::OnMouseLeave));
	m_pBtnEven->AddHandler(LSY::EM_O_MOUSEMOVE, LSY::Fnt(this, &CGoldAttendanceBookPopup::OnMouseMove));
	m_pBtnEven->SetEnable(false);

	// 닫기 버튼
	m_pBtnClose = (LSY::CButton *)GetObject(ID_GOLDATTENDANCEBOOKPOPUP_BTN_CLOSE);
	m_pBtnClose->AddHandler(LSY::EM_O_MOUSELCLICK, LSY::Fnt(this, &CGoldAttendanceBookPopup::OnClickButton));
	m_pBtnClose->SetEnable(false);
	
}

/// <summary>
/// LSY Control NULL 값으로 초기화.
/// 생성자, OnClose 함수에서 호출.
/// <summary>
/// <param name=""></param>
/// <returns></returns>
void CGoldAttendanceBookPopup::InitLSYControlToNULL()
{
	// LSY 컨트롤 변수
	m_pSprBack = NULL;
	m_pSprOdd = NULL;
	m_pSprEven = NULL;
	m_pSprCard = NULL;
	m_pSprSuccess = NULL;
	m_pSprNotice = NULL;
	m_pTxtWinNum = NULL;
	m_pTxtGold = NULL;
	m_pTxtWinGold = NULL;
	m_pImgCard = NULL;
	m_pBtnOdd = NULL;
	m_pBtnEven = NULL;
	m_pBtnClose = NULL;
}

/// <summary>
/// 데이터 세팅
/// <summary>
/// <param name=""></param>
/// <returns></returns>
void CGoldAttendanceBookPopup::SetData(int nRound, bool bShowNoti)
{
	if (m_pSprBack == NULL || m_pTxtGold == NULL || m_pTxtWinGold == NULL || m_pSprNotice == NULL)
		return;
	
	CString csCurReward;
	CString csWinReward;
	CString csCurRewardComma;
	CString csWinRewardComma;
	
	csCurReward.Format("%I64d", m_CurrentReward);
	csWinReward.Format("%I64d", m_WinReward);
	
	GetNumberWithComma(csCurReward, csCurRewardComma);
	GetNumberWithComma(csWinReward, csWinRewardComma);
	
	m_pSprNotice->SetShow(bShowNoti);

	if (bShowNoti)
	{
		m_pSprNotice->ChangeMultiScene(0, nRound + 8);
		m_pSprNotice->StartAnimation();
	}
	
	//m_pSprBack->SetPlayIndex(0);
	m_pSprBack->ChangeMultiScene(1, nRound);
	m_pSprBack->StartAnimation();
	m_pTxtGold->SetText((LPCTSTR)csCurRewardComma);
	m_pTxtWinGold->SetText((LPCTSTR)csWinRewardComma);
}

/// <summary>
/// 창 닫기
/// <summary>
/// <param name="bContinue">게임을 더 진행할 수 있는지 여부</param>
/// <returns></returns>
void CGoldAttendanceBookPopup::CloseDlg(bool bContinue)
{
	m_bContinue = bContinue;

	OnCancel();
}

/// <summary>
/// 서버에서 보내는 패킷(응답) 처리
/// <summary>
/// <param name=""></param>
/// <returns></returns>
void CGoldAttendanceBookPopup::HandleResponse(int Signal, int TotSize, char *lpData) // 서버 응답 핸들링
{
	switch(Signal)
	{
	//각 라운드가 진행되었음을 알려준다.
	//이 패킷을 받으면, 유저의 선택을 받아 홀 혹은 짝의 결과를 서버로 보내주면 된다.
	case SV_PM_ODDEVEN_START_ROUND:
		{
			CSV_PM_ODDEVEN_START_ROUND msg;
			msg.Get(lpData, TotSize);
			
			m_Round = *msg.m_Round; //단계 1 ~ 8까지 있음
			m_CurrentReward = *msg.m_CurrentReward; //지금까지 확보된 보상금액(배팅실패, 혹은 게임포기를 하면 받게 되는 보상금)
			m_WinReward = *msg.m_WinReward; //이번 라운드에서 배팅성공시 받게되는 보상금액이다.
			m_BetTime = *msg.m_BetTime; //배팅제한시간이다(초), 만일 -1로 가면 제한이 없다는 의미
			
			m_bContinue = true;
			
			if (m_Round == 1)
				SetData(m_Round, true);
		}
		break;
	//유저배팅에 대한 결과이다.
	//배팅에 실패했을경우 종료여부가 true로 가며, 성공하면 종료여부가 false 면서 CSV_PM_ODDEVEN_START_ROUND 패킷이 다시 전송됨
	case SV_PM_ODDEVEN_USER_BET_RESULT:
		{
			GM().SetPlayDailyGoldEffect(true);
			
			CSV_PM_ODDEVEN_USER_BET_RESULT msg;
			msg.Get(lpData, TotSize);
			
			SetData(m_Round, false);
			
			m_BetSuccess = *msg.m_BetSuccess; //성공여부
			m_Round_Cur = *msg.m_Round; //도전한 라운드
			m_CardNum = *msg.m_CardNum; //이번판 결과로 부여된 카드번호이다.(세븐포커 카드 인덱스와 동일함)
			m_RoundReward = *msg.m_RoundReward; //도전한 라운드에 배팅성공시 획득하게되는 보상금
			
			m_bInProgress = true;
			
			m_pImgCard->SetShow(false);
			m_pSprCard->SetShow(true);
			
			m_pSprOdd->SetShow(false);
			m_pSprEven->SetShow(false);
			
			m_pBtnOdd->SetEnable(false);
			m_pBtnEven->SetEnable(false);
			
			// 7라운드 부터는 버튼을 아예 감춘다.
			if (m_Round_Cur >= 7)
			{
				m_pBtnOdd->SetShow(false);
				m_pBtnEven->SetShow(false);
			}
			
			m_pBtnClose->SetEnable(false);
			
			if (m_BetSuccess)
			{
				DWORD dwPlayTime = 0;
				
				// 카드 오픈
				m_pSprCard->SetPlayIndex(10);
				m_pSprCard->SetRepeatCount(1);
				SetTimer(TIMER_SUCCESS_ANI2, dwPlayTime, NULL);
				// 성공 팝업 띄움
				dwPlayTime = m_pSprCard->GetTotalPlayTime();
				SetTimer(TIMER_SUCCESS_DLG, dwPlayTime, NULL);
			}
			// 실패
			else
			{
				DWORD dwPlayTime = 0;
				
				// 카드 오픈
				m_pSprCard->SetPlayIndex(11);
				m_pSprCard->SetRepeatCount(1);
				SetTimer(TIMER_FAIL_ANI2, dwPlayTime, NULL);
				// 실패 팝업 띄움
				dwPlayTime += m_pSprCard->GetTotalPlayTime();
				SetTimer(TIMER_FAIL_DLG, dwPlayTime, NULL);
			}
		}
		break;
	//당첨자를 전체 유저에게 노티해 준다~!
	case SV_PM_ODDEVEN_WINNER_NOTI:
		{
			if (m_pTxtWinNum == NULL)
				break;
			
			if (m_bContinue == false)
				break;
			
			CSV_PM_ODDEVEN_WINNER_NOTI msg;
			msg.Get(lpData, TotSize);
			
			m_WinRound = *msg.m_WinRound; //당첨 라운드
			m_RewardMoney = *msg.m_RewardMoney; //당첨금
			m_RoundWinner = *msg.m_RoundWinner; //해당 라운드 당첨자 수
			
			if (m_WinRound < 6 || m_WinRound > 8)
				break;
			
			CString csMsg;
			CString csRewardMoney;
			CString csRewardMoneyComma;
			
			csRewardMoney.Format("%I64d", m_RewardMoney);
			GetNumberWithComma(csRewardMoney, csRewardMoneyComma);
			
			if (m_WinRound == 8)
				csRewardMoneyComma = "1천만";
			else
				GetNumberWithComma(csRewardMoney, csRewardMoneyComma);
			
			csMsg.Format(
				"%d회차 최대 당첨금 %s%s   %d회차 당첨자 수 : %d명",
				m_WinRound, csRewardMoneyComma, strChip_Name[(int)ROOM_CHIPKIND_GOLD], m_WinRound, m_RoundWinner);
			
			m_pTxtWinNum->SetText((LPCTSTR)csMsg);
			
			SetTimer(TIMER_WINNER_NOTI, 2000, NULL);
			SetTimer(TIMER_WINNER_NOTI_END, 2000 + 10000, NULL);
		}
		break;
	}
}

void CGoldAttendanceBookPopup::Wait(DWORD dwMillisecond)
{
	MSG msg;
	DWORD dwStart;
	dwStart = GetTickCount();
	
	while(GetTickCount() - dwStart < dwMillisecond)
	{
		while(PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
}

/// <summary>
/// 카드 인덱스에 해당하는 spr track 인덱스를 구한다.
/// <summary>
/// <param name="">7포커 카드 인덱스</param>
/// <returns>spr track 인덱스</returns>
int CGoldAttendanceBookPopup::GetPokerSprIndex(int nCardNum)
{
	int nIdx = 0;

	// 클로버
	if (nCardNum >= 0 && nCardNum <= 12)
	{
		nIdx = (nCardNum + 1) % 13;
		nIdx += 61;
		
		return nIdx;
	}
	// 하트
	else if (nCardNum >= 13 && nCardNum <= 25)
	{
		nIdx = (nCardNum - 12) % 13;
		nIdx += 51;
		
		return nIdx;
	}
	// 다이아
	else if (nCardNum >= 26 && nCardNum <= 38)
	{
		nIdx = (nCardNum - 25) % 13;
		nIdx += 41;
		
		return nIdx;
	}
	// 스페이드
	else if (nCardNum >= 39 && nCardNum <= 51)
	{
		nIdx = (nCardNum - 38) % 13;
		nIdx += 31;
		
		return nIdx;
	}

	return nIdx;
}

/// <summary>
/// 세 자리 수 마다 콤마 찍어주기
/// <summary>
/// <param name="nType"></param>
/// <returns></returns>
void CGoldAttendanceBookPopup::GetNumberWithComma(CString parm_data, CString &parm_string)
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
/// 이 함수는 사용하면 안됨. 컴퓨터에 따라 숫자가 깨지는 현상 발생.
/// <summary>
/// <param name="nType">성공시 획득 골드</param>
/// <returns></returns>
void CGoldAttendanceBookPopup::GetNumberWithComma2(INT64 nNum, char cBuffer[])
{
	CString csNum;
	csNum.Format(_T("%I64d"), nNum);
	NUMBERFMT fmt = {0, 0, 3, ".", ",", 1};
	::GetNumberFormat(NULL, NULL, csNum, &fmt, cBuffer, strlen(cBuffer));
}

/// <summary>
/// 카드 사출
/// <summary>
/// <param name=""></param>
/// <returns></returns>
// 카드 사출
void CGoldAttendanceBookPopup::CardOut()
{
	// 카드 사출
	DWORD dwPlayTime = 0;
	m_pSprCard->SetPlayIndex(18);
	m_pSprCard->SetRepeatCount(1);
	dwPlayTime = m_pSprCard->GetTotalPlayTime();
	SetTimer(TIMER_CARD_OUT, dwPlayTime, NULL);
	m_pSprCard->StartAnimation();
	
	// 카드 사출 사운드 - 탁!
	NMBASE::SOUND::g_Sound.PlayWav(".\\Lobby\\sound\\GoldDaily\\fx_card_slide.wav");
}