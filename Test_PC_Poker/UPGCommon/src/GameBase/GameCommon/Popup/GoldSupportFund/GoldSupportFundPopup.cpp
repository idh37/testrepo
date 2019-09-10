#include "StdAfx.h"
#include "GoldSupportFundPopup.h"
#include "goldsupportfundPopupid.h"

CGoldSupportFundPopup::CGoldSupportFundPopup(CWnd* pParent/* = NULL*/) : LSY::CLSYLibDialogBase(pParent)
{
	m_Grade = -1;
}

CGoldSupportFundPopup::~CGoldSupportFundPopup(void)
{
}

INT_PTR CGoldSupportFundPopup::DoModal(CWnd *pParent)
{
	m_pParentWnd = pParent;
	return CLSYLibDialogBase::DoModal(GetObjectMan(), ID_GOLDSUPPORTFUNDPOPUP);
}

bool CGoldSupportFundPopup::OnCreatedProject(void)
{
	// LSY Control ����
	InitLSYControl();
	// ������ ����
	InitData(m_Grade);
	//InitData(2); // test
	
	return TRUE;
}

/// <summary>
/// ��ư Ŭ�� �̺�Ʈ. ��κ� ������ ��û ������ ���.
/// <summary>
/// <param name=""></param>
/// <returns></returns>
LRESULT CGoldSupportFundPopup::OnClickButton(LSY::CMessage *pMsg)
{
	LSY::CObjectMessage *msg = (LSY::CObjectMessage *)pMsg;
	switch(msg->GetObject()->GetID())
	{
	case ID_GOLDSUPPORTFUNDPOPUP_BTN_OK: // Ȯ�� ��ư
		{
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
void CGoldSupportFundPopup::InitLSYControl()
{
	// �˾�
	//m_pWndPopup = (LSY::CWindows *)GetObject(ID_GOLDSUPPORTFUNDPOPUP);
	LSY::CObjectProject *pProject = m_pObjectMan->GetProjectFromID(m_nID);
	m_pWndPopup = (LSY::CWindows *)pProject->GetMainObject();
	
	// Ȯ�� ��ư
	m_pBtnOk = (LSY::CButton *)GetObject(ID_GOLDSUPPORTFUNDPOPUP_BTN_OK);
	m_pBtnOk->AddHandler(LSY::EM_O_MOUSELCLICK, LSY::Fnt(this, &CGoldSupportFundPopup::OnClickButton));
	
	// ������ �̹���
	m_pImgFund = (LSY::CImage *)GetObject(ID_GOLDSUPPORTFUNDPOPUP_IMG_FUND);
}

/// <summary>
/// ������ ����
/// <summary>
/// <param name=""></param>
/// <returns></returns>
void CGoldSupportFundPopup::InitData(int nGrade)
{
	switch (nGrade)
	{
	// �̴�÷
	case -1:
		{
			m_pWndPopup->SetIndex(1);
			m_pImgFund->SetShow(false);
		}
		break;
	// 1��
	case 1:
		{
			m_pWndPopup->SetIndex(0);
			m_pImgFund->SetIndex(8);
			m_pImgFund->SetShow(true);
		}
		break;
	// 2��
	case 2:
	case 3:
	case 4:
	case 5:
		{
			m_pWndPopup->SetIndex(0);
			m_pImgFund->SetIndex(7);
			m_pImgFund->SetShow(true);
		}
		break;
	// 3��
	case 6:
	case 7:
	case 8:
	case 9:
	case 10:
		{
			m_pWndPopup->SetIndex(0);
			m_pImgFund->SetIndex(6);
			m_pImgFund->SetShow(true);
		}
		break;
	// ���� �̻��ϰ� ���� �� �̴�÷ �˾� ���(������ ����..)
	default:
		{
			m_pWndPopup->SetIndex(1);
			m_pImgFund->SetShow(false);
		}
		break;
	}
}

void CGoldSupportFundPopup::CloseDlg(void)
{
	OnCancel();
}

/// <summary>
/// �������� ������ ��Ŷ(����) ó��
/// <summary>
/// <param name=""></param>
/// <returns></returns>
void CGoldSupportFundPopup::HandleResponse(int Signal, int TotSize, char *lpData) // ���� ���� �ڵ鸵
{
	switch(Signal)
	{
	case SV_GOLD_HELP_REWARD:
		{
			CSV_GOLD_HELP_REWARD msg;
			msg.Get(lpData, TotSize);
			
			m_Grade = *msg.m_Grade; //����.. �� ���� -1�̸� ���̴�...
			INT64 m_RewardGold = *msg.m_RewardGold; //������̴�.
		}
		break;
	}
}
