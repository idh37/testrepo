#include "StdAfx.h"
#include "GoldAttendanceBookPopup.h"
#include "goldattendancebookpopupid.h"

CGoldAttendanceBookPopup::CGoldAttendanceBookPopup(CWnd* pParent/* = NULL*/) : LSY::CLSYLibDialogBase(pParent)
{
	InitLSYControlToNULL(); // LSY Control NULL ������ �ʱ�ȭ

	// ��Ÿ ����
	m_Round_Cur = 1; //�ܰ� 1 ~ 8���� ����
	m_CurrentReward = 0; //���ݱ��� Ȯ���� ����ݾ�(���ý���, Ȥ�� �������⸦ �ϸ� �ް� �Ǵ� �����)
	m_WinReward = 0; //�̹� ���忡�� ���ü����� �ްԵǴ� ����ݾ��̴�.
	m_BetTime = 0; //�������ѽð��̴�(��), ���� -1�� ���� ������ ���ٴ� �ǹ�
	
	m_BetSuccess = false; //��������
	m_Round = 1; //������ ����
	m_CardNum = 0; //�̹��� ����� �ο��� ī���ȣ�̴�.(������Ŀ ī�� �ε����� ������)
	m_RoundReward = 0; //������ ���忡 ���ü����� ȹ���ϰԵǴ� �����
	
	m_bContinue = false; // ���� ���� ���� ����
	m_bInProgress = true; // ���� ���� ������ ����
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
	// LSY Control ����
	InitLSYControl();
	// ������ ����
	SetData(m_Round, true);
	
	//���ӽ����� ��û�Ѵ�.
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
	// ������ ���� �� �� ��� �ݱ� �ȵ�.
	if (m_bInProgress)
	{
		ShowMainMessageDlg("���� ���� �߿��� ������ �� �����ϴ�.");
		
		return;
	}
	
	bool bClose = true;
	
	// Ȧ¦ ���� ���� ������ ��Ȳ�� ���, Ȯ�� �˾��� ����
	if (m_bContinue)
	{
		// ���� �˾�
		bool bModaled = m_popGoldAttendanceBookSub.GetSafeHwnd();
		
		// �˾� ���� ����
		// 1 : ����!, 2 : ����!, 3 : ���� ����, 4 : ����
		m_popGoldAttendanceBookSub.SetPopupType(4, m_RoundReward, bModaled);
		
		if (bModaled == false)
			m_popGoldAttendanceBookSub.DoModal(this);
		
		bClose = m_popGoldAttendanceBookSub.GetCloseParent();
		
		if (bClose)
		{
			m_bContinue = false;
			
			//Ȧ�̳�, ¦�̳� ������ �����̴�.
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
		//csMsg.Format("%I64d ��� ȹ��!", m_CurrentReward);
		//ShowMainMessageDlg(csMsg);
		
		// ���� ��� �⼮�� ��� ���������� ����.
		GM().SetPlayDailyGoldEffect(false);

		// �ѵ� �ʰ� �˾��� ������ ���Ѿ� �� ���
		if (GM().GetWaitOwnLimitPopup() == true)
		{
			MM().Call(UPG_CMK_SHOW_GOLD_OWNLIMIT_POPUP);
		}
		
		MM().Call(UPG_CMK_SET_MONEY_NUM);
		
		InitLSYControlToNULL(); // LSY Control NULL ������ �ʱ�ȭ

		__super::OnCancel();
	}
}

/// <summary>
/// Ÿ�̸� �Լ� �������̵�
/// <summary>
/// <param name=""></param>
/// <returns></returns>
void CGoldAttendanceBookPopup::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
	switch(nIDEvent)
	{
	case TIMER_CARD_OUT:
		{
			KillTimer(TIMER_CARD_OUT);
			
			// ���� ī�� ���� ���� - ��!
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
			
			// ī�� ���� ���� - Ź
			NMBASE::SOUND::g_Sound.PlayWav(".\\Lobby\\sound\\GoldDaily\\fx_Card open.wav");
			
			SetTimer(TIMER_GAME_RESULT, 2000, NULL);
			
			int nPlayIdx = -1;
			int nCardTrackIdx = -1;
			int nCardIdx = GetPokerSprIndex(m_CardNum); // ��� ��Ŀ �ε���
			int nOddEvenTrackIdx = -1;
			int nOddEvenIdx = ((nCardIdx % 2) == 0 ? 79 : 77); // 77 : Ȧ ����, 79 : ¦ ����
			
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
			m_pSprCard->ChangeMultiScene(nCardTrackIdx, nCardIdx); // ī��
			if (nOddEvenTrackIdx >= 0)
				m_pSprCard->ChangeMultiScene(nOddEvenTrackIdx, nOddEvenIdx); // Ȧ ���� or ¦ ����
			m_pSprCard->SetRepeatCount(1);
			m_pSprCard->StartAnimation();
			
			// ��÷! + ����
			if (m_Round_Cur == 7)
			{
				m_pSprSuccess->SetPlayIndex(12);
				m_pSprSuccess->SetRepeatCount(1);
				m_pSprSuccess->StartAnimation();
			}
			// ��÷! 1õ����� ȹ�� + ����
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
			
			// ����! �˾�
			bool bModaled = m_popGoldAttendanceBookSub.GetSafeHwnd();
			
			if (m_Round_Cur == 8)
			{
				// �˾� ���� ����
				// 1 : ����!, 2 : ����!, 3 : ���� ����, 4 : ����
				m_popGoldAttendanceBookSub.SetPopupType(3, m_RoundReward, bModaled);
				
				if (bModaled == false)
					m_popGoldAttendanceBookSub.DoModal(this);
				
				m_bInProgress = false;
				CloseDlg(false);
				
				break;
			}
			
			// �˾� ���� ����
			// 1 : ����!, 2 : ����!, 3 : ���� ����, 4 : ����
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
			
			// ī�� ���� ���� - Ź
			NMBASE::SOUND::g_Sound.PlayWav(".\\Lobby\\sound\\GoldDaily\\fx_Card open.wav");
			SetTimer(TIMER_GAME_RESULT, 2000, NULL);
			
			int nPlayIdx = -1;
			int nCardTrackIdx = -1;
			int nCardIdx = GetPokerSprIndex(m_CardNum); // ��� ��Ŀ �ε���
			int nOddEvenTrackIdx = -1;
			int nOddEvenIdx = ((nCardIdx % 2) == 0 ? 80 : 78); // 78 : Ȧ ����, 80 : ¦ ����
			
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
			m_pSprCard->ChangeMultiScene(nCardTrackIdx, nCardIdx); // ī��
			if (nOddEvenTrackIdx >= 0)
				m_pSprCard->ChangeMultiScene(nOddEvenTrackIdx, nOddEvenIdx); // Ȧ ���� or ¦ ����
			m_pSprCard->SetRepeatCount(1);
			m_pSprCard->StartAnimation();
		}
		break;
	case TIMER_FAIL_DLG:
		{
			KillTimer(TIMER_FAIL_DLG);
			
			// ����! �˾�
			bool bModaled = m_popGoldAttendanceBookSub.GetSafeHwnd();
			
			// �˾� ���� ����
			// 1 : ����!, 2 : ����!, 3 : ���� ����, 4 : ����
			m_popGoldAttendanceBookSub.SetPopupType(2, m_CurrentReward, bModaled);
			
			if (bModaled == false)
				m_popGoldAttendanceBookSub.DoModal(this);
			
			m_bInProgress = false;
			CloseDlg(false);
		}
		break;
	// ��÷ or ��
	case TIMER_GAME_RESULT:
		{
			KillTimer(TIMER_GAME_RESULT);
			
			if (m_BetSuccess)
			{
				if (m_Round_Cur <= 3)
				{
					// ��÷ �� ���� - ����
					NMBASE::SOUND::g_Sound.PlayWav(".\\Lobby\\sound\\GoldDaily\\fx_Card win_1.wav");
				}
				else if (m_Round_Cur <= 6)
				{
					// ��÷ �� ���� - ����~ ��!
					NMBASE::SOUND::g_Sound.PlayWav(".\\Lobby\\sound\\GoldDaily\\fx_Card win_2.wav");
				}
				else if (m_Round_Cur == 7)
				{
					// ��÷ �� ����
					NMBASE::SOUND::g_Sound.PlayWav(".\\Lobby\\sound\\GoldDaily\\fx_Card win_3.wav");
				}
				else if (m_Round_Cur == 8)
				{
					// ��÷ �� ����
					NMBASE::SOUND::g_Sound.PlayWav(".\\Lobby\\sound\\GoldDaily\\fx_Card win_4.wav");
				}
			}
			else
			{
				if (m_Round_Cur <= 6)
				{
					// �� ���� - ����~
					NMBASE::SOUND::g_Sound.PlayWav(".\\Lobby\\sound\\GoldDaily\\fx_wind_1.wav");
				}
				else
				{
					// �� ���� - ������~
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
/// ��ư Ŭ�� �̺�Ʈ. ��κ� ������ ��û ������ ���.
/// <summary>
/// <param name=""></param>
/// <returns></returns>
LRESULT CGoldAttendanceBookPopup::OnClickButton(LSY::CMessage *pMsg)
{
	LSY::CObjectMessage *msg = (LSY::CObjectMessage *)pMsg;
	switch(msg->GetObject()->GetID())
	{
	// Ȧ ��ư
	case ID_GOLDATTENDANCEBOOKPOPUP_BTN_ODD:
		{
			// ¦��, Ȧ�� ��ư ���� - ����
			NMBASE::SOUND::g_Sound.PlayWav(".\\Lobby\\sound\\GoldDaily\\fx_button.wav");
			
			if (m_bContinue == false)
			{
				ShowMainMessageDlg("������ �� �̻� ������ �� �����ϴ�.");
				break;
			}
			
			if (m_Round < 1 || m_Round > 8)
			{
				ShowMainMessageDlg("�� �� ���� ����. �����ڿ��� �����Ͻñ� �ٶ��ϴ�.");
				break;
			}
			
			// 2018-07-06 ���¡ �׽�Ʈ(for��)
			//for (int i = 0; i < 20; i++)
			//{
				//Ȧ�̳�, ¦�̳� ������ �����̴�.
				CCL_PM_ODDEVEN_USER_BET msg;
				msg.Set(m_Round, PROMOTION_ODDEVEN_BET_SIDE_ODD);
				NMBASE::SOCK::GSOCKMAN().SendData(g_strMainServerName.GetBuffer(), msg.pData, msg.GetTotalSize());
			//}
			
			// 2018-07-06 ��ư�� ������ ������ Ŭ���� �� ��û�� ������ ���°� Ȯ��.
			// ��û�� �������� ���̻� Ŭ�� ���ϵ��� ����.
			m_bInProgress = true;

			m_pImgCard->SetShow(false);
			m_pSprCard->SetShow(true);
			
			m_pSprOdd->SetShow(false);
			m_pSprEven->SetShow(false);
			
			m_pBtnOdd->SetEnable(false);
			m_pBtnEven->SetEnable(false);
		}
		break;
	// ¦ ��ư
	case ID_GOLDATTENDANCEBOOKPOPUP_BTN_EVEN:
		{
			// ¦��, Ȧ�� ��ư ���� - ����
			NMBASE::SOUND::g_Sound.PlayWav(".\\Lobby\\sound\\GoldDaily\\fx_button.wav");
			
			if (m_bContinue == false)
			{
				ShowMainMessageDlg("������ �� �̻� ������ �� �����ϴ�.");
				break;
			}
			
			if (m_Round_Cur < 1 || m_Round_Cur > 8)
			{
				ShowMainMessageDlg("�� �� ���� ����. �����ڿ��� �����Ͻñ� �ٶ��ϴ�.");
				break;
			}
			
			// 2018-07-06 ���¡ �׽�Ʈ
			//for (int i = 0; i < 20; i++)
			//{
				//Ȧ�̳�, ¦�̳� ������ �����̴�.
				CCL_PM_ODDEVEN_USER_BET msg;
				msg.Set(m_Round, PROMOTION_ODDEVEN_BET_SIDE_EVEN);
				NMBASE::SOCK::GSOCKMAN().SendData(g_strMainServerName.GetBuffer(), msg.pData, msg.GetTotalSize());
			//}
			
			// 2018-07-06 ��ư�� ������ ������ Ŭ���� �� ��û�� ������ ���°� Ȯ��.
			// ��û�� �������� ���̻� Ŭ�� ���ϵ��� ����.
			m_bInProgress = true;

			m_pImgCard->SetShow(false);
			m_pSprCard->SetShow(true);
			
			m_pSprOdd->SetShow(false);
			m_pSprEven->SetShow(false);
			
			m_pBtnOdd->SetEnable(false);
			m_pBtnEven->SetEnable(false);
		}
		break;
	// �ݱ� ��ư
	case ID_GOLDATTENDANCEBOOKPOPUP_BTN_CLOSE:
		{
			CloseDlg(m_bContinue);
		}
		break;
	}
	
	return TRUE;
}

/// <summary>
/// ���콺 ����
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
	// Ȧ ��������Ʈ
	case ID_GOLDATTENDANCEBOOKPOPUP_SPR_ODD:
		{
			m_pSprOdd->SetShow(false);
		}
		break;
	// ¦ ��������Ʈ
	case ID_GOLDATTENDANCEBOOKPOPUP_SPR_EVEN:
		{
			m_pSprEven->SetShow(false);
		}
		break;
	}
	
	return TRUE;
}

/// <summary>
/// ���콺 ����
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
	// Ȧ ��ư
	case ID_GOLDATTENDANCEBOOKPOPUP_BTN_ODD:
		{
			m_pSprOdd->SetShow(true);
		}
		break;
	// ¦ ��ư
	case ID_GOLDATTENDANCEBOOKPOPUP_BTN_EVEN:
		{
			m_pSprEven->SetShow(true);
		}
		break;
	}
	
	return TRUE;
}

/// <summary>
/// ���콺 ����
/// <summary>
/// <param name=""></param>
/// <returns></returns>
LRESULT CGoldAttendanceBookPopup::OnMouseMove(LSY::CMessage *pMsg)
{
	LSY::CObjectMessage *msg = (LSY::CObjectMessage *)pMsg;
	switch(msg->GetObject()->GetID())
	{
	// Ȧ ��ư
	case ID_GOLDATTENDANCEBOOKPOPUP_BTN_ODD:
		{

		}
		break;
	// ¦ ��ư
	case ID_GOLDATTENDANCEBOOKPOPUP_BTN_EVEN:
		{

		}
		break;
	// Ȧ spr
	case ID_GOLDATTENDANCEBOOKPOPUP_SPR_ODD:
		{

		}
		break;
	// ¦ spr
	case ID_GOLDATTENDANCEBOOKPOPUP_SPR_EVEN:
		{

		}
		break;
	}
	
	return FALSE;
}

/// <summary>
/// ���콺 ���� ��ư �ٿ�
/// <summary>
/// <param name=""></param>
/// <returns></returns>
LRESULT CGoldAttendanceBookPopup::OnLButtonDown(LSY::CMessage *pMsg)
{
	LSY::CObjectMessage *msg = (LSY::CObjectMessage *)pMsg;
	switch(msg->GetObject()->GetID())
	{
	// Ȧ ��ư
	case ID_GOLDATTENDANCEBOOKPOPUP_BTN_ODD:
		{
		}
		break;
	// ¦ ��ư
	case ID_GOLDATTENDANCEBOOKPOPUP_BTN_EVEN:
		{
		}
		break;
	}
	
	return TRUE;
}

/// <summary>
/// ���콺 ���� ��ư ��
/// <summary>
/// <param name=""></param>
/// <returns></returns>
LRESULT CGoldAttendanceBookPopup::OnLButtonUp(LSY::CMessage *pMsg)
{
	LSY::CObjectMessage *msg = (LSY::CObjectMessage *)pMsg;
	switch(msg->GetObject()->GetID())
	{
	// Ȧ ��������Ʈ
	case ID_GOLDATTENDANCEBOOKPOPUP_SPR_ODD:
		{
		}
		break;
	}
	
	return TRUE;
}

/// <summary>
/// LSY Control ����
/// <summary>
/// <param name=""></param>
/// <returns></returns>
void CGoldAttendanceBookPopup::InitLSYControl()
{
	// ���
	m_pSprBack = (LSY::CSprite *)GetObject(ID_GOLDATTENDANCEBOOKPOPUP_SPR_BACK);

	// ��ư ��Ȱ��ȭ spr �̹���
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
	
	// Ȧ ��ư
	m_pBtnOdd = (LSY::CButton *)GetObject(ID_GOLDATTENDANCEBOOKPOPUP_BTN_ODD);
	m_pBtnOdd->AddHandler(LSY::EM_O_LBUTTONDOWN, LSY::Fnt(this, &CGoldAttendanceBookPopup::OnLButtonDown));
	m_pBtnOdd->AddHandler(LSY::EM_O_MOUSELCLICK, LSY::Fnt(this, &CGoldAttendanceBookPopup::OnClickButton));
	m_pBtnOdd->AddHandler(LSY::EM_O_MOUSELEAVE, LSY::Fnt(this, &CGoldAttendanceBookPopup::OnMouseLeave));
	m_pBtnOdd->AddHandler(LSY::EM_O_MOUSEMOVE, LSY::Fnt(this, &CGoldAttendanceBookPopup::OnMouseMove));
	m_pBtnOdd->SetEnable(false);
	
	// ¦ ��ư
	m_pBtnEven = (LSY::CButton *)GetObject(ID_GOLDATTENDANCEBOOKPOPUP_BTN_EVEN);
	m_pBtnEven->AddHandler(LSY::EM_O_LBUTTONDOWN, LSY::Fnt(this, &CGoldAttendanceBookPopup::OnLButtonDown));
	m_pBtnEven->AddHandler(LSY::EM_O_MOUSELCLICK, LSY::Fnt(this, &CGoldAttendanceBookPopup::OnClickButton));
	m_pBtnEven->AddHandler(LSY::EM_O_MOUSELEAVE, LSY::Fnt(this, &CGoldAttendanceBookPopup::OnMouseLeave));
	m_pBtnEven->AddHandler(LSY::EM_O_MOUSEMOVE, LSY::Fnt(this, &CGoldAttendanceBookPopup::OnMouseMove));
	m_pBtnEven->SetEnable(false);

	// �ݱ� ��ư
	m_pBtnClose = (LSY::CButton *)GetObject(ID_GOLDATTENDANCEBOOKPOPUP_BTN_CLOSE);
	m_pBtnClose->AddHandler(LSY::EM_O_MOUSELCLICK, LSY::Fnt(this, &CGoldAttendanceBookPopup::OnClickButton));
	m_pBtnClose->SetEnable(false);
	
}

/// <summary>
/// LSY Control NULL ������ �ʱ�ȭ.
/// ������, OnClose �Լ����� ȣ��.
/// <summary>
/// <param name=""></param>
/// <returns></returns>
void CGoldAttendanceBookPopup::InitLSYControlToNULL()
{
	// LSY ��Ʈ�� ����
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
/// ������ ����
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
/// â �ݱ�
/// <summary>
/// <param name="bContinue">������ �� ������ �� �ִ��� ����</param>
/// <returns></returns>
void CGoldAttendanceBookPopup::CloseDlg(bool bContinue)
{
	m_bContinue = bContinue;

	OnCancel();
}

/// <summary>
/// �������� ������ ��Ŷ(����) ó��
/// <summary>
/// <param name=""></param>
/// <returns></returns>
void CGoldAttendanceBookPopup::HandleResponse(int Signal, int TotSize, char *lpData) // ���� ���� �ڵ鸵
{
	switch(Signal)
	{
	//�� ���尡 ����Ǿ����� �˷��ش�.
	//�� ��Ŷ�� ������, ������ ������ �޾� Ȧ Ȥ�� ¦�� ����� ������ �����ָ� �ȴ�.
	case SV_PM_ODDEVEN_START_ROUND:
		{
			CSV_PM_ODDEVEN_START_ROUND msg;
			msg.Get(lpData, TotSize);
			
			m_Round = *msg.m_Round; //�ܰ� 1 ~ 8���� ����
			m_CurrentReward = *msg.m_CurrentReward; //���ݱ��� Ȯ���� ����ݾ�(���ý���, Ȥ�� �������⸦ �ϸ� �ް� �Ǵ� �����)
			m_WinReward = *msg.m_WinReward; //�̹� ���忡�� ���ü����� �ްԵǴ� ����ݾ��̴�.
			m_BetTime = *msg.m_BetTime; //�������ѽð��̴�(��), ���� -1�� ���� ������ ���ٴ� �ǹ�
			
			m_bContinue = true;
			
			if (m_Round == 1)
				SetData(m_Round, true);
		}
		break;
	//�������ÿ� ���� ����̴�.
	//���ÿ� ����������� ���Ῡ�ΰ� true�� ����, �����ϸ� ���Ῡ�ΰ� false �鼭 CSV_PM_ODDEVEN_START_ROUND ��Ŷ�� �ٽ� ���۵�
	case SV_PM_ODDEVEN_USER_BET_RESULT:
		{
			GM().SetPlayDailyGoldEffect(true);
			
			CSV_PM_ODDEVEN_USER_BET_RESULT msg;
			msg.Get(lpData, TotSize);
			
			SetData(m_Round, false);
			
			m_BetSuccess = *msg.m_BetSuccess; //��������
			m_Round_Cur = *msg.m_Round; //������ ����
			m_CardNum = *msg.m_CardNum; //�̹��� ����� �ο��� ī���ȣ�̴�.(������Ŀ ī�� �ε����� ������)
			m_RoundReward = *msg.m_RoundReward; //������ ���忡 ���ü����� ȹ���ϰԵǴ� �����
			
			m_bInProgress = true;
			
			m_pImgCard->SetShow(false);
			m_pSprCard->SetShow(true);
			
			m_pSprOdd->SetShow(false);
			m_pSprEven->SetShow(false);
			
			m_pBtnOdd->SetEnable(false);
			m_pBtnEven->SetEnable(false);
			
			// 7���� ���ʹ� ��ư�� �ƿ� �����.
			if (m_Round_Cur >= 7)
			{
				m_pBtnOdd->SetShow(false);
				m_pBtnEven->SetShow(false);
			}
			
			m_pBtnClose->SetEnable(false);
			
			if (m_BetSuccess)
			{
				DWORD dwPlayTime = 0;
				
				// ī�� ����
				m_pSprCard->SetPlayIndex(10);
				m_pSprCard->SetRepeatCount(1);
				SetTimer(TIMER_SUCCESS_ANI2, dwPlayTime, NULL);
				// ���� �˾� ���
				dwPlayTime = m_pSprCard->GetTotalPlayTime();
				SetTimer(TIMER_SUCCESS_DLG, dwPlayTime, NULL);
			}
			// ����
			else
			{
				DWORD dwPlayTime = 0;
				
				// ī�� ����
				m_pSprCard->SetPlayIndex(11);
				m_pSprCard->SetRepeatCount(1);
				SetTimer(TIMER_FAIL_ANI2, dwPlayTime, NULL);
				// ���� �˾� ���
				dwPlayTime += m_pSprCard->GetTotalPlayTime();
				SetTimer(TIMER_FAIL_DLG, dwPlayTime, NULL);
			}
		}
		break;
	//��÷�ڸ� ��ü �������� ��Ƽ�� �ش�~!
	case SV_PM_ODDEVEN_WINNER_NOTI:
		{
			if (m_pTxtWinNum == NULL)
				break;
			
			if (m_bContinue == false)
				break;
			
			CSV_PM_ODDEVEN_WINNER_NOTI msg;
			msg.Get(lpData, TotSize);
			
			m_WinRound = *msg.m_WinRound; //��÷ ����
			m_RewardMoney = *msg.m_RewardMoney; //��÷��
			m_RoundWinner = *msg.m_RoundWinner; //�ش� ���� ��÷�� ��
			
			if (m_WinRound < 6 || m_WinRound > 8)
				break;
			
			CString csMsg;
			CString csRewardMoney;
			CString csRewardMoneyComma;
			
			csRewardMoney.Format("%I64d", m_RewardMoney);
			GetNumberWithComma(csRewardMoney, csRewardMoneyComma);
			
			if (m_WinRound == 8)
				csRewardMoneyComma = "1õ��";
			else
				GetNumberWithComma(csRewardMoney, csRewardMoneyComma);
			
			csMsg.Format(
				"%dȸ�� �ִ� ��÷�� %s%s   %dȸ�� ��÷�� �� : %d��",
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
/// ī�� �ε����� �ش��ϴ� spr track �ε����� ���Ѵ�.
/// <summary>
/// <param name="">7��Ŀ ī�� �ε���</param>
/// <returns>spr track �ε���</returns>
int CGoldAttendanceBookPopup::GetPokerSprIndex(int nCardNum)
{
	int nIdx = 0;

	// Ŭ�ι�
	if (nCardNum >= 0 && nCardNum <= 12)
	{
		nIdx = (nCardNum + 1) % 13;
		nIdx += 61;
		
		return nIdx;
	}
	// ��Ʈ
	else if (nCardNum >= 13 && nCardNum <= 25)
	{
		nIdx = (nCardNum - 12) % 13;
		nIdx += 51;
		
		return nIdx;
	}
	// ���̾�
	else if (nCardNum >= 26 && nCardNum <= 38)
	{
		nIdx = (nCardNum - 25) % 13;
		nIdx += 41;
		
		return nIdx;
	}
	// �����̵�
	else if (nCardNum >= 39 && nCardNum <= 51)
	{
		nIdx = (nCardNum - 38) % 13;
		nIdx += 31;
		
		return nIdx;
	}

	return nIdx;
}

/// <summary>
/// �� �ڸ� �� ���� �޸� ����ֱ�
/// <summary>
/// <param name="nType"></param>
/// <returns></returns>
void CGoldAttendanceBookPopup::GetNumberWithComma(CString parm_data, CString &parm_string)
{
    // ������� ��Ȯ�ϰ� �����ϱ� ���ؼ� ',' ó������� ����� ���ڿ� ������ �ʱ�ȭ�Ѵ�.
    if(parm_string.GetLength()) parm_string.Format("");
    // ���ڿ��� ���̸� ���Ѵ�.
    int count = parm_data.GetLength();
	
    for(int i = 0; i < count; i++)
	{
        // 3�� ����� �� "," �� ���ڿ��� �߰��Ѵ�.
        if(i && !(i % 3)) parm_string = "," + parm_string;
        // parm_data ���� parm_string���� �����Ѱ��� ���������� �����Ѵ�.
        parm_string = CString(parm_data[count - 1 - i]) + parm_string;
    }
}

/// <summary>
/// �� �ڸ� �� ���� �޸� ����ֱ�
/// �� �Լ��� ����ϸ� �ȵ�. ��ǻ�Ϳ� ���� ���ڰ� ������ ���� �߻�.
/// <summary>
/// <param name="nType">������ ȹ�� ���</param>
/// <returns></returns>
void CGoldAttendanceBookPopup::GetNumberWithComma2(INT64 nNum, char cBuffer[])
{
	CString csNum;
	csNum.Format(_T("%I64d"), nNum);
	NUMBERFMT fmt = {0, 0, 3, ".", ",", 1};
	::GetNumberFormat(NULL, NULL, csNum, &fmt, cBuffer, strlen(cBuffer));
}

/// <summary>
/// ī�� ����
/// <summary>
/// <param name=""></param>
/// <returns></returns>
// ī�� ����
void CGoldAttendanceBookPopup::CardOut()
{
	// ī�� ����
	DWORD dwPlayTime = 0;
	m_pSprCard->SetPlayIndex(18);
	m_pSprCard->SetRepeatCount(1);
	dwPlayTime = m_pSprCard->GetTotalPlayTime();
	SetTimer(TIMER_CARD_OUT, dwPlayTime, NULL);
	m_pSprCard->StartAnimation();
	
	// ī�� ���� ���� - Ź!
	NMBASE::SOUND::g_Sound.PlayWav(".\\Lobby\\sound\\GoldDaily\\fx_card_slide.wav");
}