#include "StdAfx.h"
#include "NewYearAttendancePopup.h"
#include "newyearattendancepopupid.h"

CNewYearAttendancePopup::CNewYearAttendancePopup(CWnd* pParent/* = NULL*/) : LSY::CLSYLibDialogBase(pParent)
{
	m_Attendance_ID = 0; //�⼮�� ID
	m_NormalCount = 0; //�Ϲ� ����Ƚ��
	m_ContiCount = 0; //���� ����Ƚ��
}

CNewYearAttendancePopup::~CNewYearAttendancePopup(void)
{
}

INT_PTR CNewYearAttendancePopup::DoModal(CWnd *pParent)
{
	m_pParentWnd = pParent;
	return CLSYLibDialogBase::DoModal(GetObjectMan(), ID_NEWYEARATTENDANCEPOPUP);
}

bool CNewYearAttendancePopup::OnCreatedProject(void)
{
	// LSY Control ����
	InitLSYControl();
	// �ʱ� ������ ����
	InitData(m_NormalCount, m_ContiCount);
	
	return TRUE;
}

/// <summary>
/// ��ư Ŭ�� �̺�Ʈ. ��κ� ������ ��û ������ ���.
/// <summary>
/// <param name=""></param>
/// <returns></returns>
LRESULT CNewYearAttendancePopup::OnClickButton(LSY::CMessage *pMsg)
{
	LSY::CObjectMessage *msg = (LSY::CObjectMessage *)pMsg;
	switch(msg->GetObject()->GetID())
	{
	case ID_NEWYEARATTENDANCEPOPUP_BTN_CLOSE: // �ݱ� ��ư
		CloseDlg();
		CTime time = CTime::GetCurrentTime();
		if((2018==time.GetYear())&&(2==time.GetMonth())&&(8<=time.GetDay())&&(18>=time.GetDay()))
		{
			char* pBuffer=NMBASE::UTIL::GetProfile_string(GM().GetMyInfo()->UI.ID, "NewYearChickenBig","20000101000000");
			int nYear=0, nMonth=0, nDay=0, nHour=0, nMinute=0, nSecond=0;
			sscanf(pBuffer, "%04d%02d%02d%02d%02d%02d", &nYear, &nMonth, &nDay, &nHour, &nMinute, &nSecond);
			if(nDay < 8 || nDay > 18){
				CLobbyEventDlg dlg(GM().GetMainWnd(), EVENT_CHICKENBIG);
				dlg.DoModal("");
				NMBASE::UTIL::WriteProfile_string(GM().GetMyInfo()->UI.ID, "NewYearChickenBig", GM().GetCurrentTimeString().c_str());
			}
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
void CNewYearAttendancePopup::InitLSYControl()
{
	// �ݱ� ��ư
	m_pBtnClose = (LSY::CButton *)GetObject(ID_NEWYEARATTENDANCEPOPUP_BTN_CLOSE);
	m_pBtnClose->AddHandler(LSY::EM_O_MOUSELCLICK, LSY::Fnt(this, &CNewYearAttendancePopup::OnClickButton));
	
	// �⼮ �Ϸ� ����
	int sizea = sizeof(m_pImgCheck);
	int sizec = sizeof(m_pImgCheck[0]);
	int size = sizea / sizec;
	for (int i = 0; i < size; i++)
	{
		m_pImgCheck[i] = (LSY::CImage *)GetObject(ID_NEWYEARATTENDANCEPOPUP_IMG_CHECK01 + i);
	}
	
	// ���� �⼮ �Ϸ� ����
	sizea = sizeof(m_pImgConCheck);
	sizec = sizeof(m_pImgConCheck[0]);
	size = sizea / sizec;
	for (int i = 0; i < size; i++)
	{
		m_pImgConCheck[i] = (LSY::CImage *)GetObject(ID_NEWYEARATTENDANCEPOPUP_IMG_CONCHECK1 + i);
	}
	
	// ���� �⼮ �Ϸ� ���� �� ��
	m_pImgContinuous1 = (LSY::CImage *)GetObject(ID_NEWYEARATTENDANCEPOPUP_IMG_CONTINUOUS1);
	m_pImgContinuous2 = (LSY::CImage *)GetObject(ID_NEWYEARATTENDANCEPOPUP_IMG_CONTINUOUS2);
}

/// <summary>
/// ������ ����
/// <summary>
/// <param name=""></param>
/// <returns></returns>
void CNewYearAttendancePopup::InitData(int normalCount, int contiCount)
{
	// �⼮ �Ϸ� ����
	int sizea = sizeof(m_pImgCheck);
	int sizec = sizeof(m_pImgCheck[0]);
	int size = sizea / sizec;
	
	int stamp = normalCount;
	if (normalCount > size)
		stamp = size;
	
	for (int i = 0; i < stamp; i++)
	{
		m_pImgCheck[i]->SetShow(true);
	}
	if (normalCount <= size)
		m_pImgCheck[normalCount - 1]->SetIndex(1);
	for (int i = normalCount; i < size; i++)
	{
		m_pImgCheck[i]->SetShow(false);
	}
	
	// ���� �⼮ �Ϸ� ����
	sizea = sizeof(m_pImgConCheck);
	sizec = sizeof(m_pImgConCheck[0]);
	size = sizea / sizec;
	
	int contiComplete = 0;
	if (contiCount >= 20)
		contiComplete = 4;
	else if (contiCount >= 10)
		contiComplete = 3;
	else if (contiCount >= 5)
		contiComplete = 2;
	else if (contiCount >= 3)
		contiComplete = 1;
	for (int i = 0; i < contiComplete; i++)
	{
		m_pImgConCheck[i]->SetShow(true);
	}
	if (contiCount == 3 || contiCount == 5 || contiCount == 10 || contiCount == 20)
		m_pImgConCheck[contiComplete - 1]->SetIndex(1);
	for (int i = contiComplete; i < size; i++)
	{
		m_pImgConCheck[i]->SetShow(false);
	}
	
	// ���� �⼮ �Ϸ� ���� �� ��
	int temp = contiCount;
	m_pImgContinuous1->SetIndex(3 + ((temp / 1) % 10));
	m_pImgContinuous2->SetIndex(3 + ((temp / 10) % 10));
}

void CNewYearAttendancePopup::CloseDlg(void)
{
	OnCancel();
}

/// <summary>
/// �������� ������ ��Ŷ(����) ó��
/// <summary>
/// <param name=""></param>
/// <returns></returns>
void CNewYearAttendancePopup::HandleResponse(int Signal, int TotSize, char *lpData)
{
	switch(Signal)
	{
	case SV_PROMOTION_COMMON_ATTENDANCE_USERINFO:
		{
			CSV_PROMOTION_COMMON_ATTENDANCE_USERINFO msg;
			msg.Get(lpData, TotSize);
			
			m_Attendance_ID = *msg.m_Attendance_ID; //�⼮�� ID
			m_NormalCount = *msg.m_NormalCount; //�Ϲ� ����Ƚ��
			m_ContiCount = *msg.m_ContiCount; //���� ����Ƚ��
			
			// �ʱ� ������ ����
			//InitData(m_NormalCount, m_ContiCount);
		}
		break;
	}
}
