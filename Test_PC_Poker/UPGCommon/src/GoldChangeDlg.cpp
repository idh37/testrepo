#include "stdafx.h"
#include "GoldChangeDlg.h"
#include "goldchangedlgid.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif


CGoldChangeDlg::CGoldChangeDlg(CWnd* pParent/* = NULL*/)
:	LSY::CLSYLibDialogBase(pParent),
m_pBtn10(NULL),
m_pBtn100(NULL),
m_pBtn1000(NULL),
m_pBtn10000(NULL),
m_pBtn100000(NULL),
m_pBtnRemove(NULL),
m_pBtnMax(NULL),
m_pBtnChange(NULL),
m_pBtnClose(NULL),
m_pTxtCurMoney(NULL),
m_pTxtCurGold(NULL),
m_pTxtChangeGold(NULL),
m_pTxtResultMoney(NULL)
{
	m_nChangeGold = 0;
}

BEGIN_MESSAGE_MAP(CGoldChangeDlg, LSY::CLSYLibDialogBase)
END_MESSAGE_MAP()

INT_PTR CGoldChangeDlg::DoModal(CWnd *pParent)
{
	m_pParentWnd = pParent;
	return CLSYLibDialogBase::DoModal(GetObjectMan(), ID_GOLDCHANGEDLG);
}

LRESULT CGoldChangeDlg::OnInit(WPARAM &wParam, LPARAM &lParam)
{
	Init();

	return (LRESULT)wParam;
}

void CGoldChangeDlg::Init()
{
	m_nChangeGold = 0;
	m_pTxtCurMoney->SetNumber(GM().GetMyInfo()->UI.GetMoney());
	m_pTxtCurGold->SetNumber(GM().GetMyGold());

	m_nMaxGold = min((g_MaxServerMoney - GM().GetMyInfo()->UI.GetMoney()) / GM().GetGoldValue(), GM().GetMyGold());
	SetChangeGold(0ll);
	//SetChangeGold(0);
}

bool CGoldChangeDlg::OnCreatedProject(void)
{
	m_pBtn10 = (LSY::CButton *)GetObject(ID_GOLDCHANGEDLG_BTN_10);
	m_pBtn10->AddHandler(LSY::EM_O_MOUSELCLICK, LSY::Fnt(this, &CGoldChangeDlg::OnClickButton));
	
	m_pBtn100 = (LSY::CButton *)GetObject(ID_GOLDCHANGEDLG_BTN_100);
	m_pBtn100->AddHandler(LSY::EM_O_MOUSELCLICK, LSY::Fnt(this, &CGoldChangeDlg::OnClickButton));
	
	m_pBtn1000 = (LSY::CButton *)GetObject(ID_GOLDCHANGEDLG_BTN_1000);
	m_pBtn1000->AddHandler(LSY::EM_O_MOUSELCLICK, LSY::Fnt(this, &CGoldChangeDlg::OnClickButton));
	
	m_pBtn10000 = (LSY::CButton *)GetObject(ID_GOLDCHANGEDLG_BTN_10000);
	m_pBtn10000->AddHandler(LSY::EM_O_MOUSELCLICK, LSY::Fnt(this, &CGoldChangeDlg::OnClickButton));
	
	m_pBtn100000 = (LSY::CButton *)GetObject(ID_GOLDCHANGEDLG_BTN_100000);
	m_pBtn100000->AddHandler(LSY::EM_O_MOUSELCLICK, LSY::Fnt(this, &CGoldChangeDlg::OnClickButton));
	
	m_pBtnRemove = (LSY::CButton *)GetObject(ID_GOLDCHANGEDLG_BTN_REMOVE);
	m_pBtnRemove->AddHandler(LSY::EM_O_MOUSELCLICK, LSY::Fnt(this, &CGoldChangeDlg::OnClickButton));
	
	m_pBtnMax = (LSY::CButton *)GetObject(ID_GOLDCHANGEDLG_BTN_MAX);
	m_pBtnMax->AddHandler(LSY::EM_O_MOUSELCLICK, LSY::Fnt(this, &CGoldChangeDlg::OnClickButton));
	
	m_pBtnChange = (LSY::CButton *)GetObject(ID_GOLDCHANGEDLG_BTN_CHANGE);
	m_pBtnChange->AddHandler(LSY::EM_O_MOUSELCLICK, LSY::Fnt(this, &CGoldChangeDlg::OnClickButton));
	
	m_pBtnClose = (LSY::CButton *)GetObject(ID_GOLDCHANGEDLG_BTN_CLOSE);
	m_pBtnClose->AddHandler(LSY::EM_O_MOUSELCLICK, LSY::Fnt(this, &CGoldChangeDlg::OnClickButton));
	
	m_pTxtCurMoney = (LSY::CTextNumber *)GetObject(ID_GOLDCHANGEDLG_TEXT_CUR_MONEY);
	m_pTxtCurGold = (LSY::CTextNumber *)GetObject(ID_GOLDCHANGEDLG_TEXT_CUR_GOLD);
	m_pTxtChangeGold = (LSY::CEdit *)GetObject(ID_GOLDCHANGEDLG_TEXT_CHANGE_GOLD);
	// �ؽ�Ʈ �ڽ��� ���� Ű���� �Է����� ���������� OnKeyUpEdit �Լ��� ȣ���Ŵ
	m_pTxtChangeGold->AddHandler(LSY::EM_O_KEYUP, LSY::Fnt(this, &CGoldChangeDlg::OnKeyUpEdit));
	m_pTxtChangeGold->SetText("0");
	m_pTxtResultMoney = (LSY::CTextNumber *)GetObject(ID_GOLDCHANGEDLG_TEXT_RESULT_MONEY);
	
	Init();
	
	return TRUE;
}

