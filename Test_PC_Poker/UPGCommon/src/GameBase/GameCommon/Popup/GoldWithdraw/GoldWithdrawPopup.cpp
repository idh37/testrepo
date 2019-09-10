#include "StdAfx.h"
#include "GoldWithdrawPopup.h"
#include "goldwithdrawpopupid.h"


CGoldWithdrawPopup::CGoldWithdrawPopup(CWnd* pParent/* = NULL*/) : LSY::CLSYLibDialogBase(pParent)
{
	InitLSYControlToNULL(); // LSY Control NULL ������ �ʱ�ȭ

	// ��Ÿ ����
	m_i64SaveGold = 0; // ���� ���
	m_i64WithdrawGold = 0; // ��� ���
	m_i64MyGold = 0; // ���� �� ���

	m_nLimit = 0; // ���� ���� ��� ���� �ѵ�, �α��ν� �޾ƿ�

	m_csLimitNormal = "2,000,000"; // ��� ���� �ѵ� �Ϲ�
	m_csLimitGold = "5,000,000"; // ��� ���� �ѵ� ���
	m_csLimitDia = "15,000,000"; // ��� ���� �ѵ� ���̾�
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
	// LSY Control ����
	InitLSYControl();
	// �ʱ� ������ ����
	InitData();

	return TRUE;
}

void CGoldWithdrawPopup::OnCancel()
{
	__super::OnCancel();
}

