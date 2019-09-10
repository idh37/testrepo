#include "StdAfx.h"
#include "GoldAttendanceBookSubPopup.h"
#include "GoldAttendanceBookSubPopupid.h"

CGoldAttendanceBookSubPopup::CGoldAttendanceBookSubPopup(CWnd* pParent/* = NULL*/) : LSY::CLSYLibDialogBase(pParent)
{
	m_pImgBack = NULL; // ��� �̹���
	m_pBtnOk = NULL; // ����ϱ� ��ư
	m_pBtnClose = NULL; // �����ϱ� ��ư
	m_pBtnXClose = NULL; // X ��ư
	m_pTxtGold1 = NULL; // ȹ�� ��� �ؽ�Ʈ
	m_pTxtGold2 = NULL; // ȹ�� ��� �ؽ�Ʈ
	
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
	// LSY Control ����
	InitLSYControl();
	// ������ ����
	InitData();
	// 2�� �� �˾� �ݱ�
	// 1 : ����!, 2 : ����!, 3 : ���� ����, 4 : ����</param>
	if (m_nPopupType >= 1 && m_nPopupType <= 3)
		SetTimer(TIMER_CLOSE_DLG, 2000, NULL);
	
	return TRUE;
}

/// <summary>
/// Ÿ�̸� �Լ� �������̵�
/// <summary>
/// <param name=""></param>
/// <returns></returns>
void CGoldAttendanceBookSubPopup::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
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
/// ��ư Ŭ�� �̺�Ʈ. ��κ� ������ ��û ������ ���.
/// <summary>
/// <param name=""></param>
/// <returns></returns>
LRESULT CGoldAttendanceBookSubPopup::OnClickButton(LSY::CMessage *pMsg)
{
	LSY::CObjectMessage *msg = (LSY::CObjectMessage *)pMsg;
	switch(msg->GetObject()->GetID())
	{
	case ID_GOLDATTENDANCEBOOKSUBPOPUP_BTN_OK: // ����ϱ� ��ư
		{
			m_bCloseParent = false;
			CloseDlg();
		}
		break;
	case ID_GOLDATTENDANCEBOOKSUBPOPUP_BTN_CLOSE: // �����ϱ� ��ư
		{
			m_bCloseParent = true;
			CloseDlg();
		}
		break;
	case ID_GOLDATTENDANCEBOOKSUBPOPUP_BTN_XCLOSE: // X ��ư
		{
			m_bCloseParent = false;
			CloseDlg();
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
void CGoldAttendanceBookSubPopup::InitLSYControl()
{
	// ��� �̹���
	m_pImgBack = (LSY::CImage *)GetObject(ID_GOLDATTENDANCEBOOKSUBPOPUP_IMG_BACK);
	// ����ϱ� ��ư
	m_pBtnOk = (LSY::CButton *)GetObject(ID_GOLDATTENDANCEBOOKSUBPOPUP_BTN_OK);
	m_pBtnOk->AddHandler(LSY::EM_O_MOUSELCLICK, LSY::Fnt(this, &CGoldAttendanceBookSubPopup::OnClickButton));
	// �����ϱ� ��ư
	m_pBtnClose = (LSY::CButton *)GetObject(ID_GOLDATTENDANCEBOOKSUBPOPUP_BTN_CLOSE);
	m_pBtnClose->AddHandler(LSY::EM_O_MOUSELCLICK, LSY::Fnt(this, &CGoldAttendanceBookSubPopup::OnClickButton));
	// X ��ư
	m_pBtnXClose = (LSY::CButton *)GetObject(ID_GOLDATTENDANCEBOOKSUBPOPUP_BTN_XCLOSE);
	m_pBtnXClose->AddHandler(LSY::EM_O_MOUSELCLICK, LSY::Fnt(this, &CGoldAttendanceBookSubPopup::OnClickButton));
	// ȹ�� ��� �ؽ�Ʈ
	m_pTxtGold1 = (LSY::CText *)GetObject(ID_GOLDATTENDANCEBOOKSUBPOPUP_TXT_GOLD1);
	m_pTxtGold2 = (LSY::CText *)GetObject(ID_GOLDATTENDANCEBOOKSUBPOPUP_TXT_GOLD2);
}

/// <summary>
/// ������ ����
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
	// ���� �˾�
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
			csGoldComma.AppendFormat("%s�� ȹ���Ͽ����ϴ�.", strChip_Name[(int)ROOM_CHIPKIND_GOLD]);
			m_pTxtGold1->SetText("��÷�� ���� �帳�ϴ�.");
			m_pTxtGold2->SetText((LPCTSTR)csGoldComma);
		}
		break;
	// ���� �˾�
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
			csGoldComma.AppendFormat("%s�� �����մϴ�.", strChip_Name[(int)ROOM_CHIPKIND_GOLD]);
			m_pTxtGold1->SetText("��Ÿ���Ե� ���Դϴ�.");
			m_pTxtGold2->SetText((LPCTSTR)csGoldComma);
		}
		break;
	// ���� ���� �˾�
	case 3:
		{
			m_pImgBack->SetIndex(103);
			m_pBtnOk->SetShow(false);
			m_pBtnClose->SetShow(false);
			m_pTxtGold1->SetShow(true);
			m_pTxtGold2->SetShow(true);
			
			m_pTxtGold1->SetText("�����մϴ�.");
			CString csFinal;
			csFinal.Format("1,000��%s ���� �Ǿ����ϴ�.", strChip_Name[(int)ROOM_CHIPKIND_GOLD]);
			m_pTxtGold2->SetText((LPCTSTR)csFinal);
		}
		break;
	// ���� �˾�
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
/// �������� ������ ��Ŷ(����) ó��
/// <summary>
/// <param name=""></param>
/// <returns></returns>
void CGoldAttendanceBookSubPopup::HandleResponse(int Signal, int TotSize, char *lpData) // ���� ���� �ڵ鸵
{
}

/// <summary>
/// �������� ������ ��Ŷ(����) ó��
/// <summary>
/// <param name="nType">// 1 : ����!, 2 : ����!, 3 : ���� ����, 4 : ����</param>
/// <param name="nGold">ȹ���� �ݾ�</param>
/// <returns></returns>
void CGoldAttendanceBookSubPopup::SetPopupType(int nType, INT64 nGold, bool bModaled)
{
	m_nPopupType = nType;
	m_nGold = nGold; // ȹ�� ��� ����
	
	if (bModaled)
	{
		// ������ ����
		InitData();
		// 2�� �� �˾� �ݱ�
		// 1 : ����!, 2 : ����!, 3 : ���� ����, 4 : ����</param>
		if (m_nPopupType >= 1 && m_nPopupType <= 3)
			SetTimer(TIMER_CLOSE_DLG, 2000, NULL);
	}
}

bool CGoldAttendanceBookSubPopup::GetCloseParent()
{
	return m_bCloseParent;
}

/// <summary>
/// �� �ڸ� �� ���� �޸� ����ֱ�
/// <summary>
/// <param name="nType">������ ȹ�� ���</param>
/// <returns></returns>
void CGoldAttendanceBookSubPopup::GetNumberWithComma(CString parm_data, CString &parm_string)
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
/// �� �Լ� ����ϸ� �ȵ�! Ư�� PC���� ���� ������ ���� �߻�.
/// <summary>
/// <param name="nType">������ ȹ�� ���</param>
/// <returns></returns>
void CGoldAttendanceBookSubPopup::GetNumberWithComma2(INT64 nNum, char cBuffer[])
{
	CString csNum;
	csNum.Format(_T("%I64d"), nNum);
	NUMBERFMT fmt = {0, 0, 3, ".", ",", 1};
	::GetNumberFormat(NULL, NULL, csNum, &fmt, cBuffer, strlen(cBuffer));
}