/// <summary>
/// ��ȯ ���, ���� �����Ӵ� ���� �����Ѵ�.
/// m_nChangeGold ���� ���� ���õǹǷ� m_nChangeGold ���� ������ �� ȣ���ؾ���.
/// <summary>
/// <param name=""></param>
/// <returns></returns>
void CGoldChangeDlg::SetChangeGold(INT64 nChangeGold)
{
	// ĸ��ȭ - ��ȯ ��忡 ���� ���� ���� �Ӵϰ� ���� �Ǿ���.
	
	// ���� ����(����ȯ)
	m_nChangeGold = nChangeGold;						// ���� ���� ����
	CString csChangeGold;								// CString�� ��ȯ ���
	csChangeGold.Format("%d", nChangeGold);
	std::string strChangeGold((LPCTSTR)csChangeGold);	// string�� ��ȯ ���
	
	// ��ȯ ���(�ؽ�Ʈ �ڽ� ��Ʈ��) ����
	m_pTxtChangeGold->SetText(strChangeGold);
	
	// ���� ���� �Ӵ�(������ �ؽ�Ʈ ��Ʈ��) ����
	m_pTxtResultMoney->SetNumber(GM().GetMyInfo()->UI.GetMoney() + m_nChangeGold * GM().GetGoldValue());
}

/// <summary>
/// ��ȯ ���, ���� �����Ӵ� ���� �����Ѵ�.
/// m_nChangeGold ���� ���� ���õǹǷ� m_nChangeGold ���� ������ �� ȣ���ؾ���.
/// <summary>
/// <param name=""></param>
/// <returns>��ȿ�� �˻� ���</returns>
bool CGoldChangeDlg::ChangeGold(INT64 addValue)
{
	// ��� ��ȭ�� ���� ��� ����
	if (addValue == 0)
		return false;
	
	// ���� ����(����ȯ)
	CString csMaxGold;											// csMaxGold : CString�� �ִ� ���
	csMaxGold.Format("%d", m_nMaxGold);							// m_nMaxGold : �̸� �����س� �ִ� ��� ���� ����
	std::string strMaxGold((LPCTSTR)csMaxGold);					// strMaxGold : string�� �ִ� ���
	
	// ������ ��� �ѵ��� �ʰ��Ͽ� �Է����� ���
	if (m_nChangeGold + addValue > GM().GetMyGold())
	{
		//AfxMessageBox(_T("������ ��尡 �����մϴ�. \n������� ������ ��ȯ���ּ���."));
		
		// �Է��� ��尡 ��ȯ �ѵ��� ������� �ѵ��� ���� �ʵ��� ����
		//m_nChangeGold = min(m_nMaxGold, m_nChangeGold + addValue);
		m_nChangeGold = m_nMaxGold;
		SetChangeGold(m_nChangeGold);
		
		return false;
	}
	
	// ������ ��� �ѵ��� �ʰ��Ͽ� �Է����� ���
	if (m_nChangeGold + addValue > m_nMaxGold)
	{
		ShowMainMessageDlg("�����Ҽ� �ִ� ���ӸӴ��� �ѵ��� �ʰ��Ͽ����ϴ�.\n�����ѵ� ������ ��ȯ���ּ���.");
		
		// �Է��� ��尡 ��ȯ �ѵ��� ������� �ѵ��� ���� �ʵ��� ����
		//m_nChangeGold = min(m_nMaxGold, m_nChangeGold + addValue);
		m_nChangeGold = m_nMaxGold;
		SetChangeGold(m_nChangeGold);
		
		return false;
	}

	// ��ȿ�� �˻縦 ������� ��� ������ �Է��� ���� ��ȯ ��忡 ����
	m_nChangeGold += addValue;
	// ��ȯ ���, ���� �����Ӵ� �� ����
	SetChangeGold(m_nChangeGold);
	
	return true;
}

