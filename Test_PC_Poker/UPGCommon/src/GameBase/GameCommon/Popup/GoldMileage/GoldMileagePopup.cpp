#include "StdAfx.h"
#include "GoldMileagePopup.h"
#include "goldmileagepopupid.h"

CGoldMileagePopup::CGoldMileagePopup(CWnd* pParent/* = NULL*/) : LSY::CLSYLibDialogBase(pParent)
{
}

CGoldMileagePopup::~CGoldMileagePopup(void)
{
}

INT_PTR CGoldMileagePopup::DoModal(CWnd *pParent)
{
	m_pParentWnd = pParent;
	return CLSYLibDialogBase::DoModal(GetObjectMan(), ID_GOLDMILEAGEPOPUP);
}

bool CGoldMileagePopup::OnCreatedProject(void)
{
	// LSY Control ����
	InitLSYControl();
	// �ʱ� ������ ����
	InitData(m_TempGold);
	
	return TRUE;
}

/// <summary>
/// ��ư Ŭ�� �̺�Ʈ. ��κ� ������ ��û ������ ���.
/// <summary>
/// <param name=""></param>
/// <returns></returns>
LRESULT CGoldMileagePopup::OnClickButton(LSY::CMessage *pMsg)
{
	LSY::CObjectMessage *msg = (LSY::CObjectMessage *)pMsg;
	switch(msg->GetObject()->GetID())
	{
	case ID_GOLDMILEAGEPOPUP_BTN_CLOSE: // �ݱ� ��ư
		CloseDlg();
		break;
	case ID_GOLDMILEAGEPOPUP_BTN_OK: // Ȯ�� ��ư
		CloseDlg();
		break;
	}
	
	return TRUE;
}

/// <summary>
/// LSY Control ����
/// <summary>
/// <param name=""></param>
/// <returns></returns>
void CGoldMileagePopup::InitLSYControl()
{
	// �ݱ� ��ư
	m_pBtnClose = (LSY::CButton *)GetObject(ID_GOLDMILEAGEPOPUP_BTN_CLOSE);
	m_pBtnClose->AddHandler(LSY::EM_O_MOUSELCLICK, LSY::Fnt(this, &CGoldMileagePopup::OnClickButton));
	// Ȯ�� ��ư
	m_pBtnOk = (LSY::CButton *)GetObject(ID_GOLDMILEAGEPOPUP_BTN_OK);
	m_pBtnOk->AddHandler(LSY::EM_O_MOUSELCLICK, LSY::Fnt(this, &CGoldMileagePopup::OnClickButton));
	
	// ��� �ؽ�Ʈ
	m_pTxtGoldAmount = (LSY::CText *)GetObject(ID_GOLDMILEAGEPOPUP_TXT_GOLDAMOUNT);
}

/// <summary>
/// ������ ����
/// <summary>
/// <param name=""></param>
/// <returns></returns>
void CGoldMileagePopup::InitData(INT64 nTempGold)
{
	// ��带 0000��0000��� �������� ǥ��
	CString strOutput;
	if (nTempGold >= 10000)
	{
		INT64 nTempGold1 = nTempGold / 10000;
		int nTempGold2 = nTempGold % 10000;
		strOutput.Format("%I64d��%04d%s", nTempGold1, nTempGold2, strChip_Name[(int)ROOM_CHIPKIND_GOLD]);
	}
	else
	{
		strOutput.Format("%I64d%s", nTempGold, strChip_Name[(int)ROOM_CHIPKIND_GOLD]);
	}

	m_pTxtGoldAmount->SetText((LPCTSTR)strOutput);
}

void CGoldMileagePopup::CloseDlg(void)
{
	OnCancel();
}

/// <summary>
/// �������� ������ ��Ŷ(����) ó��
/// <summary>
/// <param name=""></param>
/// <returns></returns>
void CGoldMileagePopup::HandleResponse(int Signal, int TotSize, char *lpData)
{
	switch(Signal)
	{
	case SV_GOLD_TEMPMONEY_REAL_UPDATE:
		{
			CSV_GOLD_TEMPMONEY_REAL_UPDATE msg;
			msg.Get(lpData, TotSize);
			
			m_TempGold = *msg.m_TempGold; //������Ʈ�Ǵ� �ӽ�(������) ��� �ݾ��̴�.
			m_RealGold = *msg.m_RealGold; //�ջ�Ǿ� ���� �����ϰԵ� ��� �ݾ��̴�.(���� �����Ӵ� ���� �Ѿ�)
		}
		break;
	}
}