/// <summary>
/// ��ư Ŭ�� �̺�Ʈ. ��κ� ������ ��û ������ ���.
/// <summary>
/// <param name=""></param>
/// <returns></returns>
LRESULT CGoldWithdrawPopup::OnClickButton(LSY::CMessage *pMsg)
{
	LSY::CObjectMessage *msg = (LSY::CObjectMessage *)pMsg;

	switch(msg->GetObject()->GetID())
	{
	case ID_GOLDWITHDRAWPOPUP_BTN_CLOSE: // �ݱ� ��ư
		CloseDlg();
		break;
	case ID_GOLDWITHDRAWPOPUP_BTN_001: // 10�� ��ư
		{
			RaiseWithdrawGold(10 * 10000);
		}
		break;
	case ID_GOLDWITHDRAWPOPUP_BTN_002: // 30�� ��ư
		{
			RaiseWithdrawGold(30 * 10000);
		}
		break;
	case ID_GOLDWITHDRAWPOPUP_BTN_003: // 50�� ��ư
		{
			RaiseWithdrawGold(50 * 10000);
		}
		break;
	case ID_GOLDWITHDRAWPOPUP_BTN_004: // �ʱ�ȭ ��ư
		{
			m_i64WithdrawGold = 0;
			
			SetData();
		}
		break;
	case ID_GOLDWITHDRAWPOPUP_BTN_005: // 100��
		{
			RaiseWithdrawGold(100 * 10000);
		}
		break;
	case ID_GOLDWITHDRAWPOPUP_BTN_006: // 200��
		{
			RaiseWithdrawGold(200 * 10000);
		}
		break;
	case ID_GOLDWITHDRAWPOPUP_BTN_007: // 500��
		{
			RaiseWithdrawGold(500 * 10000);
		}
		break;
	case ID_GOLDWITHDRAWPOPUP_BTN_008: // �ִ�
		{
			// i64MaxGold : �ִ�� ����� �� �ִ� ���
			INT64 i64MaxGold = m_i64SaveGold;
			
			// �ִ�� ����� �� �ִ� ���� �����ѵ��� �Ѿ�� �ȵȴ�
			if (m_i64MyGold + i64MaxGold > m_nLimit)
				i64MaxGold = m_nLimit - m_i64MyGold;
			
			// i64AGold : �߰� ��� ���
			INT64 i64AGold = i64MaxGold - m_i64WithdrawGold;
			
			RaiseWithdrawGold(i64AGold);
		}
		break;
	case ID_GOLDWITHDRAWPOPUP_BTN_WITHDRAW: // ����ϱ�
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
/// LSY Control ����
/// <summary>
/// <param name=""></param>
/// <returns></returns>
void CGoldWithdrawPopup::InitLSYControl()
{
	// �ݱ� ��ư
	m_pBtnClose = (LSY::CButton *)GetObject(ID_GOLDWITHDRAWPOPUP_BTN_CLOSE);
	m_pBtnClose->AddHandler(LSY::EM_O_MOUSELCLICK, LSY::Fnt(this, &CGoldWithdrawPopup::OnClickButton));

	// ���� ���
	m_pTxtSaveGold = (LSY::CText *)GetObject(ID_GOLDWITHDRAWPOPUP_TXT_SAVEGOLD);
	// ��� ���
	m_pTxtWithdrawGold = (LSY::CText *)GetObject(ID_GOLDWITHDRAWPOPUP_TXT_WITHDRAWGOLD);
	// ���� �� ���
	m_pTxtMyGold = (LSY::CText *)GetObject(ID_GOLDWITHDRAWPOPUP_TXT_MYGOLD);

	// 10��
	m_pBtn001 = (LSY::CButton *)GetObject(ID_GOLDWITHDRAWPOPUP_BTN_001);
	m_pBtn001->AddHandler(LSY::EM_O_MOUSELCLICK, LSY::Fnt(this, &CGoldWithdrawPopup::OnClickButton));
	// 30��
	m_pBtn002 = (LSY::CButton *)GetObject(ID_GOLDWITHDRAWPOPUP_BTN_002);
	m_pBtn002->AddHandler(LSY::EM_O_MOUSELCLICK, LSY::Fnt(this, &CGoldWithdrawPopup::OnClickButton));
	// 50��
	m_pBtn003 = (LSY::CButton *)GetObject(ID_GOLDWITHDRAWPOPUP_BTN_003);
	m_pBtn003->AddHandler(LSY::EM_O_MOUSELCLICK, LSY::Fnt(this, &CGoldWithdrawPopup::OnClickButton));
	// �ʱ�ȭ
	m_pBtn004 = (LSY::CButton *)GetObject(ID_GOLDWITHDRAWPOPUP_BTN_004);
	m_pBtn004->AddHandler(LSY::EM_O_MOUSELCLICK, LSY::Fnt(this, &CGoldWithdrawPopup::OnClickButton));
	// 100��
	m_pBtn005 = (LSY::CButton *)GetObject(ID_GOLDWITHDRAWPOPUP_BTN_005);
	m_pBtn005->AddHandler(LSY::EM_O_MOUSELCLICK, LSY::Fnt(this, &CGoldWithdrawPopup::OnClickButton));
	// 200��
	m_pBtn006 = (LSY::CButton *)GetObject(ID_GOLDWITHDRAWPOPUP_BTN_006);
	m_pBtn006->AddHandler(LSY::EM_O_MOUSELCLICK, LSY::Fnt(this, &CGoldWithdrawPopup::OnClickButton));
	// 500��
	m_pBtn007 = (LSY::CButton *)GetObject(ID_GOLDWITHDRAWPOPUP_BTN_007);
	m_pBtn007->AddHandler(LSY::EM_O_MOUSELCLICK, LSY::Fnt(this, &CGoldWithdrawPopup::OnClickButton));
	// �ִ�
	m_pBtn008 = (LSY::CButton *)GetObject(ID_GOLDWITHDRAWPOPUP_BTN_008);
	m_pBtn008->AddHandler(LSY::EM_O_MOUSELCLICK, LSY::Fnt(this, &CGoldWithdrawPopup::OnClickButton));

	// ��� ���� �ѵ� �Ϲ�
	m_pTxtLimitNormal = (LSY::CText *)GetObject(ID_GOLDWITHDRAWPOPUP_TXT_LIMITNORMAL);
	// ��� ���� �ѵ� ���
	m_pTxtLimitGold = (LSY::CText *)GetObject(ID_GOLDWITHDRAWPOPUP_TXT_LIMITGOLD);
	// ��� ���� �ѵ� ���̾�
	m_pTxtLimitDia = (LSY::CText *)GetObject(ID_GOLDWITHDRAWPOPUP_TXT_LIMITDIA);

	// ����ϱ� ��ư
	m_pBtnWithdraw = (LSY::CButton *)GetObject(ID_GOLDWITHDRAWPOPUP_BTN_WITHDRAW);
	m_pBtnWithdraw->AddHandler(LSY::EM_O_MOUSELCLICK, LSY::Fnt(this, &CGoldWithdrawPopup::OnClickButton));
	m_pBtnWithdraw->SetEnable(false);
}

/// <summary>
/// LSY Control NULL ������ �ʱ�ȭ.
/// ������, OnClose �Լ����� ȣ��.
/// <summary>
/// <param name=""></param>
/// <returns></returns>
void CGoldWithdrawPopup::InitLSYControlToNULL()
{
	// LSY ��Ʈ�� ����
	m_pBtnClose = NULL; // �ݱ� ��ư

	m_pTxtSaveGold = NULL; // ���� ��� �ؽ�Ʈ
	m_pTxtWithdrawGold = NULL; // ��� ��� �ؽ�Ʈ
	m_pTxtMyGold = NULL; // ���� �� ��� �ؽ�Ʈ

	m_pBtn001 = NULL; // 10�� ��ư
	m_pBtn002 = NULL; // 30�� ��ư
	m_pBtn003 = NULL; // 50�� ��ư
	m_pBtn004 = NULL; // �ʱ�ȭ ��ư
	m_pBtn005 = NULL; // 100�� ��ư
	m_pBtn006 = NULL; // 200�� ��ư
	m_pBtn007 = NULL; // 500�� ��ư
	m_pBtn008 = NULL; // �ִ� ��ư

	m_pTxtLimitNormal = NULL; // ��� ���� �ѵ� �Ϲ� �ؽ�Ʈ
	m_pTxtLimitGold = NULL; // ��� ���� �ѵ� ��� �ؽ�Ʈ
	m_pTxtLimitDia = NULL; // ��� ���� �ѵ� ���̾� �ؽ�Ʈ

	m_pBtnWithdraw = NULL; // ����ϱ� ��ư
}

/// <summary>
/// ������ �ʱⰪ ����
/// <summary>
/// <param name=""></param>
/// <returns></returns>
void CGoldWithdrawPopup::InitData()
{
	if (m_pTxtSaveGold == NULL || m_pTxtWithdrawGold == NULL || m_pTxtMyGold == NULL ||
		m_pTxtLimitNormal == NULL || m_pTxtLimitGold == NULL || m_pTxtLimitDia == NULL)
		return;

	m_i64WithdrawGold = 0;

	// �ݾ׿� �� �ڸ��� ���� �޸� �߰�
	CString csSaveGold = g_GetNumberWithComma(m_i64SaveGold); // ���� ���
	CString csWithdrawGold = g_GetNumberWithComma(m_i64WithdrawGold); // ��� ���
	CString csMyGold = g_GetNumberWithComma(m_i64MyGold); // ���� �� ���

	m_pTxtSaveGold->SetText(csSaveGold.GetBuffer(0)); // ���� ���
	m_pTxtWithdrawGold->SetText(csWithdrawGold.GetBuffer(0)); // ��� ���
	m_pTxtMyGold->SetText(csMyGold.GetBuffer(0)); // ���� �� ���

	m_pTxtLimitNormal->SetText(m_csLimitNormal.GetBuffer(0)); // ��� ���� �ѵ� �Ϲ�
	m_pTxtLimitGold->SetText(m_csLimitGold.GetBuffer(0)); // ��� ���� �ѵ� ���
	m_pTxtLimitDia->SetText(m_csLimitDia.GetBuffer(0)); // ��� ���� �ѵ� ���̾�

	SetSumBtn(); // �ݾ� ��ư Enable ����

	m_nLimit = g_MaxServerGold; // ���� ���� ��� ���� �ѵ�, �α��ν� �޾ƿ�
}

/// <summary>
/// ������ ����
/// <summary>
/// <param name=""></param>
/// <returns></returns>
void CGoldWithdrawPopup::SetData()
{
	if (m_pTxtSaveGold == NULL || m_pTxtWithdrawGold == NULL || m_pTxtMyGold == NULL || m_pBtnWithdraw == NULL)
		return;

	// �ݾ׿� �� �ڸ��� ���� �޸� �߰�
	CString csSaveGold = g_GetNumberWithComma(m_i64SaveGold); // ���� ���
	CString csWithdrawGold = g_GetNumberWithComma(m_i64WithdrawGold); // ��� ���
	CString csMyGold_sum = g_GetNumberWithComma(m_i64MyGold + m_i64WithdrawGold); // ���� �� ���
	
	m_pTxtSaveGold->SetText(csSaveGold.GetBuffer(0)); // ���� ���
	m_pTxtWithdrawGold->SetText(csWithdrawGold.GetBuffer(0)); // ��� ���
	m_pTxtMyGold->SetText(csMyGold_sum.GetBuffer(0)); // ���� �� ���
	
	if (m_i64WithdrawGold <= 0)
		m_pBtnWithdraw->SetEnable(false);
	else
		m_pBtnWithdraw->SetEnable(true);
	
	SetSumBtn(); // �ݾ� ��ư Enable ����
}

void CGoldWithdrawPopup::CloseDlg(void)
{
	InitLSYControlToNULL();
	
	OnCancel();
}

/// <summary>
/// �������� ������ ��Ŷ(����) ó��
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
				m_i64MyGold = *msg.m_RealChip; // ���� �� ���
			}
			else if (strcmp(msg.m_ChipType, "GB") == 0)
			{
				m_i64SaveGold = *msg.m_RealChip; // ���� ���
			}
			
			SetData();
		}
		break;
	case SV_GOLDBANK_WITHDRAW_RESULT:
		{
			CSV_GOLDBANK_WITHDRAW_RESULT msg;
			msg.Get(lpData, TotSize);
			
			int nResult = *msg.m_Result; //����ڵ� 0.����, 1.�����ܾ׺���, 2.��庸���ѵ����ɸ�, 99.��Ÿ����
			
			switch (nResult)
			{
			case 0:
				{
					CString strMsg;
					strMsg.Format("���������� %s�� �̵� �Ǿ����ϴ�.", strChip_Name[(int)ROOM_CHIPKIND_GOLD]);
					m_dlgMsgBox.DoModal(CMsgBoxDlg::EMBT_NOTICE, strMsg);
					GM().SetOverMoneyGB(false);
				}
				break;
			case 1:
				// ��ݹ�ư ������ ���� UI �ܰ迡�� ��ȿ�� �˻簡 �Ǳ� ������ �� �޽����� �ʿ� ����
				{
					CString strMsg;
					strMsg.Format("���� %s�� �����մϴ�. %s�� Ȯ���� �ּ���.", strChip_Name[(int)ROOM_CHIPKIND_GOLD], strChip_Name[(int)ROOM_CHIPKIND_GOLD]);
					m_dlgMsgBox.DoModal(CMsgBoxDlg::EMBT_NOTICE, strMsg);
				}
				return;
			case 2:
				// ��ݹ�ư ������ ���� UI �ܰ迡�� ��ȿ�� �˻簡 �Ǳ� ������ �� �޽����� �ʿ� ����
				m_dlgMsgBox.DoModal(CMsgBoxDlg::EMBT_NOTICE, "���� �ѵ��� �ʰ��� ����� �Ұ����մϴ�.");
				return;
			case 99:
				m_dlgMsgBox.DoModal(CMsgBoxDlg::EMBT_NOTICE, "�� �� ���� ����.\nError code : 99");
				return;
			default:
				return;
			}

			//INT64 m_WithDrawAmount = *msg.m_WithDrawAmount; //����� �ݾ�(���� ��û�� �ݾ�)
			m_i64SaveGold = *msg.m_RemainGoldBank; //������ �����¿� ���� �ִ� �ݾ�
			m_i64MyGold = *msg.m_CurGold; //����ݾ� �ջ��� �����ϰԵ� ���
			m_i64WithdrawGold = 0; // ��� ���
			
			SetData();
		}
		break;
	}
}