/// <summary>
/// ��ȿ�� �˻� - ��ȯ Ÿ�̹�
/// <summary>
/// <returns>��ȿ�� �˻� ���</returns>
bool CGoldChangeDlg::ValidationCheckChange()
{
	// ���� ����(����ȯ)
	CString csInputGold(m_pTxtChangeGold->GetText().c_str());
	INT64 nInputGold = _ttoi(csInputGold);

	if (nInputGold < 10)
	{
		CString csMsg;
		csMsg.Format("��ȯ �ּ� �ѵ��� 10%s �Դϴ�.", strChip_Name[(int)ROOM_CHIPKIND_GOLD]);
		ShowMainMessageDlg(csMsg);

		return false;
	}
	
	return true;
}

/// <summary>
/// ��� �ؽ�Ʈ �ڽ��� ���� Ű���� �Է����� ���������� ȣ��Ǵ� �Լ�
/// <summary>
/// <param name="msg">��� ���� �ڵ�(��Ƽ��, ��Ƽ��, ��ī�� ���..)</param>
/// <returns></returns>
LRESULT CGoldChangeDlg::OnKeyUpEdit(LSY::CMessage *msg)
{
	// ���� ����(����ȯ)
	CString csInputGold(m_pTxtChangeGold->GetText().c_str());	// csInputGold : CString�� �Է� ���, m_pTxtChangeGold : ���� ��� ����Ʈ �ڽ�
	INT64 nInputGold = _ttoi(csInputGold);						// nInputGold : INT64�� �Է� ���
	LSY::CMO_KeyUp *pMsg = (LSY::CMO_KeyUp *)msg;

	if (csInputGold.IsEmpty())
	{
		SetChangeGold(0);
	}
	// ������ ���ڰ� �ƴ� ���� �Է����� ���
	// NumLock, �齺���̽�, Del, �¿����Ű�� �Է� ����
	else if (!((pMsg->GetValue()>=48 && pMsg->GetValue()<=57) || (pMsg->GetValue()>=96 && pMsg->GetValue()<=105)
		|| pMsg->GetValue()==8 || pMsg->GetValue()==46 || pMsg->GetValue()==37 || pMsg->GetValue()==39 || pMsg->GetValue()==144))
	{
		ShowMainMessageDlg("���ڸ� �Է� �����մϴ�.");
		SetChangeGold(m_nChangeGold);
		
		return FALSE;
	}
	
	ChangeGold(nInputGold - m_nChangeGold);
	
	//LSY::CMO_Return *pMsg = (LSY::CMO_Return *)msg;
	//LSY::CEdit *pEdit = (LSY::CEdit *)pMsg->GetObject();
	//switch(pEdit->GetID())
	//{
	//case ID_GOLDCHANGEDLG_TEXT_CHANGE_GOLD:
	//	FindUser();
	//	break;
	//}

	return TRUE;
}

LRESULT CGoldChangeDlg::OnClickButton(LSY::CMessage *pMsg)
{
	LSY::CObjectMessage *msg = (LSY::CObjectMessage *)pMsg;
	switch(msg->GetObject()->GetID())
	{
	case ID_GOLDCHANGEDLG_BTN_10:
		ChangeGold(10ll);
		break;
	case ID_GOLDCHANGEDLG_BTN_100:
		ChangeGold(100ll);
		break;
	case ID_GOLDCHANGEDLG_BTN_1000:
		ChangeGold(1000ll);
		break;
	case ID_GOLDCHANGEDLG_BTN_10000:
		ChangeGold(10000ll);
		break;
	case ID_GOLDCHANGEDLG_BTN_100000:
		ChangeGold(100000ll);
		break;
	case ID_GOLDCHANGEDLG_BTN_REMOVE:
		ChangeGold(-m_nChangeGold);
		break;
	case ID_GOLDCHANGEDLG_BTN_MAX:
		{
			// �ִ� ������ ����(��ȣ�� �˻� �ʿ� ����)
			m_nChangeGold = m_nMaxGold;
			// ��ȯ ���, ���� �����Ӵ� �� ����
			ChangeGold(m_nChangeGold);
			//ChangeGold(GM().GetMyGold() - m_nChangeGold);
		}
		break;
	case ID_GOLDCHANGEDLG_BTN_CHANGE:
		{
			if (ValidationCheckChange() == false)
				break;
			CCL_CHANGE_ETCCHIP msg;
			msg.Set("GD", m_nChangeGold);
			NMBASE::SOCK::GSOCKMAN().SendData(g_strMainServerName.GetBuffer(), msg.pData, msg.GetTotalSize());
		}
		break;
	case ID_GOLDCHANGEDLG_BTN_CLOSE:
		OnCancel();
		break;
	}
	return TRUE;
}