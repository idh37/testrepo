
#include "StdAfx.h"
#include "ChuSeokAttendanceDlg.h"
#include "chuseokattendancedlgid.h"

enum eChuSeokImageIndex
{
	ECSII_ATTENDANCE = 1,
	ECSII_ABSENCE = 2,
	ECSII_REWARD_COMPLETION = 7,
	ECSII_REWARD_FAIL = 8,
};


CChuSeokDlg::CChuSeokDlg(CWnd* pParent/* = NULL*/)
: LSY::CLSYLibDialogBase(pParent),
m_forcePopup(false), m_finalReward(0)
{

}

CChuSeokDlg::~CChuSeokDlg()
{
	KillTimer( TIMER_ID );
}

bool CChuSeokDlg::OnCreatedProject()
{
	_InitLSYControl();

	_CheckAttendance();

	SetTimer(TIMER_ID, MSG_ELAPSE_TIME, NULL);	
	
	return true;
}

INT_PTR CChuSeokDlg::DoModal()
{
	if(!m_forcePopup)
	{
		int curDay = m_attendanceTotalInfo.size();
		INT64 todayReward = m_attendanceTotalInfo[curDay - 1].llReward;
		bool todayAttendance = m_attendanceTotalInfo[curDay - 1].bIsAttendance;

		if(todayAttendance && !todayReward)
		{
			m_forcePopup = true;
			return -1;
		}
	}	

	return __super::DoModal(GetObjectMan(), ID_CHUSEOKATTENDANCEDLG, true, AfxGetMainWnd());
}

BOOL CChuSeokDlg::PreTranslateMessage(MSG* pMsg)
{
	if(pMsg->message == WM_SYSKEYDOWN)
	{
		if(pMsg->wParam == VK_F4)
		{
			return TRUE;
		}
	}

	if(pMsg->message == WM_KEYDOWN)
	{
		if(pMsg->wParam == VK_ESCAPE)
		{
			return TRUE;
		}
	}

	return CWnd::PreTranslateMessage( pMsg );
}

LRESULT CChuSeokDlg::OnClickButton(LSY::CMessage*pMsg)
{
	LSY::CObjectMessage* msg = (LSY::CObjectMessage*)pMsg;

	switch(msg->GetObject()->GetID())
	{
	case ID_CHUSEOKATTENDANCEDLG_BUTTON_CLOSE:
		OnCancel();		
		break;
	}	

	return TRUE;
}

void CChuSeokDlg::HandleResponse(int signal, int totSize, char *lpData)
{
	switch(signal)
	{
	case SV_PM_2019HAVEST_INFO:
		break;
	case SV_PM_2019HAVEST_REQUEST_RESULT:
		break;
	}
}

void CChuSeokDlg::_InitLSYControl()
{
	m_pClose = (LSY::CButton*)GetObject(ID_CHUSEOKATTENDANCEDLG_BUTTON_CLOSE);
	m_pClose->AddHandler(LSY::EM_O_MOUSELCLICK, LSY::Fnt(this, &CChuSeokDlg::OnClickButton));

	for(int i = 0; i < MAX_ATTENDANCE_CHECK; ++i)
	{
		m_pAttendanceCheck[i] = (LSY::CImage*)GetObject(ID_CHUSEOKATTENDANCEDLG_ATTENDANCECHECKIMG01 + i);
	}

	for(int i = 0; i < MAX_CONT_ATTENDANCE_CHECK; ++i)
	{
		m_pContAttendanceCheck[i] = (LSY::CImage*)GetObject(ID_CHUSEOKATTENDANCEDLG_CONTATTENDANCECHECKIMG01 + i);
		m_pAttendingCheck[i] = (LSY::CImage*)GetObject(ID_CHUSEOKATTENDANCEDLG_ATTENDINGIMG01 + i);
	}
}