/// <summary>
/// ���� ��� ��忡 �߰� ��� ��� �߰�
/// <summary>
/// <param name="nAmount">�߰� ��� ���</param>
/// <returns></returns>
void CGoldWithdrawPopup::RaiseWithdrawGold(INT64 nAmount)
{
	// ���� ��� ��� + �߰� ��� ��� + ���� ��� > ���� �ѵ� -> ���� �ѵ� �ʰ�
	if (m_i64WithdrawGold + nAmount + m_i64MyGold > m_nLimit)
	{
		m_dlgMsgBox.DoModal(CMsgBoxDlg::EMBT_NOTICE, "���� �ѵ��� �ʰ��� ����� �Ұ����մϴ�.");
		
		return;
	}
	// ���� ��� ��尡 �̹� ������ ��ü�� ���
	else if (m_i64WithdrawGold >= m_i64SaveGold)
	{
		CString strMsg;
		strMsg.Format("���� %s�� �����մϴ�. %s�� Ȯ���� �ּ���.", strChip_Name[(int)ROOM_CHIPKIND_GOLD], strChip_Name[(int)ROOM_CHIPKIND_GOLD]);
		m_dlgMsgBox.DoModal(CMsgBoxDlg::EMBT_NOTICE, strMsg);
		
		return;
	}
	// �߰� ��� ��尡 0���̰� ������ ��尡 �ѵ��� �������� ���
	else if (nAmount == 0 && m_i64WithdrawGold + nAmount + m_i64MyGold >= m_nLimit)
	{
		m_dlgMsgBox.DoModal(CMsgBoxDlg::EMBT_NOTICE, "���� �ѵ��� �ʰ��� ����� �Ұ����մϴ�.");
	}
	// ���� ��� ��� + �߰� ���� ��尡 ���� ��庸�� ���� ���
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

// �ݾ� ��ư Enable ����
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
	
	// ���� ��尡 ��� �ݾ� ��ư ���� ���� ��� dim ó���� �� �½��ϴ�.
	// ��� �ϱ淡 �߰�.
	if (m_i64SaveGold >= 100000)
		b10 = true; // 10�� ��ư
	if (m_i64SaveGold >= 300000)
		b30 = true; // 30�� ��ư
	if (m_i64SaveGold >= 500000)
		b50 = true; // 50�� ��ư
	if (m_i64SaveGold >= 1000000)
		b100 = true; // 100�� ��ư
	if (m_i64SaveGold >= 2000000)
		b200 = true; // 200�� ��ư
	if (m_i64SaveGold >= 5000000)
		b500 = true; // 500�� ��ư
	
	m_pBtn001->SetEnable(b10); // 10�� ��ư
	m_pBtn002->SetEnable(b30); // 30�� ��ư
	m_pBtn003->SetEnable(b50); // 50�� ��ư
	m_pBtn005->SetEnable(b100); // 100�� ��ư
	m_pBtn006->SetEnable(b200); // 200�� ��ư
	m_pBtn007->SetEnable(b500); // 500�� ��ư
}