void CChuSeokDlg::_SetData(int totSize, char* lpData)
{
	if(m_attendanceTotalInfo.size())
	{
		m_attendanceTotalInfo.clear();
	}

	promotion::CSV_PROMOTION_2018THANKSGIVING_ATTEDANCE_TOTAL_INFO attendanceTotalPacket;
	attendanceTotalPacket.Get(lpData, totSize);	

	int infoSize = *attendanceTotalPacket.m_nInfoSize;
	promotion::PROMOTION_2018THANKSGIVING_ATTEDANCE_DAY_INFO* attendanceTotalInfo = attendanceTotalPacket.m_pAttDayInfos;

	int nStart = 0;
	if(infoSize > MAX_ATTENDANCE_CHECK)
	{
		nStart = infoSize - MAX_ATTENDANCE_CHECK;
	}
	
	for(int i = nStart; i < infoSize; ++i)
	{
		m_attendanceTotalInfo.push_back(attendanceTotalInfo[i]);
	}
	
	m_finalReward = *attendanceTotalPacket.m_llAdditionalFinalReward;

	m_forcePopup = false;

	MM().Call(UPG_CMK_SHOW_CHUSEOK_ATTENDANCE_BTN, TRUE);
	GM().SetShowEndEventPopup(true);
}

void CChuSeokDlg::_CheckAttendance()
{
	int curDay = m_attendanceTotalInfo.size();

	bool attendance[MAX_ATTENDANCE_CHECK] =
	{
		false, 
	};

	for(int i = 0; i < curDay; ++i)
	{
		attendance[i] = m_attendanceTotalInfo[i].bIsAttendance;
	}	

	bool contAttendance[MAX_CONT_ATTENDANCE_CHECK - 1] =
	{
		false, false, false, false,
	};

	int contAttendanceCnt = 0;	

	for(int i = 0; i < MAX_ATTENDANCE_CHECK; ++i)
	{
		m_pAttendanceCheck[i]->SetShow(false);
	}

	for(int i = 0; i < MAX_CONT_ATTENDANCE_CHECK; ++i)
	{
		m_pContAttendanceCheck[i]->SetShow(false);
	}

	for(int i = 0; i < curDay; ++i)
	{
		if(attendance[i])
		{
			++contAttendanceCnt;	
		}

		if(attendance[i])
		{
			m_pAttendanceCheck[i]->SetIndex(ECSII_ATTENDANCE);
		}
		else
		{
			m_pAttendanceCheck[i]->SetIndex(ECSII_ABSENCE);
		}

		m_pAttendanceCheck[i]->SetShow(true);

		if((i + 1) % SEVEN_DAYS_WEEK == 0)
		{
			if(contAttendanceCnt == SEVEN_DAYS_WEEK)
			{
				contAttendance[i / SEVEN_DAYS_WEEK] = true;
			}
			else
			{
				contAttendance[i / SEVEN_DAYS_WEEK] = false;
			}

			contAttendanceCnt = 0;
		}		
	}

	for(int i = 0; i < MAX_CONT_ATTENDANCE_CHECK; ++i)
	{
		m_pAttendingCheck[i]->SetShow(false);
	}

	int sAttending = -1;

	if(curDay % SEVEN_DAYS_WEEK == 0)
	{
		sAttending = curDay - SEVEN_DAYS_WEEK;
	}
	else 
	{
		sAttending = curDay - (curDay % SEVEN_DAYS_WEEK);
	}

	bool attendingOfWeek = true;
	bool regularPossibility = true;

	for(int i = sAttending; i < curDay; ++i)
	{
		if(!attendance[i])
		{
			attendingOfWeek = false;
			break;
		}
	}

	if(attendingOfWeek)
	{
		if(curDay % SEVEN_DAYS_WEEK != 0)
		{
			m_pAttendingCheck[curDay / SEVEN_DAYS_WEEK]->SetShow(true);
		}
		else
		{
			m_pAttendingCheck[curDay / SEVEN_DAYS_WEEK]->SetShow(false);
		}		
	}
	else
	{
		m_pContAttendanceCheck[(curDay - 1) / SEVEN_DAYS_WEEK]->SetIndex(ECSII_REWARD_FAIL);
		m_pContAttendanceCheck[(curDay - 1) / SEVEN_DAYS_WEEK]->SetShow(true);
		regularPossibility = false;
	}

	int eContAttendance = curDay / SEVEN_DAYS_WEEK;

	for(int i = 0; i < eContAttendance; ++i)
	{
		if(contAttendance[i])
		{
			m_pContAttendanceCheck[i]->SetIndex(ECSII_REWARD_COMPLETION);
		}
		else
		{
			m_pContAttendanceCheck[i]->SetIndex(ECSII_REWARD_FAIL);
			regularPossibility = false;
		}

		m_pContAttendanceCheck[i]->SetShow(true);
	}	

	if(regularPossibility)
	{
		if(curDay == MAX_DAYS_ATTEND_REGULARLY)
		{
			m_pContAttendanceCheck[MAX_CONT_ATTENDANCE_CHECK - 1]->SetIndex(ECSII_REWARD_COMPLETION);
			m_pContAttendanceCheck[MAX_CONT_ATTENDANCE_CHECK - 1]->SetShow(true);
		}
		else
		{
			m_pAttendingCheck[MAX_CONT_ATTENDANCE_CHECK - 1]->SetShow(true);
		}		
	}
	else
	{
		m_pContAttendanceCheck[MAX_CONT_ATTENDANCE_CHECK - 1]->SetIndex(ECSII_REWARD_FAIL);
		m_pContAttendanceCheck[MAX_CONT_ATTENDANCE_CHECK - 1]->SetShow(true);
	}
}BEGIN_MESSAGE_MAP(CChuSeokDlg, LSY::CLSYLibDialogBase)
ON_WM_TIMER()
END_MESSAGE_MAP()

void CChuSeokDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	switch(nIDEvent)
	{
	case TIMER_ID:
		{
			KillTimer(TIMER_ID);			
			_ShowRewardPopup();

			m_forcePopup = true;
			
		}
		return;
	}
	
	CLSYLibDialogBase::OnTimer(nIDEvent);
}

void CChuSeokDlg::_ShowRewardPopup()
{
	CString strPopupMsg;

	if(m_forcePopup)
	{
		strPopupMsg.Format("오늘은 더이상 참여할 수 없습니다.");

		ShowMsgModelessDlg(m_hWnd, strPopupMsg);
	}

	int curDay = m_attendanceTotalInfo.size();
	int rewardOneHundredMillionUnit = 0;
	INT64 todayReward = m_attendanceTotalInfo[curDay - 1].llReward;
	INT64 additionReward = m_attendanceTotalInfo[curDay - 1].llAdditionalReward;	
	CString strPopupRestMsg;

	rewardOneHundredMillionUnit = (int)(todayReward / ONE_HUNDRED_MILLION);
	strPopupMsg.Format("%d일차 출석보상 %d억이 지급되었습니다.\n", curDay, rewardOneHundredMillionUnit);

	if(additionReward)
	{
		rewardOneHundredMillionUnit = (int)(additionReward / ONE_HUNDRED_MILLION);
		strPopupRestMsg.Format("연속%d일 출석보상 %d억이 지급되었습니다.", SEVEN_DAYS_WEEK, rewardOneHundredMillionUnit);
	}

	if(m_finalReward)
	{
		rewardOneHundredMillionUnit += (int)(m_finalReward / ONE_HUNDRED_MILLION);
		strPopupRestMsg.Format("연속%d일 출석보상 %d억이 지급되었습니다.", MAX_DAYS_ATTEND_REGULARLY, rewardOneHundredMillionUnit);
	}

	strPopupMsg += strPopupRestMsg;

	ShowMsgModelessDlg(m_hWnd, strPopupMsg);	
}
